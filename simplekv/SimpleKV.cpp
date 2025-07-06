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
  if (!ns_exists(nspace)) {
    return res;
  }
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

  if (key_exists(nspace, key)) {
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
string getstring(variant<string, vector<string>> &elem) {
  string item;
  if (holds_alternative<string>(elem)) {
    item = get<string>(elem);
  }
  return item;
}
vector<string> &getvec(variant<string, vector<string>> &elem) {
  return get<vector<string>>(elem);
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
  if (!ns_exists(nspace) || !key_exists(nspace, key)) {
    return nullopt;
  } else if (this->container[nspace][key].second == value_type_info::string) {
    return nullopt;
  } else {
    auto vec = getvec(this->container[nspace][key].first);
    if (index >= vec.size())
      return nullopt;
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
    if (this->container[nspace][key].second == value_type_info::string) {
      return false;
    } else {
      auto &vec = getvec(this->container[nspace][key].first);
      if (index >= vec.size() || index < 0)
        return false;
      vec[index] = value;
    }
  } else {
    return false;
  }
  return true;
}
bool SimpleKV::lpush(const string &nspace, const string &key,
                     const string &value) {

  if (ns_exists(nspace) && key_exists(nspace, key)) {
    if (this->container[nspace][key].second != value_type_info::list) {
      return false;
    }
    auto &item = getvec(this->container[nspace][key].first);
    item.insert(item.begin(), value);
    return true;
  } else {
    vector<string> vec = {value};
    variant<string, vector<string>> item = vec;
    this->container[nspace][key].first = item;
    this->container[nspace][key].second = value_type_info::list;
    return true;
  }
}

optional<string> SimpleKV::lpop(const string &nspace, const string &key) {
  if (!ns_exists(nspace) || !key_exists(nspace, key) ||
      this->container[nspace][key].second != value_type_info::list) {
    return nullopt;
  } else {
    auto &vec = getvec(this->container[nspace][key].first);
    auto it = vec[0];
    vec.erase(vec.begin());
    if (vec.empty()) {
      this->container[nspace].erase(key);
    }
    if (this->container[nspace].empty()) {
      this->container.erase(nspace);
    }
    return it;
  }
}

bool SimpleKV::rpush(const string &nspace, const string &key,
                     const string &value) {

  if (ns_exists(nspace) && key_exists(nspace, key)) {
    if (this->container[nspace][key].second != value_type_info::list) {
      return false;
    }
    auto &item = getvec(this->container[nspace][key].first);
    item.push_back(value);
    return true;
  } else {
    vector<string> vec = {value};
    variant<string, vector<string>> item = vec;
    this->container[nspace][key].first = item;
    this->container[nspace][key].second = value_type_info::list;
    return true;
  }
}

optional<string> SimpleKV::rpop(const string &nspace, const string &key) {
  if (!ns_exists(nspace) || !key_exists(nspace, key) ||
      this->container[nspace][key].second != value_type_info::list) {
    return nullopt;
  } else {
    auto &vec = getvec(this->container[nspace][key].first);
    auto it = vec[vec.size() - 1];
    vec.pop_back();
    if (vec.empty()) {
      this->container[nspace].erase(key);
    }
    if (this->container[nspace].empty()) {
      this->container.erase(nspace);
    }
    return it;
  }
}
vector<string> set2vec(unordered_set<string> elem) {
  vector<string> item;
  for (auto i : elem) {
    item.push_back(i);
  }
  return item;
}
optional<vector<string>> SimpleKV::lunion(const string &nspace1,
                                          const string &key1,
                                          const string &nspace2,
                                          const string &key2) {
  unordered_set<string> items;
  if (!key_exists(nspace1, key1) && !key_exists(nspace2, key2)) {
    return nullopt;
  } else if (!key_exists(nspace1, key1) &&
             this->container[nspace2][key2].second == value_type_info::list) {
    return getvec(this->container[nspace2][key2].first);
  } else if (!key_exists(nspace2, key2) &&
             this->container[nspace1][key1].second == value_type_info::list) {
    return getvec(this->container[nspace1][key1].first);
  } else if (this->container[nspace1][key1].second != value_type_info::list ||
             this->container[nspace2][key2].second != value_type_info::list) {
    return nullopt;
  } else {
    auto v1 = getvec(this->container[nspace1][key1].first);
    auto v2 = getvec(this->container[nspace2][key2].first);
    for (auto i : v1) {
      items.insert(i);
    }
    for (auto j : v2) {
      items.insert(j);
    }
    return set2vec(items);
  }
}

optional<vector<string>> SimpleKV::linter(const string &nspace1,
                                          const string &key1,
                                          const string &nspace2,
                                          const string &key2) {
  if (this->container[nspace1][key1].second != value_type_info::list ||
      this->container[nspace2][key2].second != value_type_info::list) {
    return nullopt;
  } else if (!key_exists(nspace1, key1) || !key_exists(nspace2, key2)) {
    return {};
  } else {
    auto v1 = getvec(this->container[nspace1][key1].first);
    auto v2 = getvec(this->container[nspace2][key2].first);
    unordered_set<string> items;
    for (auto i = 0; i < v1.size(); i++) {
      for (auto j = 0; j < v2.size(); j++) {
        if (v1[i] == v2[j]) {
          items.insert(v1[i]);
        }
      }
    }
    return set2vec(items);
  }
}

optional<vector<string>> SimpleKV::ldiff(const string &nspace1,
                                         const string &key1,
                                         const string &nspace2,
                                         const string &key2) {

  if (this->container[nspace1][key1].second != value_type_info::list ||
      this->container[nspace2][key2].second != value_type_info::list) {
    return nullopt;
  } else if (!key_exists(nspace1, key1) || !key_exists(nspace2, key2)) {
    return {};
  } else {
     auto t1 = getvec(this->container[nspace1][key1].first);
     auto t2 = getvec(this->container[nspace2][key2].first);
     unordered_set<string>item;
     unordered_set<string>e;
    for(auto i = 0 ; i < t1.size() ; i ++) {
        for(auto j = 0 ; j < t2.size() ;j ++){
            if(t1[i] == t2[j]){
                item.insert(t1[i]);
            }
        }
    }
    for(auto i : t1){
        if(!item.contains(i)){
           e.insert(i);
        }
    }
    return set2vec(e);
  }
} // namespace simplekv
} // namespace simplekv
