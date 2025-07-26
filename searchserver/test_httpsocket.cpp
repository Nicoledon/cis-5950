/*
 * Copyright ©2025 Travis McGaha.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Pennsylvania
 * CIT 5950 for use solely during Spring Semester 2025 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

extern "C" {
  #include <pthread.h>  // for the pthread threading/mutex functions
}

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <optional>

#include "./HttpSocket.hpp"

#include "./HttpUtils.hpp"
#include "./catch.hpp"

using std::string;
using std::optional;

using searchserver::HttpSocket;
using searchserver::wrapped_read;
using searchserver::wrapped_write;

static pthread_mutex_t rw_lock;
static int num_read = 0;
static int num_write = 0;

static void WritePartialRequests(void* args);

static void *WriteWrapper(void* args) {
  WritePartialRequests(args);
  return nullptr;
}

TEST_CASE("Basic", "[Test_HttpSocket]") {

  // Create a socketpair; we'll hand one end of the socket to the
  // HttpSocket object, and use the other end of the socket
  // ourselves for testing.
  int spair[2] = {-1, -1};
  REQUIRE(socketpair(AF_UNIX, SOCK_STREAM, 0, spair) == 0);
  // Create the HttpSocket object we'll test.
  struct sockaddr sock{};
  HttpSocket hc(spair[0], sizeof(sock), &sock);

  // Write three requests on the socket.
  string req1 = "GET /foo HTTP/1.1\r\n";
  req1 += "Host: somehost.foo.bar\r\n";
  req1 += "Connection: close\r\n";
  req1 += "\r\n";
  // req2: check header order doesn't matter
  string req2 = "GET /bar HTTP/1.1\r\n";
  req2 += "Connection: close\r\n";
  req2 += "Host: somehost.foo.bar\r\n";
  req2 += "\r\n";
  // req3: check different values
  string req3 = "GET /baz HTTP/1.1\r\n";
  req3 += "connection: keep-alive\r\n";
  req3 += "host: somehost.foo.bar\r\n";
  req3 += "OTHER: some_value\r\n";
  req3 += "\r\n";

  REQUIRE(static_cast<int>(req1.size())==
            wrapped_write(spair[1], req1));
  REQUIRE(static_cast<int>(req2.size())==
            wrapped_write(spair[1], req2));
  REQUIRE(static_cast<int>(req3.size())==
            wrapped_write(spair[1], req3));

  // Do the next_requests.
  optional<string> htreq1, htreq2, htreq3;
  htreq1 = hc.next_request();
  REQUIRE(htreq1.has_value());
  htreq2 = hc.next_request();
  REQUIRE(htreq2.has_value());
  htreq3 = hc.next_request();
  REQUIRE(htreq3.has_value());


  // Make sure the request parsing worked.
  REQUIRE(req1 == htreq1.value());
  REQUIRE(req2 == htreq2.value());
  REQUIRE(req3 == htreq3.value());
  

  // Test a "split" response being written
  // string req4 = "GET /foo HTTP/1.1\r\n";
  // req1 += "Host: somehost.foo.bar\r\n";
  // req1 += "Connection: close\r\n";
  // req1 += "\r\n";
  // req2: check header order doesn't matter
  // string req5 = "GET /bar HTTP/1.1\r\n";
  // req2 += "Connection: close\r\n";
  // req2 += "Host: somehost.foo.bar\r\n";
  // req2 +=  "\r\n";

  // Prepare the responses.
  string expectedrep1 = "HTTP/1.1 200 OK\r\n";
  expectedrep1 += "Content-length: 33\r\n\r\n";
  expectedrep1 += "This is the body of the response.";

  string expectedrep2 = "HTTP/1.1 200 OK\r\n";
  expectedrep2 += "Content-type: text/html\r\n";
  expectedrep2 += "Content-length: 28\r\n\r\n";
  expectedrep2 += "This is the second response.";

  // Generate the responses, test them.
  string buf1;
  REQUIRE(hc.write_response(expectedrep1));
  REQUIRE(72 == wrapped_read(spair[1], &buf1));
  REQUIRE(expectedrep1 == buf1);

  string buf2;
  REQUIRE(hc.write_response(expectedrep2));
  REQUIRE(92 == wrapped_read(spair[1], &buf2));
  REQUIRE(expectedrep2 == buf2);

  // Clean up.
  close(spair[0]);
  close(spair[1]);
  
}

TEST_CASE("PartialRead", "[Test_HttpSocket]") {


  // Create a socketpair; we'll hand one end of the socket to the
  // HttpSocket object, and use the other end of the socket
  // ourselves for testing.
  int spair[2] = {-1, -1};
  REQUIRE(0 == socketpair(AF_UNIX, SOCK_STREAM, 0, spair));

  // Note that we use sleep() a lot to try and "motivate"
  // the threads to swap. This is beacuse valgrind isn't
  // the happiest when we have multiple threads so
  // sleep is needed to make it cooperate.

  // Create the HttpSocket object we'll test.
  struct sockaddr sock{};
  HttpSocket hc(spair[0], sizeof(sock), &sock);

  // create a thread to write on the other end of the socket
  // so that we can test our http connection class
  REQUIRE(0 == pthread_mutex_init(&rw_lock, nullptr));
  pthread_t thread_id;
  REQUIRE(0 == pthread_create(&thread_id, nullptr, WriteWrapper, &(spair[1])));
  // sleep so that the write thread can start writing to us
  sleep(1);

  // Do the next_requests.
  optional<string> htreq1, htreq2, htreq3;

  // loop until we know that the write socket has started to
  // write the first request.
  while (1) {
    REQUIRE(0 == pthread_mutex_lock(&rw_lock));
    if (num_write == 1) {
      num_read += 1;
      REQUIRE(0 == pthread_mutex_unlock(&rw_lock));
      htreq1 = hc.next_request();
      REQUIRE(htreq1.has_value());
      break;
    }
    REQUIRE(0 == pthread_mutex_unlock(&rw_lock));
    sleep(1);
  }

  // Make sure the request parsing worked.
  string req1 = "GET /foo HTTP/1.1\r\n";
  req1 += "Host: somehost.foo.bar\r\n";
  req1 += "Connection: close\r\n";
  req1 += "\r\n";

  REQUIRE(req1 == htreq1.value());
  
  sleep(1);  // sleep so that req2 can be sent

  // start parsing req2
  htreq2 = hc.next_request();
  REQUIRE(htreq2.has_value());
  string req2 = "GET /bar HTTP/1.1\r\n";
  req2 += "Connection: close\r\n";
  req2 += "Host: somehost.foo.bar\r\n";
  req2 += "\r\n";
  REQUIRE(req2 == htreq2.value());
  

  // loop until we know that the write socket has started to
  // write the third request.
  while (1) {
    REQUIRE(0 == pthread_mutex_lock(&rw_lock));
    if (num_write == 2) {
      num_read += 1;
      REQUIRE(0 == pthread_mutex_unlock(&rw_lock));
      break;
    }
    REQUIRE(0 == pthread_mutex_unlock(&rw_lock));
    sleep(1);
  }

  // verify that req3 is correct
  htreq3 = hc.next_request();
  REQUIRE(htreq3.has_value());

  string req3 = "GET /baz HTTP/1.1\r\n";
  req3 += "connection: keep-alive\r\n";
  req3 += "host: somehost.foo.bar\r\n";
  req3 += "OTHER: some_value\r\n\r\n";

  REQUIRE(req3 == htreq3.value());

  // make sure that we read/wrote the correct
  // number of times and clean up
  REQUIRE(0 == pthread_mutex_lock(&rw_lock));
  num_read += 1;
  REQUIRE(3 == num_write);
  REQUIRE(3 == num_read);
  REQUIRE(0 == pthread_mutex_unlock(&rw_lock));
  REQUIRE(0 == pthread_mutex_destroy(&rw_lock));
  REQUIRE(0 == pthread_join(thread_id, nullptr));

  

  // Clean up.
  close(spair[0]);
  close(spair[1]);
}

static void WritePartialRequests(void* args) {
  int socket = *static_cast<int*>(args);
  // Write three requests on the socket.
  // Note that the writes for these requests
  // are split up in such a way to test that
  // your http connection works properly if
  // some of the request that are read are not
  // read completely.
  string req1 = "GET /foo HTTP/1.1\r\n";
  req1 += "Host: somehost.foo.bar\r\n";
  req1 += "Connection: close\r\n";
  req1 += "\r\nGET /bar ";
  // req2: check that it is ok if the request
  // is split in the first line
  // across calls to wrapped_read().
  string req2 = "HTTP/1.1\r\n";
  req2 += "Connection: close\r\n";
  req2 += "Host: somehost.foo.bar\r\n";
  req2 += "\r\nGET /baz HTTP/1.1\r\n";
  req2 += "connection:";
  // req3: check that it is ok if the request
  // is split in the "headers" and during the
  // end \r\n\r\n sequence
  string req3 = " keep-alive\r\n";
  req3 += "host: somehost.foo.bar\r\n";
  req3 += "OTHER: some_value\r\n";
  string req3tail = "\r\n";

  // acquire the rw lock to make sure that
  // the socket doesn't read until we have
  // written req1
  REQUIRE(0 == pthread_mutex_lock(&rw_lock));
  REQUIRE(0 == num_read);
  REQUIRE(0 == num_write);

  // write req1
  REQUIRE(static_cast<int>(req1.size()) ==
            wrapped_write(socket, req1));

  // Release lock and update num_write so that
  // the read socket thread knows it can proceed
  // to read req1
  num_write += 1;
  REQUIRE(0 == pthread_mutex_unlock(&rw_lock));
  sleep(3);

  // Loop trying to acquire the lock but checking
  // to make sure that the client has had the chance
  // to start reading req1
  while (1) {
    REQUIRE(0 == pthread_mutex_lock(&rw_lock));
    if (num_read == 1) {
      num_write += 1;
      break;
    }
    REQUIRE(0 == pthread_mutex_unlock(&rw_lock));
    sleep(1);
  }

  // now that we know req1 has started to have been read
  // (probably), we write req2.
  REQUIRE(static_cast<int>(req2.size()) ==
            wrapped_write(socket, req2));
  REQUIRE(0 == pthread_mutex_unlock(&rw_lock));
  sleep(3);

  // Loop trying to acquire the lock but checking
  // to make sure that the reading thread has had
  // the chance to start reading req2
  while (1) {
    REQUIRE(0 == pthread_mutex_lock(&rw_lock));
    if (num_read == 2) {
      num_write += 1;
      break;
    }
    REQUIRE(0 == pthread_mutex_unlock(&rw_lock));
    sleep(1);
  }

  // Write the first part of req3
  REQUIRE(static_cast<int>(req3.size()) ==
            wrapped_write(socket, req3));
  REQUIRE(0 == pthread_mutex_unlock(&rw_lock));

  // sleep so that the read thread has a chance to
  // start reading req3.
  sleep(3);

  // write the last \r\n necessary to mark the
  // end of req3. If you are going infinite at
  // this point, you are not handling the reading
  // of http requests properly
  REQUIRE(static_cast<int>(req3tail.size()) ==
            wrapped_write(socket, req3tail));
}

TEST_CASE("write_response", "[Test_HttpSocket]") {

  string expected = "HTTP/1.1 200 OK\r\nContent-type: text/plain\r\n";
  expected += "Content-length: 3\r\n\r\nhi!";

  int pipefds[2];
  pipe(pipefds);
  
  struct sockaddr sock{};
  HttpSocket connection(pipefds[1], sizeof(sock), &sock);
  connection.write_response(expected);

  string actual;
  wrapped_read(pipefds[0], &actual);
  REQUIRE(expected == actual);
  close(pipefds[0]);
}

