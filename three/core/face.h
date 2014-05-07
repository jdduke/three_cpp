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

  Face( int a, int b, int c, const Vector3& normal = Vector3(), const Color& color = Color(), int materialIndex = -1 )
    : a( a ), b( b ), c( c ), normal( normal ), color( color ), materialIndex( materialIndex ) { }

  Face( int a, int b, int c, const Vector3& n1, const Vector3& n2, const Vector3& n3, const Color& color = Color(), int materialIndex = -1 )
    : a( a ), b( b ), c( c ), color( color ), materialIndex( materialIndex ) {
    vertexNormals[0] = n1;
    vertexNormals[1] = n2;
    vertexNormals[2] = n3;
  }

  union {
    struct {
      int a, b, c;
    };
    int abc[3];
  };

  Vector3 normal;
  std::array<Vector3, 3> vertexNormals;

  Color color;
  std::array<Color, 3> vertexColors;

  std::array<Vector4, 3> vertexTangents;

  int materialIndex;

  Vector3 centroid;

  enums::FaceType type() const {
    return enums::Face3;
  }

  int size() const {
    return 3;
  }

  THREE_REVIEW("EA: Does this do a deep copy?")
  Face clone() const {
    return Face( *this );
  }

private:

  Face() {}

};

} // namespace three

#endif // enums_FACE_H