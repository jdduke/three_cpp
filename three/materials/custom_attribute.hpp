#ifndef THREE_CUSTOM_ATTRIBUTE_HPP
#define THREE_CUSTOM_ATTRIBUTE_HPP

#include <three/common.hpp>

#include <unordered_map>

namespace three {

class CustomAttribute {
public:

  explicit CustomAttribute( THREE::AttributeType type = THREE::V3, int arraySize = 0 )
    : type( type ),
      buffer( 0 ),
      createUniqueBuffers( false ),
      needsUpdate( false ),
      size( 0 ),
      itemSize( 1 ),
      __glInitialized( false ),
      __original( nullptr ) {

    if ( arraySize > 0 )
      array.resize( arraySize );

  }

  /////////////////////////////////////////////////////////////////////////

  THREE::AttributeType type;

  std::vector<float> array;
  std::vector<Vector4> value;
  std::string belongsToAttribute;
  std::string boundTo;
  Buffer buffer;
  bool createUniqueBuffers;
  bool needsUpdate;
  int size;
  int itemSize;

  bool __glInitialized;
  CustomAttribute* __original;
};

typedef std::unordered_map<std::string, CustomAttribute> CustomAttributes;

} // namespace three

#endif // THREE_CUSTOM_ATTRIBUTE_HPP