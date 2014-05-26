#include <three/core/raycaster.h>

#include <three/core/buffer_geometry.h>
#include <three/core/object3d.h>
#include <three/materials/mesh_face_material.h>
#include <three/math/plane.h>
#include <three/objects/mesh.h>
#include <three/objects/sprite.h>
#include <three/objects/LOD.h>
#include <three/objects/line.h>
#include <three/utils/optional.h>

namespace three {

struct Intersect {

  public:

    Intersect( float distanceIn, Vector3& pointIn, optional<Face> faceIn, size_t faceIndexIn = 0, Object3D* objectIn = nullptr )
    : distance( distanceIn), point( pointIn ), face( std::move( faceIn ) ), faceIndex( faceIndexIn ), object( objectIn ) {}

    float distance;

    Vector3 point;

    optional<Face> face;
    size_t faceIndex;

    Object3D* object;

};

struct DescSort {
  bool operator()( const Intersect& a, const Intersect& b ) const {
    return a.distance - b.distance < 0.f;
  }
};

float Raycaster::precision = 0.0001f;
float Raycaster::linePrecision = 1.f;

struct Raycaster::Impl : public NonCopyable {

  Impl() {}

  Sphere sphere;
  Ray localRay;
  Plane facePlane;
  Vector3 intersectPoint;
  Vector3 matrixPosition;

  Matrix4 inverseMatrix;

  Vector3 vA;
  Vector3 vB;
  Vector3 vC;

};

namespace detail {


struct IntersectObjectVisitor : public ConstRawPointerVisitor {

  IntersectObjectVisitor( Raycaster& raycasterIn, Raycaster::Impl& implIn, Intersects& intersectsIn )
    : raycaster( raycasterIn ), impl( implIn ), intersects( intersectsIn ) { }

  Raycaster& raycaster;
  Raycaster::Impl& impl;
  Intersects& intersects;

  virtual void operator()( const Sprite* object ) {

    impl.matrixPosition.setFromMatrixPosition( object->matrixWorld );
    float distance = raycaster.ray.distanceToPoint( impl.matrixPosition );

    if ( distance > object->scale().x ) {

      return;

    }

    auto position = object->position();

    intersects.push_back(Intersect(
                           distance,
                           position,
                           nullptr,
                           0,
                           (Object3D*)object
                         ));

  }

  virtual void operator()( const LOD* object ) {

    impl.matrixPosition.setFromMatrixPosition( object->matrixWorld );
    float distance = raycaster.ray.origin.distanceTo( impl.matrixPosition );

    raycaster.intersectObject( object->getObjectForDistance( distance ) );

  }

  virtual void operator()( const Mesh* object ) {

    auto& geometry = object->geometry;

    // Checking boundingSphere distance to ray

    if ( !geometry->boundingSphere ) geometry->computeBoundingSphere();

    impl.sphere.copy( *geometry->boundingSphere );
    impl.sphere.applyMatrix4( object->matrixWorld );

    if ( raycaster.ray.isIntersectionSphere( impl.sphere ) == false ) {

      return;

    }

    // Check boundingBox before continuing

    impl.inverseMatrix.getInverse( object->matrixWorld );
    impl.localRay.copy( raycaster.ray ).applyMatrix4( impl.inverseMatrix );

    if ( geometry->boundingBox ) {

      if ( impl.localRay.isIntersectionBox( *geometry->boundingBox ) == false )  {

        return;

      }

    }

    if ( geometry->type() == enums::BufferGeometry ) {

      visitMeshBufferGeometry( object, (BufferGeometry*) geometry.get() );

    } else if ( geometry->type() == enums::Geometry ) {

      visitMeshGeometry( object, (Geometry*) geometry.get() );

    }

  }

