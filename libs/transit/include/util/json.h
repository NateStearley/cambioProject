#ifndef UTIL_JSON_H_
#define UTIL_JSON_H_

#include <functional>
#include <iostream>
#include <initializer_list>
#include <string>
#include <utility>

#include "WebServer.h"
#include "picojson.h"

class JsonObject;
class JsonArray;

/**
 * @class Manages a picojson::value, provides implicit casting for valid JSON types.
 */
class JsonValue {
 public:

  /**
   * @brief Default constructor
   */
  JsonValue() : v() {};

  /**
   * @brief Create a JsonValue from an existing picojson::value (creates a copy)
   * @param val: picojson:value to copy
   */
  explicit JsonValue(picojson::value val) : v(std::move(val)) {}

  /**
   * @brief Create a JsonValue from a double
   * @param d: a double
   */
  explicit JsonValue(double d) : v(d) {}

  /**
   * @brief Create a JsonValue from an int
   * @param i: an int
   */
  explicit JsonValue(int i) : v(static_cast<double>(i)) {}

  /**
   * @brief Create a JsonValue from a string
   * @param s: a string
   */
  explicit JsonValue(const std::string &s) : v(s) {}

  /**
   * @brief Create a JsonValue from a string literal
   * @param s: a string literal
   */
  explicit JsonValue(const char *s) : v(s) {}

  /**
   * @brief Create a JsonValue from a bool
   * @param b: a bool
   */
  explicit JsonValue(bool b) : v(b) {}

  /**
   * @brief Create a JsonValue from a JsonObject
   * @param o: a JsonObject
   */
  explicit JsonValue(const JsonObject &o);

  /**
   * @brief Create a JsonValue from a JsonArray
   * @param a: a JsonArray
   */
  explicit JsonValue(const JsonArray &a);

  /**
   * @brief Destructor
   */
  ~JsonValue();

  /**
   * @brief Copy constructor
   * @param other: another JsonValue
   */
  JsonValue(const JsonValue &other);

  /**
   * @brief Copy operator
   * @param other: another JsonValue
   * @return this
   */
  JsonValue &operator=(const JsonValue &other);

  /**
   * @brief Move constructor
   * @param other: another JsonValue
   */
  JsonValue(JsonValue &&other) noexcept;

  /**
   * @brief Move operator
   * @param other: another JsonValue
   * @return this
   */
  JsonValue &operator=(JsonValue &&other) noexcept;

  /**
   * @brief Create a a JsonValue using a reference to an existing picojson::value.
   * Instead of creating a copy of the picojson::value, this just stores a reference to it.
   * This is only needed for internal use.
   * @param v: a picojson::value to create a reference to
   * @return the new JsonValue
   */
  static JsonValue FromReference(picojson::value &v);

  /**
   * @return The underlying picojson::value
   */
  picojson::value &GetValue() { return isRef ? ref.get() : v; }

  /**
   * @return The underlying picojson::value
   */
  [[nodiscard]] const picojson::value &GetValue() const {
    return isRef ? ref.get() : v;
  }

  // DOUBLE

  /**
   * @return Converts this to a double
   */
  explicit operator double() const { return Get<double>(); }

  /**
   * @brief Set this value to a double
   * @return this
   */
  JsonValue &operator=(double d) {
    Set(d);
    return *this;
  }

  // FLOAT

  /**
   * @return Converts this to a float
   */
  explicit operator float() const { return static_cast<float>(Get<double>()); }

  /**
   * @brief Set this value to a float
   * @return this
   */
  JsonValue &operator=(float f) {
    Set(static_cast<double>(f));
    return *this;
  }

  // INT

  /**
   * @return Converts this to an int
   */
  explicit operator int() const { return static_cast<int>(Get<double>()); }

  /**
   * @brief Set this value to an int
   * @return this
   */
  JsonValue &operator=(int i) {
    Set(static_cast<double>(i));
    return *this;
  }

  // STRING

  /**
   * @return Converts this to a string
   */
  explicit operator std::string() const { return Get<std::string>(); }

  /**
   * @brief Set this value to a string
   * @return this
   */
  JsonValue &operator=(const std::string &s) {
    Set(s);
    return *this;
  }

  /**
   * @brief Set this value to a string
   * @return this
   */
  JsonValue &operator=(const char *s) {
    Set(s);
    return *this;
  }

  // BOOL

  /**
   * @return Converts this to a bool
   */
  explicit operator bool() const { return Get<bool>(); }

  /**
   * @brief Set this value to a bool
   * @return this
   */
  JsonValue &operator=(bool b) {
    Set(b);
    return *this;
  }

  // JSON OBJECT

  /**
   * @return Converts this to a JsonObject
   */
  explicit operator JsonObject() const;

  /**
   * @brief Set this value to a JsonObject
   * @return this
   */
  JsonValue &operator=(const JsonObject &o);

