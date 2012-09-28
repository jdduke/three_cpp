#ifndef THREE_FRUSTUM_IPP
#define THREE_FRUSTUM_IPP

#include <three/core/frustum.hpp>

#include <three/core/object3d.hpp>
#include <three/core/geometry.hpp>
#include <three/core/vector4.hpp>

#include <three/objects/mesh.hpp>

#include <array>

namespace three {

void Frustum::setFromMatrix( const Matrix4& m ) {

  const auto& me = m.elements;

  auto me0 = me[0],   me1 = me[1],   me2 = me[2],   me3 = me[3];
  auto me4 = me[4],   me5 = me[5],   me6 = me[6],   me7 = me[7];
  auto me8 = me[8],   me9 = me[9],   me10 = me[10], me11 = me[11];
  auto me12 = me[12], me13 = me[13], me14 = me[14], me15 = me[15];

  planes[ 0 ].set( me3 - me0, me7 - me4, me11 - me8, me15 - me12 );
  planes[ 1 ].set( me3 + me0, me7 + me4, me11 + me8, me15 + me12 );
  planes[ 2 ].set( me3 + me1, me7 + me5, me11 + me9, me15 + me13 );
  planes[ 3 ].set( me3 - me1, me7 - me5, me11 - me9, me15 - me13 );
  planes[ 4 ].set( me3 - me2, me7 - me6, me11 - me10, me15 - me14 );
  planes[ 5 ].set( me3 + me2, me7 + me6, me11 + me10, me15 + me14 );

  for ( auto& plane : planes ) {
    plane.divideScalar( Math::sqrt( plane.x * plane.x + plane.y * plane.y + plane.z * plane.z ) );
  }

}

bool Frustum::contains( const Object3D& object ) {

  if ( !object.geometry )
    return false;

  auto distance = 0.0f;
  const auto& matrix = object.matrixWorld;
  const auto& me = matrix.elements;
  auto radius = -object.geometry->boundingSphere.radius * matrix.getMaxScaleOnAxis();

  for ( int i = 0; i < 6; i ++ ) {
    distance = planes[ i ].x * me[12] + planes[ i ].y * me[13] + planes[ i ].z * me[14] + planes[ i ].w;
    if ( distance <= radius )
      return false;
  }

  return true;

}

} // namespace three

#endif // THREE_FRUSTUM_IPP