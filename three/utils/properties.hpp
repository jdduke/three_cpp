#ifndef THREE_PROPERTIES_HPP
#define THREE_PROPERTIES_HPP

#include <three/config.hpp>

#include <unordered_map>
//#include <map>

namespace three {

template < typename Key, typename Value >
class Properties {
public:

  typedef std::unordered_map<Key, Value> MapType;
  //typedef std::map<Key, Value> MapType;

  Properties() { }
  Properties(Properties&& other) { swap(other); }
  Properties(const Properties& other) : contents( other.contents ) { }
  Properties& operator=(Properties other) { return swap(other); }

  Properties& add(Key key, Value value) {
    contents.emplace( std::make_pair(std::move(key), std::move(value)) );
    return *this;
  }

#if THREE_HAS_VARIADIC_TEMPLATES
  template < typename... T >
  Properties& emplace( Key&& key, T&&... ts ) {
    return add( std::move(key), Value( std::move(ts)...) );
  }
#else
  template < typename T, typename U >
  Properties& emplace( Key&& key, T&& t, U&& u ) {
    return add( std::move(key), Value( std::move(t), std::move(u) ) );
  }
  template < typename T, typename U, typename V >
  Properties& emplace( Key&& key, T&& t, U&& u, V&& v ) {
    return add( std::move(key), Value( std::move(t), std::move(u), std::move(v) ) );
  }
  template < typename T, typename U, typename V, typename W >
  Properties& emplace( Key&& key, T&& t, U&& u, V&& v, W&& w ) {
    return add( std::move(key), Value( std::move(t), std::move(u), std::move(v), std::move(w) ) );
  }
#endif // THREE_HAS_VARIADIC_TEMPLATES

  THREE_EXPLICIT operator bool() const { return contents.size() > 0; }

  inline bool contains( const Key& key ) const {
    return contents.find( key ) != contents.end();
  }

  template < typename T >
  inline bool contains( const T& key ) const {
    return contains( Key(key) );
  }

  Value& operator[]( const char* key ) { return contents[ Key(key) ]; }
  Value& operator[]( const Key& key ) { return contents[key]; }

  const Value* get( const Key& key ) const {
    auto it = contents.find( key );
    return it == contents.end() ? nullptr : &it->second;
  }

  Value* get( const Key& key ) {
    auto it = contents.find( key );
    return it == contents.end() ? nullptr : &it->second;
  }

  typename MapType::iterator       find( const Key& key ) { return contents.find( key ); }
  typename MapType::const_iterator find( const Key& key ) const { return contents.find( key ); }
  typename MapType::iterator       begin()       { return contents.begin(); }
  typename MapType::const_iterator begin() const { return contents.cbegin(); }
  typename MapType::iterator       end()         { return contents.end(); }
  typename MapType::const_iterator end() const   { return contents.cend(); }

  //////////////////////////////////////////////////////////////////////////

  MapType contents;

private:

  Properties& swap( Properties& other ) {
    std::swap( contents, other.contents );
    return *this;
  }

};

} // namespace three

#endif // THREE_PROPERTIES_HPP