  // JSON ARRAY

  /**
   * @return Converts this to a JsonArray
   */
  explicit operator JsonArray() const;

  /**
   * @brief Set this value to a JsonArray
   * @return this
   */
  JsonValue &operator=(const JsonArray &a);

  /**
   * @brief Serialize this to valid JSON text.
   * @return This as a serialized JSON-formatted string.
   */
  [[nodiscard]] std::string ToString() const { return GetValue().serialize(); }

 protected:
  template<class T>
  [[nodiscard]] T Get() const { return GetValue().get<T>(); }

  template<class T>
  void Set(const T &t) { GetValue() = picojson::value(t); }

  union {
    picojson::value v;
    std::reference_wrapper<picojson::value> ref;
  };

  bool isRef = false;
};

/**
 * @class Manages a picojson::object, works with JsonValue to provide implicit casting
 */
class JsonObject {

 public:

  /**
   * @brief Default constructor
   */
  JsonObject() = default;

  /**
   * @brief Copy a picojson::object into this
   * @param obj: a picojson::object
   */
  explicit JsonObject(picojson::object obj) : obj(std::move(obj)) {}

  /**
   * @brief Read/write access to a value in this JSON object from a given key.
   * You can add to the JsonObject this way.
   *
   * @param key: the key
   * @return read/write access to a JsonValue
   *
   * @example
   * @code
   * obj["my_number"] = 9.0;
   * obj["my_string"] = "hello";
   * obj["my_array"] = JsonArray();
   */
  JsonValue operator[](const std::string &key) {
    return JsonValue::FromReference(obj[key]);
  }

  /**
   * @brief Read only access to a value in this JSON object from a given key.
   * Requires existence of provided key. The returned value IS A COPY.
   * @throws std::runtime_error: if the key does not exist already
   * @param key: the key
   * @return a COPY of the value at the provided key
   *
   * @example
   * @code
   * double num = obj["my_number"];
   * std::string s = obj["my_string"];
   */
  JsonValue operator[](const std::string &key) const {
    return (JsonValue) obj.at(key);
  }

  /**
   * @return The underlying picojson::object
   */
  picojson::object &GetObject() { return obj; }

  /**
   * @return The underlying picojson::object
   */
  [[nodiscard]] const picojson::object &GetObject() const { return obj; }

  /**
   * @brief Serialize this to valid JSON text.
   * @return This as a serialized JSON-formatted string.
   */
  [[nodiscard]] std::string ToString() const { return picojson::value(obj).serialize(); }

  /**
   * @brief Check if this JSON object contains a given key.
   * @param key: the key
   * @return whether or not key exists in this JsonObject
   */
  [[nodiscard]] bool Contains(const std::string &key) const {
    return obj.find(key) != obj.end();
  }

  /**
   * @return A vector of all the keys in this JsonObject
   */
  [[nodiscard]] std::vector<std::string> GetKeys() const;

 protected:
  picojson::object obj;
};

/**
 * @class Manages an underlying picojson::array. Works with JsonValue for implicit casting.
 */
class JsonArray {

 public:

  /**
   * @brief Default constructor.
   */
  JsonArray() = default;

  /**
   * @brief Construct using a copy of a provided picojson::array
   * @param arr: a picojson::array
   */
  explicit JsonArray(picojson::array arr) : arr(std::move(arr)) {}

  /**
   * @brief Construct using an initializer list
   * @param ls: the initializer list
   *
   * @example
   * @code
   * JsonArray arr = {9.0, "Hello", 1, JsonArray()};
   */
  JsonArray(const std::initializer_list<JsonValue> ls) {
    for (const auto &val : ls)Push(val);
  }

  /**
   * @brief Initialize a JsonArray to a given size.
   * @param size: the size of the array
   */
  explicit JsonArray(int size) : arr(size) {}

  /**
   * @return The underlying picojson::array
   */
  picojson::array &GetArray() { return arr; }

  /**
   * @return The underlying picojson::array
   */
  [[nodiscard]] const picojson::array &GetArray() const { return arr; }

  /**
   * @brief Serialize this to valid JSON text.
   * @return This as a serialized JSON-formatted string.
   */
  [[nodiscard]] std::string ToString() const { return picojson::value(arr).serialize(); }

  /**
   * @brief Read-write access to the entry at the given index.
   * You can modify an existing entry using this.
   * @param idx: the index
   * @return read/write access to a JsonValue
   *
   * @example
   * @code
   * JsonArray arr(3);
   * arr[0] = 2;
   * arr[1] = "Apple";
   * arr[2] = JsonObject();
   */
  JsonValue operator[](int idx) { return JsonValue::FromReference(arr[idx]); }

