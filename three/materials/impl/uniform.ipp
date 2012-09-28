#ifndef THREE_UNIFORM_IPP
#define THREE_UNIFORM_IPP

#include <three/materials/uniform.hpp>

#include <three/core/color.hpp>
#include <three/core/vector2.hpp>
#include <three/core/vector3.hpp>
#include <three/core/matrix4.hpp>

#include <three/utils/any.hpp>

#include <three/gl.hpp>

namespace three {

Uniform::Uniform( )
  : type( INVALID ), value( ), texture( nullptr ) { }

Uniform::Uniform( Type type, any value /*= any()*/, Texture* texture /*= nullptr*/ )
  : type( type ), value( std::move( value ) ), texture( texture ) { }

Uniform& Uniform::operator=( const Uniform& other ) {
  type = other.type;
  value = other.value;
  texture = other.texture;
  return *this;
}

/////////////////////////////////////////////////////////////////////////

template < typename T, typename F >
void load1( const F& f, int location, const any& value ) {
  const auto& t = value.cast<T>();
  GL_CALL( f( location, t ) );
}
template < typename T, typename F >
void load2( const F& f, int location, const any& value ) {
  const auto& t = value.cast<T>();
  GL_CALL( f( location, t[ 0 ], t[ 1 ] ) );
}
template < typename T, typename F >
void load3( const F& f, int location, const any& value ) {
  const auto& t = value.cast<T>();
  GL_CALL( f( location, t[ 0 ], t[ 1 ], t[ 2 ] ) );
}
template < typename T, typename F >
void load4( const F& f, int location, const any& value ) {
  const auto& t = value.cast<T>();
  GL_CALL( f( location, t[ 0 ], t[ 1 ], t[ 2 ], t[ 3 ] ) );
}
template < typename V, typename T, typename F>
void loadv( const F& f, int location, const any& value, int stride = 1 ) {
  const auto& v = value.cast<V>();
  const int count = ( int )v.size() / stride;
  const auto pv = reinterpret_cast<const T*>( v.data() );
  GL_CALL( f( location, count, pv ) );
}
template < typename T, typename F >
void loadm( const F& f, int location, const any& value ) {
  const auto& t = value.cast<T>();
  const int count = 1;
  const auto pv = reinterpret_cast<const float*>( &t );
  GL_CALL( f( location, count, false, pv ) );
}
template < typename V, typename F>
void loadmv( const F& f, int location, const any& value ) {
  const auto& v = value.cast<V>();
  const int count = ( int )v.size();
  const auto pv = reinterpret_cast<const float*>( v.data() );
  GL_CALL( f( location, count, false, pv ) );
}

/////////////////////////////////////////////////////////////////////////

template < int UniformType > struct UniformToType { };

#define DECLARE_UNIFORM(UNIFORM_TYPE, TYPE, FUNC, FUNC_IMPL) \
  template <> struct UniformToType<Uniform:: UNIFORM_TYPE>   { \
    static void load( int location, const any& value ) {     \
      FUNC_IMPL < TYPE > ( FUNC, location, value );        \
    }                                                        \
  };

#define DECLARE_UNIFORM_V(UNIFORM_TYPE, TYPE, ELEM_TYPE, FUNC, STRIDE) \
  template <> struct UniformToType<Uniform:: UNIFORM_TYPE>   {         \
    static void load( int location, const any& value ) {             \
      loadv < TYPE, ELEM_TYPE > ( FUNC, location, value, STRIDE ); \
    }                                                                \
  };

DECLARE_UNIFORM( c,  Color,   glUniform3f, load3 )
DECLARE_UNIFORM( i,  int,     glUniform1i, load1 )
DECLARE_UNIFORM( t,  int,     glUniform1i, load1 )

DECLARE_UNIFORM( f,  float,   glUniform1f, load1 )
DECLARE_UNIFORM( v2, Vector2, glUniform2f, load2 )
DECLARE_UNIFORM( v3, Vector3, glUniform3f, load3 )
DECLARE_UNIFORM( v4, Vector4, glUniform4f, load4 )

DECLARE_UNIFORM( m4,  Matrix4,              glUniformMatrix4fv, loadm );
DECLARE_UNIFORM( m4v, std::vector<Matrix4>, glUniformMatrix4fv, loadmv );

DECLARE_UNIFORM_V( iv,  std::vector<int>,     int,   glUniform3iv, 3 );
DECLARE_UNIFORM_V( iv1, std::vector<int>,     int,   glUniform1iv, 1 );
DECLARE_UNIFORM_V( fv,  std::vector<float>,   float, glUniform3fv, 3 );
DECLARE_UNIFORM_V( fv1, std::vector<float>,   float, glUniform1fv, 1 );
DECLARE_UNIFORM_V( v2v, std::vector<Vector2>, float, glUniform2fv, 1 );
DECLARE_UNIFORM_V( v3v, std::vector<Vector3>, float, glUniform3fv, 1 );
DECLARE_UNIFORM_V( v4v, std::vector<Vector4>, float, glUniform4fv, 1 );

#undef DECLARE_UNIFORM
#undef DECLARE_UNIFORM_V

void Uniform::load( int location ) {

  try {

    switch ( type ) {

    case Uniform::i: // single integer
      UniformToType<Uniform::i>::load( location, value ); break;
    case Uniform::f: // single float
      UniformToType<Uniform::f>::load( location, value ); break;
    case Uniform::v2: // single THREE::Vector2
      UniformToType<Uniform::v2>::load( location, value ); break;
    case Uniform::v3: // single THREE::Vector3
      UniformToType<Uniform::v3>::load( location, value ); break;
    case Uniform::v4: // single THREE::Vector4
      UniformToType<Uniform::v4>::load( location, value ); break;
    case Uniform::c: // single THREE::Color
      UniformToType<Uniform::c>::load( location, value ); break;
    case Uniform::iv1: // flat array of integers (JS or typed array)
      UniformToType<Uniform::iv1>::load( location, value ); break;
    case Uniform::iv: // flat array of integers with 3 x N size (JS or typed array)
      UniformToType<Uniform::iv>::load( location, value ); break;
    case Uniform::fv1: // flat array of floats (JS or typed array)
      UniformToType<Uniform::fv1>::load( location, value ); break;
    case Uniform::fv: // flat array of floats with 3 x N size (JS or typed array)
      UniformToType<Uniform::fv>::load( location, value ); break;
    case Uniform::v2v: // array of THREE::Vector2
      UniformToType<Uniform::v2v>::load( location, value ); break;
    case Uniform::v3v: // array of THREE::Vector3
      UniformToType<Uniform::v3v>::load( location, value ); break;
    case Uniform::v4v: // array of THREE::Vector4
      UniformToType<Uniform::v4v>::load( location, value ); break;
    case Uniform::m4: // single THREE::Matrix4
      UniformToType<Uniform::m4>::load( location, value ); break;
    case Uniform::m4v: // array of THREE::Matrix4
      UniformToType<Uniform::m4v>::load( location, value ); break;
    case Uniform::t: // single THREE::Texture (2d or cube)
      UniformToType<Uniform::t>::load( location, value ); break;
#ifdef TODO_TEXTURE_ARRAY
    case Uniform::tv: // array of THREE::Texture (2d)
      std::vector<int> _array( uniform.texture.size() );
      for ( i = 0, il = uniform.texture.size(); i < il; i ++ ) {
        _array[ i ] = value + i;
      }
      glUniform1iv( location, uniform._array );
      for ( i = 0, il = uniform.texture.size(); i < il; i ++ ) {
        texture = uniform.texture[ i ];
        if ( !texture ) continue;
        setTexture( texture, uniform._array[ i ] );
      }
      break;
#endif // TODO_TEXTURE_ARRAY
    default:
      console().warn() << "Unsupported uniform type: " << type;
      break;
    };

  } catch ( detail::bad_any_cast& ) {
    console().warn() << "Uniform value/type mismatch" << type;
  }

}

} // namespace three

#endif // THREE_UNIFORM_IPP