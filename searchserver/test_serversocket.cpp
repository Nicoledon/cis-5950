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

#include <unistd.h>
#include <stdint.h>
#include <iostream>
#include <string>
#include <cstdlib>

#include "./ServerSocket.hpp"
#include "./HttpUtils.hpp"
#include "./ThreadPool.hpp"
#include "./catch.hpp"

#include <optional>

using std::cout;
using std::cerr;
using std::endl;
using std::string;

using searchserver::ServerSocket;
using searchserver::connect_to_server;
using searchserver::rand_port;
using searchserver::ThreadPool;
using searchserver::HttpSocket;

static uint16_t portnum;

// A task object created to hold the necessary information
// of what information is retrived from the server when
// Accept is run.
struct ServerTaskArgs {

  // public fields to store conneciton information.
  std::optional<HttpSocket> opt;
  bool setup_done = false;
  bool task_done = false;
  pthread_mutex_t mutex;
  pthread_cond_t cond;
};

void* TestSSThreadFn(void *arg) {
  ServerTaskArgs *task = reinterpret_cast<ServerTaskArgs *>(arg);

  // Create the server socket.
  cout << "Creating ServerSocket on " << portnum << endl;
  ServerSocket ss(AF_INET, "127.0.0.1", portnum);

  pthread_mutex_lock(&(task->mutex));
  task->setup_done = true;
  pthread_cond_signal(&(task->cond));
  pthread_mutex_unlock(&(task->mutex));


  // Accept a connection.
  cout << "Doing accept..." << endl;
  task->opt = ss.accept_client();
  REQUIRE(task->opt.has_value());

  // It worked!
  cout << "Accept succeeded." << endl;
  task->task_done = true;

  return nullptr;
}

TEST_CASE("Basic", "[Test_ServerSocket]") {
  // Create a threadpool, and dispatch a thread to go listen on a
  // server socket on a random port.

  portnum = rand_port();

  pthread_t thd;

  auto args = new ServerTaskArgs();
  REQUIRE(pthread_mutex_init(&(args->mutex), nullptr) == 0);
  REQUIRE(pthread_cond_init(&(args->cond), nullptr) == 0);

  REQUIRE(pthread_create(&thd, nullptr, TestSSThreadFn, args) == 0);

  // Give the thread a chance to create the socket.
  pthread_mutex_lock(&(args->mutex));
  while (!args->setup_done) {
    pthread_cond_wait(&(args->cond), &(args->mutex));
  }
  pthread_mutex_unlock(&(args->mutex));


  // Connect to the socket
  cerr << "Attempting to connect to 127.0.0.1 port "
       << portnum << endl;
  int cfd = -1;
  REQUIRE(connect_to_server("127.0.0.1", portnum, &cfd));

  // Make sure that the server had a chance to get client & server info
  REQUIRE(pthread_join(thd, nullptr) == 0);

  REQUIRE(args->task_done);

  // verify that the file descriptor is valid.
  // cfd needs to be non-negative
  REQUIRE(0 <= cfd );



  // Check that the output params
  // (caddr, cport, cdns, saddr, sdns)
  // are set correctly.
  cout << "Checking output params from Accept..." << endl;

  HttpSocket sck = std::move(args->opt.value());

  // check the port...
  REQUIRE(0 < sck.client_port() );

  // check the fd...
  REQUIRE(0 < sck.fd() );


  // check the addresses...

  REQUIRE("127.0.0.1" ==  sck.server_addr());
  // client address could be in either ipv4 or ipv6,
  // depending on implementation and ConnectToServer()
  REQUIRE(((sck.client_addr() == "::ffff:127.0.0.1") || (sck.client_addr() == "127.0.0.1")));

  pthread_mutex_destroy(&(args->mutex));
  pthread_cond_destroy(&(args->cond));
  delete args;

  close(cfd);

}


