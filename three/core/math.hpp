#ifndef THREE_MATH_HPP
#define THREE_MATH_HPP

#include <three/common.hpp>

#include <cmath>
#include <random>

namespace three {

class Math {

	static const float PI;

	template < typename T > static T sqrt( T t )  { return std::sqrt( t ); }
	template < typename T > static T abs( T t )   { return std::abs( t ); }

	template < typename T > static T acos( T t )  { return std::acos( t ); }
	template < typename T > static T asin( T t )  { return std::asin( t ); }
	template < typename T > static T atan2( T t ) { return std::atan2( t ); }
	template < typename T > static T atan( T t )  { return std::atan( t ); }
	template < typename T > static T cos( T t )   { return std::cos( t ); }
	template < typename T > static T sin( T t )   { return std::sin( t ); }
	template < typename T > static T tan( T t )   { return std::tan( t ); }

	template < typename T > static T ceil ( T t )  { return std::ceil( t ); }
	template < typename T > static T floor ( T t ) { return std::floor( t ); }

	template < typename T > static T min( T a, T b )   { return std::min( a, b ); }
	template < typename T > static T max( T a, T b )   { return std::max( a, b ); }
	template < typename T > static T clamp ( T x, T a, T b )  { return x < a ? a : ( ( x > b ) ? b : x ); }
	template < typename T > static T clampBottom ( T x, T a ) { return x < a ? a : x; }

	// Linear mapping from range <a1, a2> to range <b1, b2>
	template < typename T >
	static mapLinear( T x, T a1, T a2, T b1, T b2 ) {
		return b1 + ( x - a1 ) * ( b2 - b1 ) / ( a2 - a1 );
	}

	// Random between 0 and 1
	template < typename T >
	static T randomT ( T low , T high ) {
		static std::random_device rd;
		static std::mt19937 rng( rd() );
		std::uniform_real_distribution<T> dis ( low, high );
		return dis();
	}

	static float random ( float low = 0, float high = 1 ) {
		return randomT ( low, high );
	}

	// Random float from <0, 1> with 16 bits of randomness
	// (standard Math.random() creates repetitive patterns when applied over larger space)
	static float random16() {
		return ( 65280 * random() + 255 * random() ) / 65535;
	}

	// Random integer from <low, high> interval
	static int randInt ( int low, int high ) {
		return low + floor( Math.random() * ( high - low + 1 ) );
	}

	// Random float from <low, high> interval
	static float randFloat ( float low, float high ) {
		return random ( low, high );
	}

	// Random float from <-range/2, range/2> interval
	static float randFloatSpread ( float range ) {
		return range * ( 0.5f - random() );
	}

	template < typename T >
	static int sign ( T x ) {
		return ( x < 0 ) ? -1 : ( ( x > 0 ) ? 1 : 0 );
	}

};

float Math::PI = Math.atan(1.f)/4;

} // namespace three

#endif // THREE_MATH_HPP