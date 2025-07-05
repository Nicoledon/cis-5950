#include "./SimpleKV.hpp"

using namespace std;

namespace simplekv {

// General Operations
vector<string> SimpleKV::namespaces() {
  vector<string> res{};
  for (auto const &item : container) {
    res.push_back(item.first);
  }
  return res;
}

vector<string> SimpleKV::keys(const string &nspace) {
  vector<string> res{};
  auto items = container[nspace];
  for (auto const &item : items) {
    res.push_back(item.first);
  }
  return res;
}

bool SimpleKV::ns_exists(const string &nspace) {
  if (container.contains(nspace)) {
    return true;
  }
  return false;
}

bool SimpleKV::key_exists(const string &nspace, const string &key) {
  auto item = container[nspace];
  if (item.contains(key)) {
    return true;
  }
  return false;
}

value_type_info SimpleKV::type(const std::string &nspace,
                               const std::string &key) {
  if (this->container[nspace].contains(key)) {
    return this->container[nspace][key].second;
  }
  return value_type_info::none;
}

bool SimpleKV::del(const string &nspace, const string &key) {
  if (this->container[nspace].contains(key)) {
    this->container[nspace].erase(key);
    if (this->container[nspace].empty()) {
      this->container.erase(nspace);
    }
    return true;
  }
  return false;
}

// string operations

optional<string> SimpleKV::sget(const string &nspace, const string &key) {
  auto &ns = this->container[nspace];
  if (!ns.contains(nspace) && ns[key].second != value_type_info::string) {
    return nullopt;
  }
  optional<string> item;
  if (holds_alternative<string>(ns[key].first)) {
    item = get<string>(ns[key].first);
  }
  return item;
}

void SimpleKV::sset(const string &nspace, const string &key,
                    const string &value) {
  variant<string, vector<string>> item = value;
  auto p = make_pair(value, value_type_info::string);
  this->container[nspace][key] = p;
  // do nothing
}

// list operations

ssize_t SimpleKV::llen(const string &nspace, const string &key) {
  auto &ns = this->container[nspace];
  if (ns[key].second == value_type_info::list) {
    vector<string> item;
    if (holds_alternative<vector<string>>(ns[key].first)) {
      item = get<vector<string>>(ns[key].first);
      return item.size();
    }
  }
  return -1;
}

optional<string> SimpleKV::lindex(const string &nspace, const string &key,
                                  size_t index) { 
  return nullopt;
}

optional<vector<string>> SimpleKV::lmembers(const string &nspace,
                                            const string &key) {
  optional<vector<string>> elem;
  auto &ns = this->container[nspace];
  if (ns[key].second == value_type_info::list) {
    vector<string> item;
    if (holds_alternative<vector<string>>(ns[key].first)) {
      item = get<vector<string>>(ns[key].first);
       elem = item;
       return elem; 
    }
  }
  return nullopt;
}

bool SimpleKV::lset(const string &nspace, const string &key, size_t index,
                    const string &value) {
  
  return false;
}

bool SimpleKV::lpush(const string &nspace, const string &key,
                     const string &value) {

  return false;
}

optional<string> SimpleKV::lpop(const string &nspace, const string &key) {
  return nullopt;
}

bool SimpleKV::rpush(const string &nspace, const string &key,
                     const string &value) {
  return false;
}

optional<string> SimpleKV::rpop(const string &nspace, const string &key) {
  return nullopt;
}

optional<vector<string>> SimpleKV::lunion(const string &nspace1,
                                          const string &key1,
                                          const string &nspace2,
                                          const string &key2) {
  return nullopt;
}

optional<vector<string>> SimpleKV::linter(const string &nspace1,
                                          const string &key1,
                                          const string &nspace2,
                                          const string &key2) {
  return nullopt;
}

optional<vector<string>> SimpleKV::ldiff(const string &nspace1,
                                         const string &key1,
                                         const string &nspace2,
                                         const string &key2) {
  return nullopt;
}

} // namespace simplekv
