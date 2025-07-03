#ifndef SIMPLEKV_HPP_
#define SIMPLEKV_HPP_

#include <optional>
#include <string>
#include <vector>

namespace simplekv {

// Enum that declares three diferent values
// this is used by teh SimpleKV::type() function
// to specify what type a current value has or if
// it exists
enum class value_type_info { none, string, list };

class SimpleKV {
 public:
  // Constructs an empty SimpleKV Object
  SimpleKV() = default;

  // default destructor serves our needs
  ~SimpleKV() = default;

  // ignore these, we will cover these later
  SimpleKV(const SimpleKV& other) = delete;
  SimpleKV(SimpleKV&& other) = delete;
  SimpleKV& operator=(const SimpleKV& other) = delete;
  SimpleKV& operator=(SimpleKV&& other) = delete;

  /////////////////////////////////////////////////////////////////////////////
  // General Operations
  /////////////////////////////////////////////////////////////////////////////

  // Gets all of the namespace names in the SimpleKV object
  //
  // Arguments: None
  //
  // Returns:
  // - a vector of strings, each string is a namespace name in this object
  std::vector<std::string> namespaces();

  // Gets all of the key names in the SimpleKV object that are under
  // the specified namespace
  //
  // Arguments:
  // - nspace: the name of the namespace we want to get all of the keys of
  //
  // Returns:
  // - a vector of strings, each string is a namespace name in this object
  std::vector<std::string> keys(const std::string& nspace);

  // Returns true iff ("iff" == "if and only if") the specified namespace
  // exists in this object
  //
  // Arguments:
  // - nspace: the name of the namespace we want to check for existence
  //
  // Returns:
  // - true iff the namespace exists in the object, false otherwise
  bool ns_exists(const std::string& nspace);

  // Returns true iff ("iff" == "if and only if") the specified key exists
  // within the specified namespace in this object
  //
  // Arguments:
  // - nspace: the name of the namespace we want to look in for the
  //           specified key.
  // - key: the name of the key we want to check for existence
  //
  // Returns:
  // - true iff the key exists in the specified namespace, false otherwise
  bool key_exists(const std::string& nspace, const std::string& key);

  // Returns an enum to indicate the type of the value associated with the
  // specified key in the specified namespace
  //
  // Arguments:
  // - nspace: the name of the namespace we want to look in for the
  //           specified key.
  // - key: the name of the key we want to check the type of
  //
  // Returns: one of 3 possible enum values
  // - none   iff the key does not exist in the specified namespace
  // - string iff the key in the specified namespace is associated with a
  //            string value
  // - list   iff the key in the specified namespace is associated with a
  //            list value
  value_type_info type(const std::string& nspace, const std::string& key);

  // Deletes the specified key from the specified namespace.
  // Also deletes the specified namespace if the key deletion
  // would make the namespace empty.
  //
  // Arguments:
  // - nspace: the name of the namespace we want to look in for the
  //           specified key to delete.
  // - key: the name of the key we want to delete
  //
  // Returns:
  // - true iff the the specified key in the specified namespace is
  //   successfully deleted.
  bool del(const std::string& nspace, const std::string& key);

  /////////////////////////////////////////////////////////////////////////////
  // String Operations
  /////////////////////////////////////////////////////////////////////////////

  // "String Get"
  //
  // Gets the string value from the specified key in the specified namespace
  //
  // Arguments:
  // - nspace: the name of the namespace we want to look in for the
  //           specified key to get.
  // - key: the name of the key whose value we want to get
  //
  // Returns:
  // - nullopt if the specified key in the specified namespace doesn't exist
  //   or if it doesn't contain a string value.
  // - the string value of the specified key
  std::optional<std::string> sget(const std::string& nspace,
                                  const std::string& key);

  // "String Set"
  //
  // Sets the specified key in the specified namespace to have the specified
  // value. Creates the specified namespace and key if it already exists.
  // Overwrites the value if it already exists, regardless of type.
  //
  // Arguments:
  // - nspace: the name of the namespace we want to look in for the
  //           specified key to set.
  // - key: the name of the key whose value we want to set
  // - value: the value we want to set the key to.
  //
  // Returns: None
  void sset(const std::string& nspace,
            const std::string& key,
            const std::string& value);

