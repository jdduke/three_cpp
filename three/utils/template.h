#ifndef THREE_TEMPLATE_UTILS_H
#define THREE_TEMPLATE_UTILS_H

#include <algorithm>
#include <functional>
#include <map>
#include <unordered_map>

namespace three {

template < typename C >
inline C& concat( C& a, const C& b ) {
  a.insert( a.end(), b.begin(), b.end() );
  return a;
}

template < typename C >
inline void grow( C& c, size_t size, float factor = 1.5f ) {
  if ( c.size() < size )
    c.resize( ( int )( factor * size ) );
}

template < typename C, typename T >
inline bool contains( const C& c, const T& elem ) {
  return std::find( c.begin(), c.end(), elem ) != c.end();
}

template < typename T >
inline bool contains( const std::unordered_map<std::string, T>& c, const std::string& elem ) {
  return c.find( elem ) != c.end();
}

template < typename T >
inline bool contains( const std::unordered_map<std::string, T>& c, const char* elem ) {
  return contains ( c, std::string( elem ) );
}

template < typename C, typename T >
inline bool erase( C& c, const T& elem ) {
  auto i = std::find( c.begin(), c.end(), elem );
  if ( i != c.end() ) {
    c.erase( i );
    return true;
  }
  return false;
}

template < typename C, typename T >
inline bool push_unique( C& c, T && elem ) {
  if ( !contains( c, elem ) ) {
    c.push_back( std::move( elem ) );
    return true;
  }
  return false;
}

/////////////////////////////////////////////////////////////////////////

struct Deferred {
  typedef std::function<void(void)> DeferredFunc;
  explicit Deferred( DeferredFunc&& f ) : f ( std::move(f) ) { }
  Deferred( Deferred&& other ) {
    std::swap( f, other.f );
  }
private:
  DeferredFunc f;
  Deferred();
  Deferred( const Deferred& );
  Deferred& operator=( const Deferred& );
  Deferred& operator=( Deferred&& other );
};

template < typename F >
Deferred defer( F&& f ) {
  return Deferred( std::move(f) );
}

} // namespace three

#endif // THREE_TEMPLATE_UTILS_H