  void visitMeshBufferGeometry( const Mesh* object, const BufferGeometry* geometry ) {

    auto& material = object->material;

    if ( material == nullptr ) return;
    if ( geometry->dynamic == false ) return;

    float precision = raycaster.precision;

    if ( geometry->attributes.contains( AttributeKey::index() ) ) {

      const auto& offsets = geometry->offsets;
      const auto& indices   = geometry->attributes.get( AttributeKey::index() )->array;
      const auto& positions = geometry->attributes.get( AttributeKey::position() )->array;


      for ( size_t oi = 0; oi < offsets.size(); ++oi ) {

        int start = offsets[ oi ].start;
        int count = offsets[ oi ].count;
        int index = offsets[ oi ].index;

        for ( size_t i = start, il = start + count; i < il; i += 3 ) {

          int a = index + (int)indices[ i ];
          int b = index + (int)indices[ i + 1 ];
          int c = index + (int)indices[ i + 2 ];

          impl.vA.set(
            positions[ a * 3 ],
            positions[ a * 3 + 1 ],
            positions[ a * 3 + 2 ]
          );
          impl.vB.set(
            positions[ b * 3 ],
            positions[ b * 3 + 1 ],
            positions[ b * 3 + 2 ]
          );
          impl.vC.set(
            positions[ c * 3 ],
            positions[ c * 3 + 1 ],
            positions[ c * 3 + 2 ]
          );

          optional<Vector3> intersectionPoint;
          if ( material->side == enums::BackSide ) {

            intersectionPoint = impl.localRay.intersectTriangle( impl.vC, impl.vB, impl.vA, true );

          } else {

            intersectionPoint = impl.localRay.intersectTriangle( impl.vA, impl.vB, impl.vC, material->side != enums::DoubleSide );

          }

          if ( !intersectionPoint ) continue;

          intersectionPoint->applyMatrix4( object->matrixWorld );

          float distance = raycaster.ray.origin.distanceTo( *intersectionPoint );

          if ( distance < precision || distance < raycaster.near || distance > raycaster.far ) continue;

          intersects.push_back(Intersect(
                                 distance,
                                 *intersectionPoint,
                                 nullptr,
                                 0,
                                 (Object3D*)object
                               ));

        }

      }

    } else {

      const auto& offsets = geometry->offsets;
      const auto& positions = geometry->attributes.get( AttributeKey::position() )->array;

      for ( size_t i = 0; i < offsets.size(); i += 3 ) {

        const auto a = i;
        const auto b = i + 1;
        const auto c = i + 2;

        impl.vA.set(
          positions[ a * 3 ],
          positions[ a * 3 + 1 ],
          positions[ a * 3 + 2 ]
        );
        impl.vB.set(
          positions[ b * 3 ],
          positions[ b * 3 + 1 ],
          positions[ b * 3 + 2 ]
        );
        impl.vC.set(
          positions[ c * 3 ],
          positions[ c * 3 + 1 ],
          positions[ c * 3 + 2 ]
        );


        optional<Vector3> intersectionPoint;
        if ( material->side == enums::BackSide ) {

          intersectionPoint = impl.localRay.intersectTriangle( impl.vC, impl.vB, impl.vA, true );

        } else {

          intersectionPoint = impl.localRay.intersectTriangle( impl.vA, impl.vB, impl.vC, material->side != enums::DoubleSide );

        }

        if ( !intersectionPoint ) continue;

        intersectionPoint->applyMatrix4( object->matrixWorld );

        float distance = raycaster.ray.origin.distanceTo( *intersectionPoint );

        if ( distance < precision || distance < raycaster.near || distance > raycaster.far ) continue;

        intersects.push_back(Intersect(
                               distance,
                               *intersectionPoint,
                               nullptr,
                               0,
                               (Object3D*)object
                             ));

      }

    }

  }

  void visitMeshGeometry( const Mesh* object, const Geometry* geometry ) {

    bool isFaceMaterial = object->material->type() == enums::MeshFaceMaterial;
    auto& objectMaterials = ((MeshFaceMaterial*)object)->materials;

    Vector3 a, b, c;
    float precision = raycaster.precision;

    auto& vertices = geometry->vertices;

    for ( size_t f = 0, fl = geometry->faces.size(); f < fl; f ++ ) {

      auto& face = geometry->faces[ f ];

      auto& material = isFaceMaterial ? objectMaterials[ face.materialIndex ] : object->material;

      if ( material == nullptr ) continue;

      a = vertices[ face.a ];
      b = vertices[ face.b ];
      c = vertices[ face.c ];

      if ( material->morphTargets == true ) {

        auto& morphTargets = geometry->morphTargets;
        auto& morphInfluences = object->morphTargetInfluences;

        impl.vA.set( 0, 0, 0 );
        impl.vB.set( 0, 0, 0 );
        impl.vC.set( 0, 0, 0 );

        for ( size_t t = 0, tl = morphTargets.size(); t < tl; t ++ ) {

          float influence = morphInfluences[ t ];

          if ( influence == 0 ) continue;

          auto& targets = morphTargets[ t ].vertices;

          impl.vA.x += ( targets[ face.a ].x - a.x ) * influence;
          impl.vA.y += ( targets[ face.a ].y - a.y ) * influence;
          impl.vA.z += ( targets[ face.a ].z - a.z ) * influence;

          impl.vB.x += ( targets[ face.b ].x - b.x ) * influence;
          impl.vB.y += ( targets[ face.b ].y - b.y ) * influence;
          impl.vB.z += ( targets[ face.b ].z - b.z ) * influence;

          impl.vC.x += ( targets[ face.c ].x - c.x ) * influence;
          impl.vC.y += ( targets[ face.c ].y - c.y ) * influence;
          impl.vC.z += ( targets[ face.c ].z - c.z ) * influence;

        }

        impl.vA.add( a );
        impl.vB.add( b );
        impl.vC.add( c );

        a = impl.vA;
        b = impl.vB;
        c = impl.vC;

      }

      optional<Vector3> intersectionPoint;
      if ( material->side == enums::BackSide ) {

        intersectionPoint = impl.localRay.intersectTriangle( c, b, a, true );

      } else {

        intersectionPoint = impl.localRay.intersectTriangle( a, b, c, material->side != enums::DoubleSide );

      }

      if ( !intersectionPoint ) continue;

      intersectionPoint->applyMatrix4( object->matrixWorld );

      float distance = raycaster.ray.origin.distanceTo( *intersectionPoint );

      if ( distance < precision || distance < raycaster.near || distance > raycaster.far ) continue;

      intersects.push_back( Intersect(
                             distance,
                             *intersectionPoint,
                             make_optional(face),
                             f,
                             (Object3D*)object
                           ));

    }

  }

