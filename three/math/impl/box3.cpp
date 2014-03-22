#ifndef THREE_BOX3_CPP
#define THREE_BOX3_CPP

#include <three/core/object3d.h>

#include <three/math/box3.h>
#include <three/math/vector3.h>

namespace three {

Box3& Box3::setFromObject ( Object3D& object ) {

  // Computes the world-axis-aligned bounding box of an object (including its children),
  // accounting for both the object's, and childrens', world transforms

  auto v1 = Vector3();

  object.updateMatrixWorld( true );

  makeEmpty();

  auto scope = this;

  object.traverse( [&]( const Object3D& node ) {

    if ( node.geometry ) {

      const auto& vertices = node.geometry->vertices;

      for ( auto it = vertices.begin(); it != vertices.end(); it++ ) {
        v1.copy( *it );
        v1.applyMatrix4( node.matrixWorld );
        scope->expandByPoint( v1 );
      }

    }

  });

  return *this;
}

} // namespace three

#endif // THREE_BOX3_CPP