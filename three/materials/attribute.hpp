#ifndef THREE_ATTRIBUTE_HPP
#define THREE_ATTRIBUTE_HPP

#include <three/common.hpp>

#include <three/utils/any.hpp>

#include <unordered_map>

namespace three {

class Attribute {
public:

  explicit Attribute( THREE::AttributeType type = THREE::v3, int arraySize = 0 )
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
  //std::vector<Vector4> value;
  any value; // This will be a vector, of float/vec2/vec3/color/vec4
  std::string belongsToAttribute;
  std::string boundTo;
  Buffer buffer;
  bool createUniqueBuffers;
  bool needsUpdate;
  int size;
  int itemSize;

  bool __glInitialized;
  Attribute* __original;
};

//////////////////////////////////////////////////////////////////////////

typedef std::unordered_map<std::string, Attribute> Attributes;
typedef std::unordered_map<std::string, int> AttributeLocations;

//////////////////////////////////////////////////////////////////////////

namespace detail {

template < typename T, bool TypeIsScalar >
struct FillerImpl {
  enum { Stride = sizeof(T) / sizeof(float) };

  inline void fill( const std::vector<T>& src, std::vector<float>& dst ) {
    /*const auto srcData = (const float*)src.data();
    const auto srcDataSize = src.size() * Stride;
    std::copy( srcData, srcData + srcDataSize, dst.data() );*/
    const auto count = src.size();
    for ( size_t i = 0, offset = 0; i < count; ++i, offset += Stride ) {
      for ( int j = 0; j < Stride; ++j ) {
        dst[ offset + j ] = src[ i ][ j ];
      }
    }
  }

  template < typename SortArray >
  inline void fill( const std::vector<T>& src, const SortArray& sortArray, std::vector<float>& dst ) {
    const auto count = src.size();
    for ( size_t i = 0, offset = 0; i < count; ++i, offset += Stride ) {
      const auto& index = sortArray[ i ].second;
      for ( int j = 0; j < Stride; ++j ) {
        dst[ offset + j ] = src[ index ][ j ];
      }
    }
  }
};

template < typename T >
struct FillerImpl<T, true> {
  inline void fill( const std::vector<T>& src, std::vector<float>& dst ) {
    static_assert( std::is_same<T,float>::value, "Only float targets are supported" );
    std::copy( src.begin(), src.end(), dst.begin() );
  }

  template < typename SortArray >
  inline void fill( const std::vector<T>& src, const SortArray& sortArray, std::vector<float>& dst ) {
    static_assert( std::is_same<T,float>::value, "Only float targets are supported" );
    const auto count = src.size();
    for ( size_t i = 0; i < count; i++ ) {
      const auto& index = sortArray[ i ].second;
      dst[ i ] = src[ index ];
    }
  }
};

template < typename T >
struct Filler : public FillerImpl<T, sizeof(T) <= sizeof(float)> { };

} // namespace detail

//////////////////////////////////////////////////////////////////////////

template < typename T >
void fill( const any& src, std::vector<float>& dst ) {
  detail::Filler< T > filler;
  filler.fill( src.cast<std::vector<T>>(), dst );
}

template < typename T, typename SortArray >
void fill( const any& src, const SortArray& sortArray, std::vector<float>& dst ) {
  detail::Filler< T > filler;
  filler.fill( src.cast<std::vector<T>>(), sortArray, dst );
}


} // namespace three

#endif // THREE_ATTRIBUTE_H