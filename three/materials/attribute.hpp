#ifndef THREE_ATTRIBUTE_HPP
#define THREE_ATTRIBUTE_HPP

#include <three/common.hpp>

#include <unordered_map>

namespace three {

typedef int Attribute;

typedef std::unordered_map<std::string, Attribute> Attributes;

inline bool contains( const Attributes& c, const std::string& elem ) {
    return c.find( elem ) != c.end();
}

} // namespace three

#endif // THREE_ATTRIBUTE_H