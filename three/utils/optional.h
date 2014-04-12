#ifndef THREE_OPTIONAL_H
#define THREE_OPTIONAL_H

#include <type_traits>
#include <new>

namespace three {

// Simple optional container, credit to simonask from https://github.com/simonask/reflect/blob/master/base/maybe.hpp.
template <typename T>
class optional {
public:
  optional();
  optional(std::nullptr_t);
  optional(const optional<T>& other);
  optional(optional<T>&& other);
  optional(const T& other);
  optional(T&& other);
  ~optional() { clear(); }
  optional<T>& operator=(const optional<T>& other);
  optional<T>& operator=(optional<T>&& other);
  optional<T>& operator=(const T& other);
  optional<T>& operator=(T&& other);

  T* operator->() { return get(); }
  const T* operator->() const { return get(); }
  T& operator*() { return *get(); }
  const T& operator*() const { return *get(); }

  THREE_EXPLICIT operator bool() const { return is_set(); }

  const T& value() const { return *get(); }
  T& value() { return *get(); }

private:
  const T* get() const { return is_set() ? memory() : nullptr; }
  T* get() { return is_set() ? memory() : nullptr; }

  enum {
    kStorageSize = sizeof(T) + 1, // T + is_set byte
    kAlignment = std::alignment_of<T>::value,
  };
  typedef typename std::aligned_storage<kStorageSize, kAlignment>::type StorageType;

  StorageType memory_;

  const T* memory() const { return reinterpret_cast<const T*>(&memory_); }
  T* memory() { return reinterpret_cast<T*>(&memory_); }
  const char* is_set_ptr() const { return reinterpret_cast<const char*>(&memory_) + sizeof(T); }
  char* is_set_ptr() { return reinterpret_cast<char*>(&memory_) + sizeof(T); }

  void clear() {
    if (is_set()) {
      memory()->~T();
      set(false);
    }
  }

  bool is_set() const { return *is_set_ptr() != 0; }

  void set(bool b) {
    if (b) *is_set_ptr() = 1;
    else   *is_set_ptr() = 0;
  }

  void assign(const T& by_copy) {
    if (is_set()) {
      clear();
    }
    ::new(memory()) T(by_copy);
    set(true);
  }

  void assign(T&& by_move) {
    if (is_set()) {
      clear();
    }
    ::new(memory()) T(std::move(by_move));
    set(true);
  }
};

template <typename T>
optional<T>::optional() {
  set(false);
}

template <typename T>
optional<T>::optional(std::nullptr_t) {
  set(false);
}

template <typename T>
optional<T>::optional(const optional<T>& other) {
  set(false);
  if (other) assign(*other);
}

template <typename T>
optional<T>::optional(optional<T>&& other) {
  set(false);
  if (other) assign(std::move(*other));
  other.clear();
}

template <typename T>
optional<T>::optional(const T& other) {
  set(false);
  assign(other);
}

template <typename T>
optional<T>::optional(T&& other) {
  set(false);
  assign(std::move(other));
}

template <typename T>
optional<T>& optional<T>::operator=(const optional<T>& other) {
  if (other) assign(*other);
  else clear();
  return *this;
}

template <typename T>
optional<T>& optional<T>::operator=(optional<T>&& other) {
  if (other) { assign(std::move(*other)); other.clear(); }
  else clear();
  return *this;
}

template <typename T>
optional<T>& optional<T>::operator=(const T& other) {
  assign(other);
  return *this;
}

template <typename T>
optional<T>& optional<T>::operator=(T&& other) {
  assign(std::move(other));
  return *this;
}

template <typename T>
optional<T> make_optional(T&& type) {
  return optional<T>(std::move(type));
}

template <typename T>
optional<T> make_optional(const T& type) {
  return optional<T>(type);
}

} // namespace three

#endif // THREE_OPTIONAL_H