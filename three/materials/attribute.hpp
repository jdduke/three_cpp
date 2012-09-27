#ifndef THREE_ATTRIBUTE_HPP
#define THREE_ATTRIBUTE_HPP

#include <three/common.hpp>

#include <unordered_map>

namespace three {

typedef int Attribute;

typedef std::unordered_map<std::string, Attribute> Attributes;

} // namespace three

#endif // THREE_ATTRIBUTE_H