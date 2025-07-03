#include "./SimpleKV.hpp"

using namespace std;

namespace simplekv {

// General Operations
vector<string> SimpleKV::namespaces() {
  vector<string> res {};
  return res;
}

vector<string> SimpleKV::keys(const string& nspace) {
  vector<string> res {};
  return res;
}

bool SimpleKV::ns_exists(const string& nspace) {
  return false;
}

bool SimpleKV::key_exists(const string& nspace, const string& key) {
  return false;
}

value_type_info SimpleKV::type(const std::string& nspace,
                               const std::string& key) {
  return value_type_info::none;
}

bool SimpleKV::del(const string& nspace, const string& key) {
  return false;
}

// string operations

optional<string> SimpleKV::sget(const string& nspace, const string& key) {
  return nullopt;
}

void SimpleKV::sset(const string& nspace,
                    const string& key,
                    const string& value) {
  // do nothing
}

// list operations

ssize_t SimpleKV::llen(const string& nspace, const string& key) {
  return -1;
}

optional<string> SimpleKV::lindex(const string& nspace,
                                  const string& key,
                                  size_t index) {
  return nullopt;
}

optional<vector<string>> SimpleKV::lmembers(const string& nspace,
                                            const string& key) {
  return nullopt;
}

bool SimpleKV::lset(const string& nspace,
                    const string& key,
                    size_t index,
                    const string& value) {
  return false;
}

bool SimpleKV::lpush(const string& nspace,
                     const string& key,
                     const string& value) {

  return false;
}

optional<string> SimpleKV::lpop(const string& nspace, const string& key) {
  return nullopt;
}

bool SimpleKV::rpush(const string& nspace,
                     const string& key,
                     const string& value) {
  return false;
}

optional<string> SimpleKV::rpop(const string& nspace, const string& key) {
  return nullopt;
}

optional<vector<string>> SimpleKV::lunion(const string& nspace1,
                                          const string& key1,
                                          const string& nspace2,
                                          const string& key2) {
  return nullopt;
}

optional<vector<string>> SimpleKV::linter(const string& nspace1,
                                          const string& key1,
                                          const string& nspace2,
                                          const string& key2) {
  return nullopt;
}

optional<vector<string>> SimpleKV::ldiff(const string& nspace1,
                                         const string& key1,
                                         const string& nspace2,
                                         const string& key2) {
  return nullopt;
}

}  // namespace simplekv