  /////////////////////////////////////////////////////////////////////////////
  // List Operations
  /////////////////////////////////////////////////////////////////////////////

  // "List Length"
  //
  // gets the length of the list with the associated key and namespace
  //
  // Non-existent values are treated as empty lists
  //
  // Arguments:
  // - nspace: the name of the namespace we want to look in for the
  //           specified key.
  // - key: the name of the key whose value we want the length of.
  //
  // returns:
  // - -1 if it is a string value
  // - the length of the list if the value exists
  ssize_t llen(const std::string& nspace, const std::string& key);

  // "List Members"
  //
  // Gets the a copy of the list stored in the associated namesapce and key
  //
  // Non-existent values are treated as empty lists
  //
  // Arguments:
  // - nspace: the name of the namespace we want to look in for the
  //           specified key.
  // - key: the name of the key whose value we want to index into.
  // - index: the index into the list we want to get the value of.
  //
  // returns:
  // - nullopt if the value is a string
  // - a copy of the value stored at the key and namespace
  std::optional<std::vector<std::string>> lmembers(const std::string& nspace,
                                                   const std::string& key);

  // "List Index"
  //
  // Gets the value at the specified index of the list.
  // The list is at the specified nspace and key.
  // The index should be >= 0 and < length of the list for
  // the operation to succeed.
  //
  // Non-existent values are treated as empty lists
  //
  // Arguments:
  // - nspace: the name of the namespace we want to look in for the
  //           specified key.
  // - key: the name of the key whose value we want to index into.
  // - index: the index into the list we want to get the value of.
  //
  // returns:
  // - nullopt if the value is a string or the index is out of bounds
  // - the value at the specified index or key does not exist
  std::optional<std::string> lindex(const std::string& nspace,
                                    const std::string& key,
                                    size_t index);

  // "List Set"
  //
  // Sets the value at the specified index of the list.
  // The list is at the specified nspace and key.
  // The index should be >= 0 and < length of the list for
  // the operation to succeed.
  //
  // Non-existent values are treated as empty lists
  //
  // Arguments:
  // - nspace: the name of the namespace we want to look in for the
  //           specified key.
  // - key: the name of the key whose value we want to modify.
  // - index: the index into the list we want to set the value of.
  //
  // returns:
  // - false if the value is a string or the index is out of bounds
  // - true otherwise
  bool lset(const std::string& nspace,
            const std::string& key,
            size_t index,
            const std::string& value);

  // "List Push"
  //
  // Pushes the specified value to the beginning of the specified
  // list. The list is specified by the nspace and the key.
  // If the list did not exist beforehand, it is created.
  //
  // Arguments:
  // - nspace: the name of the namespace we want to look in for the
  //           specified key.
  // - key: the name of the key whose value we want to push onto
  // - value: the value we want to push to the front of the list.
  //
  // returns:
  // - false if the value is a string
  // - true otherwise
  bool lpush(const std::string& nspace,
             const std::string& key,
             const std::string& value);

  // "List Pop"
  //
  // Pops a value off the front of the specified list.
  // The list is specified by the nspace and the key.
  // If the list would be emptied, it and its corresponding key is deleted.
  // If the namespace would then be empty, it is also deleted.
  //
  // Arguments:
  // - nspace: the name of the namespace we want to look in for the
  //           specified key.
  // - key: the name of the key whose value we want to pop from
  //
  // Returns:
  // - nullopt if it is a string or key does not exist
  // - the value popped of the list
  std::optional<std::string> lpop(const std::string& nspace,
                                  const std::string& key);

  // "List Right Push"
  //
  // Pushes the specified value to the end of the specified
  // list. The list is specified by the nspace and the key.
  // If the list did not exist beforehand, it is created.
  //
  // Arguments:
  // - nspace: the name of the namespace we want to look in for the
  //           specified key.
  // - key: the name of the key whose value we want to push onto
  // - value: the value we want to push to the end of the list.
  //
  // Returns:
  // - false if the value is a string
  // - true otherwise
  bool rpush(const std::string& nspace,
             const std::string& key,
             const std::string& value);

