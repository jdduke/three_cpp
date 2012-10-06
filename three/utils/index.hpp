#ifndef THREE_INDEX_HPP
#define THREE_INDEX_HPP

namespace three {

template < typename T, T invalid >
struct IndexT {
  explicit IndexT( T t = invalid ) : value( t ) { }
  IndexT( const IndexT& other ) : value( other.value ) { }

  IndexT& operator=( T t ) { value = t; return *this; }
  IndexT& operator=( const IndexT& other ) { value = other.value; return *this; }

  //THREE_EXPLICIT operator bool() const { return value != invalid; }
  bool valid() const { return value != invalid; }

  operator T() const { return value; }
  //operator int() const { return (int)value; }
  operator unsigned() const { return (unsigned)value; }
  //operator size_t() const { return (size_t)value; }

  T value;
};

typedef IndexT<int, -1> Index;

} // namespace three

#endif // THREE_PROPERTIES_HPP