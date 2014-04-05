#ifndef THREE_MATH_H
#define THREE_MATH_H

#include <three/common.h>

#include <cmath>
#include <random>

#define NEAR_ZERO_FLOAT_32 0.0000000000000000001f
#define MATH_PI 3.1415926535897932384f
#define MATH_LN2 0.6931471805599453094f
#define MATH_DEG_TO_RAD_FACTOR (3.1415926535897932384f/180)
#define MATH_RAD_TO_DEG_FACTOR (180.f/MATH_PI)

namespace three {

namespace Math {

inline const float PI()   {
  return MATH_PI;
}
inline const float LN2()  {
  return MATH_LN2;
}
inline const float INF()  {
  return std::numeric_limits<float>::max();
}

inline static std::string generateUUID() {

  char chars[63] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

  std::string uuid;

  int rnd = 0, r;

  for ( auto i = 0; i < 36; i ++ ) {

    if ( i == 8 || i == 13 || i == 18 || i == 23 ) {

      uuid.append("-");

    } else if ( i == 14 ) {

      uuid.append("4");

    } else {

      if (rnd <= 0x02) rnd = 0x2000000 + ((std::rand()*0x1000000)|0);
      r = rnd & 0xf;
      rnd = rnd >> 4;
      uuid.append(&chars[(i == 19) ? (r & 0x3) | 0x8 : r]);

    }
  }

  return uuid;

};


template < typename T > inline T sqrt( T t ) {
  return std::sqrt( t );
}
template < typename T > inline T abs( T t )  {
  return std::abs( t );
}

template < typename T > inline T acos( T t ) {
  return std::acos( t );
}
template < typename T > inline T asin( T t ) {
  return std::asin( t );
}
template < typename T > inline T atan2( T y, T x ) {
  return std::atan2( y, x );
}
template < typename T > inline T atan( T t ) {
  return std::atan( t );
}
template < typename T > inline T cos( T t )  {
  return std::cos( t );
}
template < typename T > inline T sin( T t )  {
  return std::sin( t );
}
template < typename T > inline T tan( T t )  {
  return std::tan( t );
}
template < typename T > inline T log( T t )  {
  return std::log( t );
}
template < typename T, typename U > inline T pow( T a, U b )   {
  return std::pow( a, b );
}
template < typename T > inline T degToRad( T a )   {
  return a * MATH_DEG_TO_RAD_FACTOR;
}

template < typename T > inline T radToDeg( T a ) {
    return a * MATH_RAD_TO_DEG_FACTOR;
}

#if defined(_MSC_VER)
template < typename T > inline T round( T n ) {
  return ( n > ( T )0 ) ? std::floor( n + ( T )0.5 ) : std::ceil( n - ( T )0.5 );
}
#else
template < typename T > inline T round( T t ) {
  return std::round( t );
}
#endif

template < typename T > inline T ceil( T t )  {
  return std::ceil( t );
}
template < typename T > inline T floor( T t ) {
  return std::floor( t );
}

template < typename T > inline T fmod( T a, T b ) {
  return std::fmod( a, b );
}

template < typename T > inline T min( T a, T b ) {
  return std::min( a, b );
}
template < typename T > inline T min( T a, T b, T c ) {
  return std::min( std::min( a, b ), c );
}
template < typename T > inline T max( T a, T b ) {
  return std::max( a, b );
}
template < typename T > inline T max( T a, T b, T c) {
  return std::max( std::max( a, b ), c );
}

template < typename T > inline T clamp( T x, T a, T b )  {
  return x < a ? a : ( ( x > b ) ? b : x );
}
template < typename T > inline T clampBottom( T x, T a ) {
  return x < a ? a : x;
}

// Linear mapping from range <a1, a2> to range <b1, b2>
template < typename T >
inline T mapLinear( T x, T a1, T a2, T b1, T b2 ) {
  return b1 + ( x - a1 ) * ( b2 - b1 ) / ( a2 - a1 );
}

// MinGW crashes on std::random_device initialization
#if !defined(__MINGW32__)

template < typename T >
inline typename std::enable_if<std::is_floating_point<T>::value,T>::type
randomT( T low , T high ) {
  static std::random_device rd;
  static std::mt19937 rng( rd() );
  std::uniform_real_distribution<T> dis( low, high );
  return dis( rng );
}

template < typename T >
inline typename std::enable_if<!std::is_floating_point<T>::value,T>::type
randomT( T low , T high ) {
  static std::random_device rd;
  static std::mt19937 rng( rd() );
  std::uniform_int_distribution<T> dis( low, high );
  return dis( rng );
}

#else

template< typename T >
inline T randomT( T low, T high ) {
  return ( low + static_cast<double>(rand())/((unsigned long long)RAND_MAX+1) * ( high - low ));
}

#endif // !defined(__MINGW32__)

inline float random( float low = 0, float high = 1 ) {
  return randomT( low, high );
}

// Random float from <0, 1> with 16 bits of randomness
// (standard Math.random() creates repetitive patterns when applied over larger space)
inline float random16() {
  return ( 65280 * random() + 255 * random() ) / 65535;
}

// Random integer from <low, high> interval
inline int randInt( int low, int high ) {
  return low + ( int )floor( random() * ( high - low + 1 ) );
}

// Random float from <low, high> interval
inline float randFloat( float low, float high ) {
  return random( low, high );
}

// Random float from <-range/2, range/2> interval
inline float randFloatSpread( float range ) {
  return range * ( 0.5f - random() );
}

template < typename T >
inline int sign( T x ) {
  return ( x < 0 ) ? -1 : ( ( x > 0 ) ? 1 : 0 );
}

inline bool isPowerOfTwo( int value ) {
  return ( value != 0 ) && ( ( value & ( value - 1 ) ) == 0 );
}

inline int upperPowerOfTwo( int value ) {
  return (int)pow( 2.f, ceil( log( (float)value ) / LN2() ) );
}

inline int lowerPowerOfTwo( int value ) {
  return (int)pow( 2.f, floor( log( (float)value ) / LN2() ) );
}

inline int nearestPowerOfTwo( int value ) {
  return (int)pow( 2.f, round( log( (float)value ) / LN2() ) );
}

}

} // namespace three

#endif // THREE_MATH_H