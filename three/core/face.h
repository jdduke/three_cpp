#ifndef enums_FACE_H
#define enums_FACE_H

#include <three/common.h>
#include <three/constants.h>
#include <three/math/vector3.h>
#include <three/math/vector4.h>
#include <three/math/color.h>

#include <array>

namespace three {

class Face {

public:

  typedef std::shared_ptr<Face> Ptr;

  static Ptr create( int a, int b, int c) {
    return three::make_shared<Face>(a, b, c);
  }

  static Ptr create( int a, int b, int c, int d) {
    return three::make_shared<Face>(a, b, c, d);
  }

  Face( int a, int b, int c, const Vector3& normal = Vector3(), const Color& color = Color(), int materialIndex = -1 )
    : a( a ), b( b ), c( c ), normal( normal ), color( color ), materialIndex( materialIndex ), mType( enums::Face3 ), mSize( 3 ) { }

  Face( int a, int b, int c, const Vector3& n1, const Vector3& n2, const Vector3& n3, const Color& color = Color(), int materialIndex = -1 )
    : a( a ), b( b ), c( c ), color( color ), materialIndex( materialIndex ), mType( enums::Face3 ), mSize( 3 ) {
    vertexNormals[0] = n1;
    vertexNormals[1] = n2;
    vertexNormals[2] = n3;
  }

  Face( int a, int b, int c, int d, const Vector3& normal = Vector3(), const Color& color = Color(), int materialIndex = -1 )
    : a( a ), b( b ), c( c ), d( d ), normal( normal ), color( color ), materialIndex( materialIndex ), mType( enums::Face4 ), mSize( 4 ) { }

  Face( int a, int b, int c, int d, const Vector3& n1, const Vector3& n2, const Vector3& n3, const Vector3& n4, const Color& color = Color(), int materialIndex = -1 )
    : a( a ), b( b ), c( c ), d( d ), color( color ), materialIndex( materialIndex ), mType( enums::Face4 ), mSize( 3 ) {
    vertexNormals[0] = n1;
    vertexNormals[1] = n2;
    vertexNormals[2] = n3;
    vertexNormals[3] = n4;
  }

  union {
    struct {
      int a, b, c, d;
    };
    int abcd[4];
  };

  Vector3 normal;
  std::array<Vector3, 4> vertexNormals;

  Color color;
  std::array<Color, 4> vertexColors;

  std::array<Vector4, 4> vertexTangents;

  int materialIndex;

  Vector3 centroid;

  enums::FaceType type() const {
    return mType;
  }

  int size() const {
    return mSize;
  }

  THREE_REVIEW("EA: Deep copy?")
  Face clone() const {
    return Face( *this );
  }

private:

  enums::FaceType mType;
  int mSize;

  Face() {}

};

} // namespace three

#endif // enums_FACE_H