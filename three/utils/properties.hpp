#ifndef THREE_PROPERTIES_HPP
#define THREE_PROPERTIES_HPP

#include <unordered_map>

namespace three {

template < typename Key, typename Value >
class Properties {
public:

  typedef std::unordered_map<Key, Value> MapType;

  Properties() { }
  Properties(Properties&& other) { swap(other); }
  Properties(const Properties& other) : contents( other.contents ) { }
  Properties& operator=(Properties other) { return swap(other); }

  Properties& add(Key key, Value value) {
    //contents.insert( std::make_pair(std::move(key), std::move(value)) );
    contents.emplace( std::make_pair(std::move(key), std::move(value)) );
    return *this;
  }

  bool contains( const Key& key ) const {
    return contents.find( key ) != contents.end();
  }

  Value& operator[]( const Key& key ) { return contents[key]; }

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