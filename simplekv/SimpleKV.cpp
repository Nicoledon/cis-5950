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
// can have any keys inside this object
vector<string> SimpleKV::keys(const string &nspace) {
  vector<string> res{};
  auto items = container[nspace];
  for (auto const &item : items) {
    res.push_back(item.first);
  }
  return res;
}

bool SimpleKV::ns_exists(const string &nspace) {
  if (this->container.contains(nspace)) {
    return true;
  }
  return false;
}

bool SimpleKV::key_exists(const string &nspace, const string &key) {
  if (!ns_exists(nspace)) {
    return false;
  }
  auto item = this->container[nspace];
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
  if (!ns_exists(nspace) || !key_exists(nspace, key)) {
    return false;
  } else {
    auto &ns = this->container[nspace];
    ns.erase(key);
    if (ns.empty()) {
      this->container.erase(nspace);
    }
  }
  return true;
}

// string operations
string getstring(variant<string, vector<string>> elem) {
  string item;
  if (holds_alternative<string>(elem)) {
    item = get<string>(elem);
  }
  return item;
}
vector<string> getvec(variant<string, vector<string>> elem) {
  vector<string> item;
  if (holds_alternative<vector<string>>(elem)) {
    item = get<vector<string>>(elem);
  }
  return item;
}
optional<string> SimpleKV::sget(const string &nspace, const string &key) {
  if (!ns_exists(nspace) || !key_exists(nspace, key) ||
      this->container[nspace][key].second == value_type_info::list) {
    return nullopt;
  } else {
    optional<string> item = getstring(this->container[nspace][key].first);
    return item;
  }
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
  if (!ns_exists(nspace) || !key_exists(nspace, key)) {
    return 0;
  } else if (type(nspace, key) == value_type_info::string) {
    return -1;
  } else {
    auto vec = getvec(this->container[nspace][key].first);
    return vec.size();
  }
}

optional<string> SimpleKV::lindex(const string &nspace, const string &key,
                                  size_t index) {
  if(!ns_exists(nspace) || !key_exists(nspace , key)) {
    return key;
  } else if(this->container[nspace][key].second == value_type_info::string) {
    return nullopt;
  } else {
    auto vec =  getvec(this->container[nspace][key].first);
    if(index >= vec.size()) return nullopt;
    return vec[index];
  }
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
  if (ns_exists(nspace) && key_exists(nspace, key)) {
    auto vec =  getvec(this->container[nspace][key].first);
    if (index >= vec.size() ||
      this->container[nspace][key].second == value_type_info::string) {
      return false;
    } else {
      vec[index] = value;
    }
  } else {
    vector<string> vec = {value};
    variant<string,vector<string>> item = vec;
    this->container[nspace][key].first = item; 
  }
  return true;
}

bool SimpleKV::lpush(const string &nspace, const string &key,
                     const string &value) {
  return true;
}

optional<string> SimpleKV::lpop(const string &nspace, const string &key) {
   return nullopt; 
}

bool SimpleKV::rpush(const string &nspace, const string &key,
                     const string &value) {
  return true; 
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
