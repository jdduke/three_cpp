#include <three/math/frustum.h>

#include <three/core/object3d.h>
#include <three/core/geometry.h>

#include <three/math/vector4.h>
#include <three/math/sphere.h>
#include <three/math/box3.h>

#include <three/objects/mesh.h>

#include <array>

namespace three {


Frustum& Frustum::set( const Plane& p0, const Plane& p1, const Plane& p2, const Plane& p3, const Plane& p4, const Plane& p5 ) {

  planes[0].copy( p0 );
  planes[1].copy( p1 );
  planes[2].copy( p2 );
  planes[3].copy( p3 );
  planes[4].copy( p4 );
  planes[5].copy( p5 );

  return *this;

}

Frustum& Frustum::copy( const Frustum& frustum ) {

  // TODO EA: use STL
  for( char i = 0; i < 6; i ++ ) {

    planes[i].copy( frustum.planes[i] );

  }

  return *this;
}

Frustum& Frustum::setFromMatrix( const Matrix4& m ) {

    auto& me = m.elements;
    float me0 = me[0], me1 = me[1], me2 = me[2], me3 = me[3];
    float me4 = me[4], me5 = me[5], me6 = me[6], me7 = me[7];
    float me8 = me[8], me9 = me[9], me10 = me[10], me11 = me[11];
    float me12 = me[12], me13 = me[13], me14 = me[14], me15 = me[15];

    planes[ 0 ].setComponents( me3 - me0, me7 - me4, me11 - me8, me15 - me12 ).normalize();
    planes[ 1 ].setComponents( me3 + me0, me7 + me4, me11 + me8, me15 + me12 ).normalize();
    planes[ 2 ].setComponents( me3 + me1, me7 + me5, me11 + me9, me15 + me13 ).normalize();
    planes[ 3 ].setComponents( me3 - me1, me7 - me5, me11 - me9, me15 - me13 ).normalize();
    planes[ 4 ].setComponents( me3 - me2, me7 - me6, me11 - me10, me15 - me14 ).normalize();
    planes[ 5 ].setComponents( me3 + me2, me7 + me6, me11 + me10, me15 + me14 ).normalize();

    return *this;
}

bool Frustum::intersectsObject( const Object3D& object ) {

  if ( !object.geometry ) {

    return false;

  }
  //_sphere.reset(new Sphere);

  const auto& geometry = object.geometry;

  if ( !geometry->boundingSphere ) {

    geometry->computeBoundingSphere();

  }

  _sphere.copy( *geometry->boundingSphere );
  _sphere.applyMatrix4( object.matrixWorld );

  return intersectsSphere( _sphere );

}

bool Frustum::intersectsSphere( const Sphere& sphere ) const {

  const auto& center = sphere.center;
  float negRadius = -sphere.radius;

  for ( char i = 0; i < 6; i ++ ) {

    auto distance = planes[ i ].distanceToPoint( center );

    if ( distance < negRadius ) {

      return false;

    }

  }

  return true;

}

bool Frustum::intersectsBox( const Box3& box ) {

  for ( unsigned char i = 0; i < 6 ; i ++ ) {

    const auto& plane = planes[i];

    _p1.x = plane.normal.x > 0 ? box.min.x : box.max.x;
    _p2.x = plane.normal.x > 0 ? box.max.x : box.min.x;
    _p1.y = plane.normal.y > 0 ? box.min.y : box.max.y;
    _p2.y = plane.normal.y > 0 ? box.max.y : box.min.y;
    _p1.z = plane.normal.z > 0 ? box.min.z : box.max.z;
    _p2.z = plane.normal.z > 0 ? box.max.z : box.min.z;

    auto d1 = plane.distanceToPoint( _p1 );
    auto d2 = plane.distanceToPoint( _p2 );

    // if both outside plane, no intersection
    if ( d1 < 0 && d2 < 0 ) {

      return false;

    }

  }

  return true;

}

bool Frustum::containsPoint( const Vector3& point ) const {

  for ( size_t i = 0; i < 6; i ++ ) {

    if ( planes[ i ].distanceToPoint( point ) < 0.f ) {

      return false;

    }

  }

  return true;

}

Frustum Frustum::clone() const {
  return Frustum(*this);
}


} // namespace three
