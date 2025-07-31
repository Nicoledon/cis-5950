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

std::string read_files(const std::string &path) {
  std::ifstream in(path, std::ios::binary);
  if (!in)
    throw std::runtime_error("open failed");
  return std::string(std::istreambuf_iterator<char>(in),
                     std::istreambuf_iterator<char>());
}
void TestTaskFn(void *arg) {
  pthread_mutex_lock(&m);
  searchserver::ServerSocket *server = (searchserver::ServerSocket *)arg;
  auto opt_httpserver = server->accept_client();
  pthread_mutex_unlock(&m);
  auto &httpserver = opt_httpserver.value();
  std::string path = "./sample_http/sotired_response.txt";
  std::string content = read_files(path);
  httpserver.write_response(content);
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
  // auto val = searchserver::crawl_filetree(dictory);
  // if(!val.has_value()){
  //     exit(1);
  // }
  // auto value  = val.value();
  pthread_mutex_init(&m, NULL);
  auto server = searchserver::ServerSocket(AF_INET, "127.0.0.1", port);
  searchserver::ThreadPool pool(10);
  while (1) {
    searchserver::ThreadPool::Task next_t = {TestTaskFn, &server};
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