  /**
   * @brief Read-only access to the entry at the given index.
   * This entry must be within the bounds of the array.
   * This returns a COPY of the entry.
   * @throws std::runtime_error: if the index is not within the bounds of the array
   * @param idx: the index
   * @return a COPY of the entry at the given index.
   *
   * @example
   * @code
   * double num = arr[0];
   * std::string str = arr[1];
   * JsonObject obj = arr[2];
   */
  JsonValue operator[](int idx) const { return (JsonValue) arr.at(idx); }

  /**
   * @brief Refer to operator[] const
   * @throws std::runtime_error: if the index is not within the bounds of the array
   * @param idx: the index
   * @return a COPY of the entry at the given index
   */
  [[nodiscard]] JsonValue At(int idx) const { return (JsonValue) arr.at(idx); }

  /**
   * @brief Push a JsonValue to the array (like std::vector::push_back())
   * @param val: the value to push
   *
   * @example
   * @code
   * arr.push_back(5.4);
   * arr.push_back("Car");
   */
  void Push(const JsonValue &val) { arr.push_back(val.GetValue()); }

  /**
   * @return The size of the array
   */
  [[nodiscard]] unsigned long Size() const { return arr.size(); }

  /**
   * @brief Resize the array to the given size.
   * @param size: the new size
   */
  void Resize(int size) { arr.resize(size); }

 protected:
  picojson::array arr;
};

class JsonSession : public JSONSession {
 public:

  /**
   * @brief Initializes the server
   */
  JsonSession() = default;

  ~JsonSession() override = default;

  /**
   * @brief Receive a command from the web server
   * @param val: the command (in JSON format)
   */
  void receiveJSON(picojson::value &val) override {
    JsonObject data = (JsonObject) JsonValue(val);

    std::string cmd = (std::string) data["command"];

    JsonObject returnValue;
    returnValue["id"] = data["id"];

    ReceiveCommand(cmd, data, returnValue);
    JsonValue retVal(returnValue);
    sendJSON(retVal.GetValue());
  }

  /**
   * @brief Handles specific commands from the web server
   * @param cmd: command
   * @param data: data (JSON)
   * @param returnValue: return value (JSON) which will be sent back to the server
   */
  virtual void ReceiveCommand(const std::string &cmd,
                              JsonObject &data,
                              JsonObject &returnValue) = 0;
};

inline JsonValue::~JsonValue() {
  if (!isRef) {
    v.~value();
  }
}

inline JsonValue::JsonValue(const JsonValue &other) {
  isRef = other.isRef;
  if (isRef) {
    ref = other.ref;
  } else {
    v = other.v;
  }
}

inline JsonValue &JsonValue::operator=(const JsonValue &other) {
  if (this != &other) {
    isRef = other.isRef;
    if (isRef) {
      ref = other.ref;
    } else {
      v = other.v;
    }
  }
  return *this;
}

inline JsonValue::JsonValue(JsonValue &&other) noexcept {
  isRef = other.isRef;
  if (isRef) {
    ref = other.ref;
  } else {
    v = std::move(other.v);
  }
}

inline JsonValue &JsonValue::operator=(JsonValue &&other) noexcept {
  isRef = other.isRef;
  if (isRef) {
    ref = other.ref;
  } else {
    v = std::move(other.v);
  }
  return *this;
}

inline JsonValue JsonValue::FromReference(picojson::value &v) {
  JsonValue val;
  val.ref = v;
  val.isRef = true;
  return val;
}

inline JsonValue &JsonValue::operator=(const JsonObject &o) {
  Set(o.GetObject());
  return *this;
}

inline JsonValue::operator JsonObject() const {
  return (JsonObject) {Get<picojson::object>()};
}

inline JsonValue &JsonValue::operator=(const JsonArray &a) {
  Set(a.GetArray());
  return *this;
}

inline JsonValue::operator JsonArray() const {
  return (JsonArray) {Get<picojson::array>()};
}

inline JsonValue::JsonValue(const JsonObject &o) : v(o.GetObject()) {}

inline JsonValue::JsonValue(const JsonArray &a) : v(a.GetArray()) {}

inline std::vector<std::string> JsonObject::GetKeys() const {
  std::vector<std::string> keys;
  for (const auto &kv : obj) {
    keys.push_back(kv.first);
  }
  return keys;
}

/**
 * @brief Operator overload for writing to a stream.
 */
inline std::ostream &operator<<(std::ostream &os, const JsonValue &val) {
  os << val.ToString();
  return os;
}

/**
 * @brief Operator overload for writing to a stream.
 */
inline std::ostream &operator<<(std::ostream &os, const JsonObject &obj) {
  os << obj.ToString();
  return os;
}

/**
 * @brief Operator overload for writing to a stream.
 */
inline std::ostream &operator<<(std::ostream &os, const JsonArray &array) {
  os << array.ToString();
  return os;
}

#endif //UTIL_JSON_H_
