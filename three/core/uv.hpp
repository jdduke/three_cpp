#ifndef THREE_UV_HPP
#define THREE_UV_HPP

#include <three/common.hpp>

namespace three {

class UV {
public:

	union {
		struct { float u, v; };
		float uv[2];
	};

	UV ( float u = 0, float v = 0 )
		: u ( u ), v ( v ) { }

	UV ( const UV& ) = default;
	UV& operator= ( const UV& ) = default;

	UV& set ( float uIn, float vIn ) {

		u = uIn;
		v = vIn;

		return *this;

	}

	UV& copy ( const UV& uv ) {

		u = uv.u;
		v = uv.v;

		return *this;

	}

	UV& lerpSelf ( const UV& uv, float alpha ) {

		u += ( uv.u - u ) * alpha;
		v += ( uv.v - v ) * alpha;

		return *this;

	}

	UV clone () {

		return UV ( *this );

	}

};

} // namespace three

#endif // THREE_UV_HPP