  // "List Right Pop"
  //
  // Pops a value off the back of the specified list.
  // The list is specified by the nspace and the key.
  // If the list would be emptied, it and its corresponding key is deleted.
  // If the namespace would then be empty, it is also deleted.
  //
  // Arguments:
  // - nspace: the name of the namespace we want to look in for the
  //           specified key.
  // - key: the name of the key whose value we want to pop from
  //
  // Returns:
  // - nullopt if it is a string or key does not exist
  // - the value popped of the list
  std::optional<std::string> rpop(const std::string& nspace,
                                  const std::string& key);

  // "List Union"
  //
  // Takes the union of two specified lists.
  // The first list is specified by nspace1 and key1.
  // The second list is specified by nspace2 and key2.
  // The union is the set of values that are in either lists.
  //
  // Non-existent values are treated as empty lists
  //
  // Important to note here: the values we are operating on are lists, meaning
  // that they have a strict ordering and can have duplicate values. Our return
  // value is a vector which has these properties, but the return value can
  // have the elements in any order and should not have any duplicate values.
  //
  // For example, if we union these two lists:
  // - ["hello", "hello", "hello   "]
  // - ["hello", "howdy", "bye", "bye"]
  // our result should contain the following (in any order):
  // - ["hello", "howdy", "hello   ", "bye"]
  // Note that the two strings that say "hello" in the output are different
  // since there is whitespace in it.
  //
  // Arguments:
  // - nspace1 : the namespace of the first list
  // - key1    : the key of the first list
  // - nspace2 : the namespace of the second list
  // - key2    : the key of the second list
  //
  // Returns:
  // - nullopt if either value is a string
  // - a vector containing the set union of the two lists
  std::optional<std::vector<std::string>> lunion(const std::string& nspace1,
                                                 const std::string& key1,
                                                 const std::string& nspace2,
                                                 const std::string& key2);

  // "List Intersection"
  //
  // Takes the intersection of two specified lists.
  // The first list is specified by nspace1 and key1.
  // The second list is specified by nspace2 and key2.
  // The union is the set of values that can be found in both lists.
  //
  // Non-existent values are treated as empty lists
  //
  // Important to note here: the values we are operating on are lists, meaning
  // that they have a strict ordering and can have duplicate values. Our return
  // value is a vector which has these properties, but the return value can
  // have the elements in any order and should not have any duplicate values.
  //
  // For example, if we find the intersection of these two lists:
  // - ["hello", "hello", "hello   "]
  // - ["hello", "howdy", "bye", "bye"]
  // our result should contain the following (in any order):
  // - ["hello"]
  //
  // Arguments:
  // - nspace1 : the namespace of the first list
  // - key1    : the key of the first list
  // - nspace2 : the namespace of the second list
  // - key2    : the key of the second list
  //
  // Returns:
  // - nullopt if either value is a string
  // - a vector containing the set intersection of the two lists
  std::optional<std::vector<std::string>> linter(const std::string& nspace1,
                                                 const std::string& key1,
                                                 const std::string& nspace2,
                                                 const std::string& key2);

  // "List Difference"
  //
  // Takes the difference of the two specified lists.
  // The first list is specified by nspace1 and key1.
  // The second list is specified by nspace2 and key2.
  // The union is the set of values that can be found in the first list but
  // cannot be found in the second list.
  //
  // Non-existent values are treated as empty lists
  //
  // Important to note here: the values we are operating on are lists, meaning
  // that they have a strict ordering and can have duplicate values. Our return
  // value is a vector which has these properties, but the return value can
  // have the elements in any order and should not have any uplicate values.
  //
  // For example, if we find the intersection of these two lists:
  // - ["hello", "hello", "hello   "]
  // - ["hello", "howdy", "bye", "bye"]
  // our result should contain the following (in any order):
  // - ["hello   "]
  //
  // Arguments:
  // - nspace1 : the namespace of the first list
  // - key1    : the key of the first list
  // - nspace2 : the namespace of the second list
  // - key2    : the key of the second list
  //
  // Returns:
  // - nullopt if either value is a string
  // - a vector containing the set difference of the two lists
  std::optional<std::vector<std::string>> ldiff(const std::string& nspace1,
                                                const std::string& key1,
                                                const std::string& nspace2,
                                                const std::string& key2);

 private:
  // TODO: decide what to put here!
};

}  // namespace simplekv

#endif  // SIMPLEKV_HPP_
