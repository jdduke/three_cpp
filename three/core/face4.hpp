#ifndef THREE_FACE4_HPP
#define THREE_FACE4_HPP

#include <three/common.hpp>

#include <three/core/color.hpp>
#include <three/core/vector3.hpp>

#include <vector>

namespace three {

class Face4 {
public:

    Face4 ( int a, int b, int c, int d, const Vector3& normal = Vector3(), const Color& color = Color(), int materialIndex = 0 )
        : a ( a ), b ( b ), c ( c ), d ( d ), normal ( normal ), color ( color ), materialIndex ( materialIndex ) { }

    Face4 ( const Face4& other ) = default;
    Face4 ( Face4 && )           = default;
    Face4& operator= ( const Face4& other ) = default;
    Face4& operator= ( Face4 && other )     = default;

    Face4 clone() const {
        return Face4 ( *this );
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

private:

    Face4( ) = delete;

};

} // namespace three

#endif // THREE_FACE4_HPP