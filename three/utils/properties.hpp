#ifndef THREE_PROPERTIES_HPP
#define THREE_PROPERTIES_HPP

#include <unordered_map>

namespace three {

template < typename Key, typename Value >
class Properties {
public:

  Properties() { }

  Properties& add(Key key, Value value) {
    contents.insert( std::make_pair(std::move(key), std::move(value)) );
    return *this;
  }

  bool contains( const Key& key ) const {
    return contents.find( key ) != contents.end();
  }

  std::unordered_map<Key, Value> contents;

};

} // namespace three

#endif // THREE_PROPERTIES_HPP