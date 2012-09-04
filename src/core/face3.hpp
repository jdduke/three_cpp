#ifndef _FACE3_HPP_
#define _FACE3_HPP_

#include <vector>

#include "color.hpp"
#include "vector3.hpp"

namespace three {

class Face3 {
public:

	Face3 ( int a, int b, int c, const Vector3& normal = Vector3(), const Color& color = Color(), int materialIndex = 0)
		: a(a), b(b), c(c), normal(normal), color(color), materialIndex(materialIndex) {

	}

	Face3 ( ) = delete;

	Face3 ( const Face3& other ) = default;
	Face3 ( Face3&& ) = default;

	Face3& operator= ( const Face3& other ) = default;
	Face3& operator= ( Face3&& other ) = default;

	Face3 clone() const {

		return Face( *this );

	}


public:

	int a, b, c;

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