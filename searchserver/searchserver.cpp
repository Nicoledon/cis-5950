#include "./CrawlFileTree.hpp"
#include "./HttpUtils.hpp"
#include "./ServerSocket.hpp"
#include "./ThreadPool.hpp"
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
using namespace std;

pthread_mutex_t m;

struct thd_arg {
  std::string origin;
  searchserver::ServerSocket* server;
  searchserver::WordIndex item;
};

std::string
read_files(const std::string &path) {
  std::ifstream in(path, std::ios::binary);
  if (!in)
    throw std::runtime_error("open failed");
  return std::string(std::istreambuf_iterator<char>(in),
                     std::istreambuf_iterator<char>());
}
void TestTaskFn(void *arg) {
  pthread_mutex_lock(&m);
  struct thd_arg * value = (struct thd_arg *)arg;
  searchserver::ServerSocket *server = value->server;
  auto opt_httpserver = server->accept_client();
  auto origin = value->origin;
  auto item = value->item;
  pthread_mutex_unlock(&m);
  if (opt_httpserver.has_value()) {
    auto &httpserver = opt_httpserver.value();
    auto opt_string = httpserver.next_request();
    std::string str = "";
    if (opt_string.has_value()) {
      str = opt_string.value();
    }
    auto num = str.find_first_of("\r\n");
    str = str.substr(0, num);
    auto s = searchserver::split(str, " ");
    searchserver::URLParser p;
    p.parse(s[1]);
    std::string path = p.path();
    std::string content = "";
    if (path == "/") {
      path = "./sample_http/initial_response.txt";
      content = read_files(path);
      httpserver.write_response(content);
    } else if(p.args().size() == 0){
      std::string head = "HTTP/1.1 200 OK\r\n";
      std::string type = "Content-type: text/plain\r\n";
      std::string len = "Content-length: ";
      path = "." + path;
      content = read_files(path);
      size_t size = content.size();
      std::string str_size = to_string(size);
      len += (str_size + "\r\n\r\n");
      std::string file = head + type + len + content;
      httpserver.write_response(searchserver::escape_html(file));
    }else{
      std::string head = "HTTP/1.1 200 OK\r\n";
      std::string len = "Content-length: ";
      std::string somecontent = "<html><head><title>595gle</title></head><body><center style="font-size:500%;"><span style="position:relative;bottom:-0.33em;color:orange;">5</span><span style="color:red;">9</span><span style="color:gold;">5</span><span style="color:blue;">g</span><span style="color:green;">l</span><span style="color:red;">e</span></center><p><div style="height:20px;"></div><center><form action="/query" method="get"><input type="text" size=30 name="terms" /><input type="submit" value="Search" /></form></center><p><p>
      ";
      std::string temp = "<br>22 results found for <b>hello</b><p>";
      std::string start = "<ul>";
      
      content = read_files(path);
      httpserver.write_response(content);
    }
  }
  delete arg;
}
searchserver::URLParser parse_url(searchserver::HttpSocket &httpserver) {
  auto str = httpserver.next_request();
  auto temp = str.value().find_first_of("\r\n");
  auto temp_str = str.value().substr(0, temp);
  auto temp_vec = searchserver::split(temp_str, " ");
  searchserver::URLParser p;
  p.parse(temp_vec[1]);
  return p;
}
int main(int argc, char *argv[]) {
  // TODO
  // Combine everything together!
  if (argc < 2) {
    exit(1);
  }
  std::string str_port = std::string(argv[1]);
  uint16_t port = static_cast<uint16_t>(std::stoi(str_port));
  std::string dictory = std::string(argv[2]);
  auto val = searchserver::crawl_filetree(dictory);
  if (!val.has_value()) {
    exit(1);
  }
  auto value = val.value();
  pthread_mutex_init(&m, NULL);
  auto server = searchserver::ServerSocket(AF_INET, "127.0.0.1", port);
  searchserver::ThreadPool pool(10);
  while (1) {
    struct thd_arg *thd = new struct thd_arg;
    thd->server = &server ;
    thd->origin = dictory;
    thd->item = value;
    searchserver::ThreadPool::Task next_t = {TestTaskFn, thd};
    pool.dispatch(next_t);
    usleep(1250000); // 1.25s
  }
  // std::string path = "./sample_http/initial_response.txt";
  // std::string content = read_files(path);
  // httpserver.write_response(content);
  pthread_mutex_destroy(&m);
  // You can just use AF_INET "127.0.0.1" as the address for the searchserver
  // for simplicity.
  return EXIT_SUCCESS;
}
