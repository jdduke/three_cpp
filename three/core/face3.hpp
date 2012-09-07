#ifndef THREE_FACE3_HPP
#define THREE_FACE3_HPP

#include <three/common.hpp>

#include <three/core/color.hpp>
#include <three/core/vector3.hpp>

#include <vector>

namespace three {

class Face3 {
public:

    Face3 ( int a, int b, int c, const Vector3& normal = Vector3(), const Color& color = Color(), int materialIndex = 0 )
        : a ( a ), b ( b ), c ( c ), normal ( normal ), color ( color ), materialIndex ( materialIndex ) {

    }

    Face3( ) = delete;

    Face3 ( const Face3& other ) = default;
    Face3 ( Face3 && ) = default;

    Face3& operator= ( const Face3& other ) = default;
    Face3& operator= ( Face3 && other ) = default;

    Face3 clone() const {

        return Face ( *this );

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

} // namespace three

#endif // THREE_FACE3_HPP