  virtual void operator()( const Line* object ) {

    float precision = raycaster.linePrecision;
    float precisionSq = precision * precision;

    auto& geometry = object->geometry;

    if ( !geometry->boundingSphere ) geometry->computeBoundingSphere();

    // Checking boundingSphere distance to ray

    impl.sphere.copy( *geometry->boundingSphere );
    impl.sphere.applyMatrix4( object->matrixWorld );

    if ( raycaster.ray.isIntersectionSphere( impl.sphere ) == false ) {

      return;

    }

    impl.inverseMatrix.getInverse( object->matrixWorld );
    impl.localRay.copy( raycaster.ray ).applyMatrix4( impl.inverseMatrix );

    if ( geometry->type() == enums::Geometry ) {

      const auto& vertices = geometry->vertices;

      size_t nbVertices = vertices.size();
      Vector3 interSegment;
      Vector3 interRay;

      int step = object->lineType == enums::LineStrip ? 1 : 2;

      for ( size_t i = 0; i < nbVertices - 1; i = i + step ) {

        float distSq = impl.localRay.distanceSqToSegment( vertices[ i ], vertices[ i + 1 ], &interRay, &interSegment );

        if ( distSq > precisionSq ) continue;

        float distance = impl.localRay.origin.distanceTo( interRay );

        if ( distance < raycaster.near || distance > raycaster.far ) continue;

        intersects.push_back(Intersect(
                               distance,
                               // What do we want? intersection point on the ray or on the segment??
                               // point: raycaster.ray.at( distance ),
                               interSegment.clone().applyMatrix4( object->matrixWorld ),
                               nullptr,
                               0,
                               (Object3D*)object
                             ));

      }

    }

  }

}; // end visitor


}; // end namespace detail


Raycaster::Raycaster( const Vector3& origin, const Vector3& direction, float near, float far)
  : ray( Ray( origin, direction ) ), near( near ), far( far ), impl( new Impl() ) { }

Raycaster& Raycaster::set( const Vector3& origin, const Vector3& direction ) {

  ray.set( origin, direction );
  // direction is assumed to be normalized (for accurate distance calculations)

  return *this;

}

Intersects Raycaster::intersectObject ( const Object3D::Ptr& object, bool recursive ) {

  Intersects intersects;

  if ( recursive == true ) {

    _intersectDescendants( object, intersects );

  }

  _intersectObject( object, intersects );

  std::sort( intersects.begin(),
             intersects.end(),
             DescSort() );

  return intersects;

};

Intersects Raycaster::intersectObjects ( const std::vector<Object3D::Ptr>& objects, bool recursive )  {

  Intersects intersects;

  for ( auto& obj : objects ) {

    _intersectObject( obj, intersects );

    if ( recursive == true ) {

      _intersectDescendants( obj, intersects );

    }

  }

  std::sort( intersects.begin(),
             intersects.end(),
             DescSort() );

  return intersects;

}

void Raycaster::_intersectDescendants( const Object3D::Ptr& object, Intersects& intersects ) {

  std::vector<Object3D::Ptr> descendants;

  object->getDescendants(descendants);

  for ( auto& descendant : descendants ) {

    _intersectObject( descendant, intersects );

  }

}

void Raycaster::_intersectObject( const Object3D::Ptr& object, Intersects& intersects ) {

  detail::IntersectObjectVisitor visitor( *this, *impl, intersects );

  object->visit( visitor );
}

} // end namespace
