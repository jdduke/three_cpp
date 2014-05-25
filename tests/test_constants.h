#ifndef THREE_TESTS_CONSTANTS_H
#define THREE_TESTS_CONSTANTS_H

#include <three/math/vector2.h>
#include <three/math/vector3.h>

extern float x, y, z, w;
extern three::Vector2 negInf2, posInf2;
extern three::Vector2 zero2, one2, two2;
extern three::Vector3 negInf3, posInf3;
extern three::Vector3 zero3, one3, two3;
extern three::Vector3 unit3;

bool matrixEquals4 ( const three::Matrix4& a, const three::Matrix4& b, float tolerance = 0.0001 );

#endif