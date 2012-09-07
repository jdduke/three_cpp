#ifndef _UV_HPP_
#define _UV_HPP_

#include "three.hpp"

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

		u += ( uv.u - this.u ) * alpha;
		v += ( uv.v - this.v ) * alpha;

		return *this;

	}

	UV clone () {

		return UV ( *this );

	}

};

}

#endif