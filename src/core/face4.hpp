#ifndef _FACE4_HPP_
#define _FACE4_HPP_

#include <vector>

#include "color.hpp"
#include "vector3.hpp"

namespace three {

class Face4 {
public:

	Face4 ( int a, int b, int c, int d, const Vector3& normal = Vector3(), const Color& color = Color(), int materialIndex = 0)
		: a(a), b(b), c(c), d(d), normal(normal), color(color), materialIndex(materialIndex) {

	}

	Face4 ( ) = delete;

	Face4 ( const Face4& other ) = default;
	Face4 ( Face4&& ) = default;

	Face4& operator= ( const Face4& other ) = default;
	Face4& operator= ( Face4&& other ) = default;

	Face4 clone() const {

		return Face( *this );

	}


public:

	int a, b, c, d;

	Vector3 normal;
	std::vector<Vector3> vertexNormals;

	Color color;
	std::vector<Color> vertexColors;

	std::vector<Vector3> vertexTangents;

	int materialIndex;

	Vector3 centroid;

};

}

#endif