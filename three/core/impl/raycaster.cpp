#ifndef THREE_RAYCASTER_CPP
#define THREE_RAYCASTER_CPP

#include <three/core/raycaster.h>

namespace three {

  float Raycaster::precision = 0.0001f;
  float Raycaster::linePrecision = 1.f;

  struct Raycaster::Impl {};
    
  Raycaster& Raycaster::set( const Vector3& origin, const Vector3& direction ) {

    ray.set( origin, direction );
    // direction is assumed to be normalized (for accurate distance calculations)
      
      return *this;

  }

  Intersects Raycaster::intersectObject ( const Object3D& object, bool recursive ) const {

    Intersects intersects;

    if ( recursive == true ) {

      //intersectDescendants( object, *this, intersects );

    }

    //intersectObject( object, *this, intersects );

    //intersects.sort( descSort() );

    return intersects;

  };

  Intersects Raycaster::intersectObjects ( const std::vector<Object3D::Ptr>& objects, bool recursive ) const {

    Intersects intersects;

    for ( auto& obj : objects ) {

      //intersectObject( *obj, *this, intersects );

      if ( recursive == true ) {

        //intersectDescendants( *obj, *this, intersects );

      }

    }

    //intersects.sort( descSort() );

    return intersects;

  };
    
} // end namespace

#endif // THREE_RAYCASTER_CPP
