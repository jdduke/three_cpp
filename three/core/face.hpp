#ifndef THREE_FACE_HPP
#define THREE_FACE_HPP

#include <three/common.hpp>

#include <three/core/color.hpp>
#include <three/core/vector3.hpp>
#include <three/core/vector4.hpp>

#include <vector>

namespace three {

class Face {
public:

    Face ( int a, int b, int c, const Vector3& normal = Vector3(), const Color& color = Color(), int materialIndex = 0 )
        : a ( a ), b ( b ), c ( c ), normal ( normal ), color ( color ), materialIndex ( materialIndex ), mType ( THREE::Face3 ), mSize ( 3 ) { }

    Face ( int a, int b, int c, int d, const Vector3& normal = Vector3(), const Color& color = Color(), int materialIndex = 0 )
        : a ( a ), b ( b ), c ( c ), d ( d ), normal ( normal ), color ( color ), materialIndex ( materialIndex ), mType ( THREE::Face4 ), mSize ( 4 ) { }

    Face ( const Face& other ) = default;
    Face ( Face&& )            = default;
    Face& operator= ( const Face& other ) = default;
    Face& operator= ( Face&& other )      = default;

    THREE::FaceType type() const {
        return mType;
    }

    int size() const {
        return mSize;
    }

    Face clone() const {
        return Face ( *this );
    }

public:

    union {
        struct { int a, b, c, d; };
        int abcd[4];
    };

    Vector3 normal;
    Vector3 vertexNormals[4];

    Color color;
    Color vertexColors[4];

    Vector4 vertexTangents[4];

    int materialIndex;

    Vector3 centroid;

private:

    THREE::FaceType mType;
    int mSize;

    Face( ) = delete;

};

} // namespace three

#endif // THREE_FACE_HPP