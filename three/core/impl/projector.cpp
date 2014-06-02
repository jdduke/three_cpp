#include <three/core/projector.h>

#include <three/core/face.h>

#include <three/cameras/camera.h>

#include <three/math/math.h>

#include <three/math/frustum.h>
#include <three/math/matrix4.h>
#include <three/math/vector4.h>
#include <three/math/box3.h>

#include <three/objects/line.h>
#include <three/objects/mesh.h>
#include <three/objects/particle.h>

#include <three/renderers/renderables/renderable_vertex.h>
#include <three/renderers/renderables/renderable_face.h>
#include <three/renderers/renderables/renderable_line.h>
#include <three/renderers/renderables/renderable_sprite.h>

#include <three/scenes/scene.h>

#include <functional>

namespace three {

namespace detail {

template < typename Renderable >
struct Renderables {
  Renderables() : count( 0 ) { }

  Renderable& next() {
    if ( count == (int)pool.size() ) {
      pool.push_back( Renderable() );
      ++count;
      return pool.back();
    } else {
      return pool[ count++ ];
    }
  }
  Renderable& current() {
    return pool[ count ];
  }
  void reset() {
    count = 0;
  }

  std::vector<Renderable> pool;
  int count;
};

} // namespace detail

struct Projector::Impl : public NonCopyable {

  Impl()
    : _clipBox( Box3( Vector3( -1, -1, -1 ), Vector3( 1, 1, 1) ) ) {}

  detail::Renderables<RenderableObject>   _objects;
  detail::Renderables<RenderableVertex>   _vertices;
  detail::Renderables<RenderableFace>     _faces;
  detail::Renderables<RenderableLine>     _lines;
  detail::Renderables<RenderableSprite> _particles;

  Projector::RenderData _renderData;

  Vector3 _vA;
  Vector3 _vB;
  Vector3 _vC;

  Vector3 _vector3;
  Vector4 _vector4;

  Box3 _clipBox;
  Box3 _boundingBox;
  std::array<Vector4, 3> _points3;
  std::array<Vector4, 4> _points4;

  Matrix4 _viewMatrix;
  Matrix4 _viewProjectionMatrix;

  Matrix4 _modelMatrix;
  Matrix4 _modelViewProjectionMatrix;

  Matrix3 _normalMatrix;
  Matrix3 _normalViewMatrix;

  Vector3 _centroid;

  Frustum _frustum;

  Vector4 _clippedVertex1PositionScreen;
  Vector4 _clippedVertex2PositionScreen;

};

namespace detail {

static bool clipLine( Vector4& s1, Vector4& s2 ) {
  float alpha1 = 0, alpha2 = 1;

  // Calculate the boundary coordinate of each vertex for the near and far clip planes,
  // Z = -1 and Z = +1, respectively.
  auto bc1near =  s1.z + s1.w;
  auto bc2near =  s2.z + s2.w;
  auto bc1far =  - s1.z + s1.w;
  auto bc2far =  - s2.z + s2.w;

  if ( bc1near >= 0 && bc2near >= 0 && bc1far >= 0 && bc2far >= 0 ) {
    // Both vertices lie entirely within all clip planes.
    return true;
  } else if ( ( bc1near < 0 && bc2near < 0 ) || ( bc1far < 0 && bc2far < 0 ) ) {
    // Both vertices lie entirely outside one of the clip planes.
    return false;
  } else {
    // The line segment spans at least one clip plane.
    if ( bc1near < 0 ) {
      // v1 lies outside the near plane, v2 inside
      alpha1 = Math::max( alpha1, bc1near / ( bc1near - bc2near ) );
    } else if ( bc2near < 0 ) {
      // v2 lies outside the near plane, v1 inside
      alpha2 = Math::min( alpha2, bc1near / ( bc1near - bc2near ) );
    }

    if ( bc1far < 0 ) {
      // v1 lies outside the far plane, v2 inside
      alpha1 = Math::max( alpha1, bc1far / ( bc1far - bc2far ) );
    } else if ( bc2far < 0 ) {
      // v2 lies outside the far plane, v2 inside
      alpha2 = Math::min( alpha2, bc1far / ( bc1far - bc2far ) );
    }

    if ( alpha2 < alpha1 ) {
      // The line segment spans two boundaries, but is outside both of them.
      // (This can't happen when we're only clipping against just near/far but good
      //  to leave the check here for future usage if other clip planes are added.)
      return false;
    } else {
      // Update the s1 and s2 vertices to match the clipped line segment.
      s1.lerp( s2, alpha1 );
      s2.lerp( s1, 1 - alpha2 );
      return true;
    }
  }
}

struct SceneVisitor : public Visitor {
  Projector::Impl& p;
  Camera& c;
  Matrix4& modelMatrix;
  Matrix4& viewProjectionMatrix;

  SceneVisitor( Projector::Impl& p, Camera& c, Matrix4& model, Matrix4& viewProjection )
    : p( p ), c( c ), modelMatrix( model ), viewProjectionMatrix( viewProjection ) { }

  virtual void operator()( const Particle& p ) { }
  virtual void operator()( const Mesh& object ) {
    const auto& geometry          = *object.geometry;
//    const auto& geometryMaterials = geometry.materials;
    const auto& vertices          = geometry.vertices;
    const auto& faces             = geometry.faces;
    const auto& faceVertexUvs     = geometry.faceVertexUvs;


//    auto rotationMatrix = p._normalMatrix.getNormalMatrix( modelMatrix );

    auto isFaceMaterial = object.material->type() == THREE::MeshFaceMaterial;
    auto objectMaterials = isFaceMaterial ? object.material : nullptr;

    for ( const auto& v : vertices ) {

      auto& vertex = p._vertices.next();
      vertex.positionWorld.copy( v );

      projectVertex( p, vertex );

    }

    for ( size_t f = 0; f < faces.size(); ++f ) {

      auto& face = faces[ f ];

      bool visible = false;

      auto material = isFaceMaterial ? object.material : nullptr;

      if ( material == nullptr ) continue;

      auto side = material->side;

      auto& v1 = p._vertices.pool[ face.a ];
      auto& v2 = p._vertices.pool[ face.b ];
      auto& v3 = p._vertices.pool[ face.c ];

      if ( material->morphTargets == true ) {

        auto& morphTargets = geometry.morphTargets;
        auto& morphInfluences = object.morphTargetInfluences;

        auto& v1p = v1.position;
        auto& v2p = v2.position;
        auto& v3p = v3.position;

        p._vA.set( 0, 0, 0 );
        p._vB.set( 0, 0, 0 );
        p._vC.set( 0, 0, 0 );

        for ( size_t t = 0, tl = morphTargets.size(); t < tl; ++t ) {

          float influence = morphInfluences[ t ];

          if ( influence == 0 ) continue;

          auto& targets = morphTargets[ t ].vertices;

          p._vA.x += ( targets[ face.a ].x - v1p.x ) * influence;
          p._vA.y += ( targets[ face.a ].y - v1p.y ) * influence;
          p._vA.z += ( targets[ face.a ].z - v1p.z ) * influence;

          p._vB.x += ( targets[ face.b ].x - v2p.x ) * influence;
          p._vB.y += ( targets[ face.b ].y - v2p.y ) * influence;
          p._vB.z += ( targets[ face.b ].z - v2p.z ) * influence;

          p._vC.x += ( targets[ face.c ].x - v3p.x ) * influence;
          p._vC.y += ( targets[ face.c ].y - v3p.y ) * influence;
          p._vC.z += ( targets[ face.c ].z - v3p.z ) * influence;

        }

        v1.position.add( p._vA );
        v2.position.add( p._vB );
        v3.position.add( p._vC );

        projectVertex( p, v1 );
        projectVertex( p, v2 );
        projectVertex( p, v3 );

      }

      p._points3[ 0 ] = v1.positionScreen;
      p._points3[ 1 ] = v2.positionScreen;
      p._points3[ 2 ] = v3.positionScreen;

      RenderableFace* _face = nullptr;
      if ( v1.visible == true || v2.visible == true || v3.visible == true ||
           p._clipBox.isIntersectionBox( p._boundingBox.setFromPoints( p._points3 ) ) ) {

        visible = ( ( v3.positionScreen.x - v1.positionScreen.x ) *
                    ( v2.positionScreen.y - v1.positionScreen.y ) -
                    ( v3.positionScreen.y - v1.positionScreen.y ) *
                    ( v2.positionScreen.x - v1.positionScreen.x ) ) < 0;

        if ( side == THREE::DoubleSide || visible == ( side == THREE::FrontSide ) ) {

          _face = &p._faces.next();

          _face->id = object.id;
          _face->v1.copy( v1 );
          _face->v2.copy( v2 );
          _face->v3.copy( v3 );

        } else {

          continue;

        }

      } else {

        continue;

      }

      _face->normalModel.copy( face.normal );

      if ( visible == false && ( side == THREE::BackSide || side == THREE::DoubleSide ) ) {

        _face->normalModel.negate();

      }

      _face->normalModel.applyMatrix3( p._normalMatrix ).normalize();

      _face->normalModelView.copy( _face->normalModel ).applyMatrix3( p._normalViewMatrix );

      _face->centroidWorld.copy( face.centroid ).applyMatrix4( p._modelMatrix );

      auto& faceVertexNormals = face.vertexNormals;

      for ( size_t n = 0, nl = Math::min( (int)faceVertexNormals.size(), 3 ); n < nl; ++n ) {

        auto& normalModel = _face->vertexNormalsModel[ n ];
        normalModel.copy( faceVertexNormals[ n ] );

        if ( visible == false && ( side == THREE::BackSide || side == THREE::DoubleSide ) ) {

          normalModel.negate();

        }

        normalModel.applyMatrix3( p._normalMatrix ).normalize();

        auto& normalModelView = _face->vertexNormalsModelView[ n ];
        normalModelView.copy( normalModel ).applyMatrix3( p._normalViewMatrix );

      }

      _face->vertexNormalsLength = (int)faceVertexNormals.size();

      for ( int c = 0, cl = face.size(); c < cl; c ++ ) {

        const auto& uvs = faceVertexUvs[ c ][ f ];

        for ( size_t u = 0, ul = uvs.size(); u < ul; u ++ ) {

          _face->uvs[ c ][ u ] = uvs[ u ];

        }

      }

      _face->color = face.color;
      _face->material = material.get();

      p._centroid.copy( _face->centroidWorld ).applyProjection( p._viewProjectionMatrix );

      _face->z = p._centroid.z;

      // TODO "JD: FIX!!! EA: Uhhh"

      p._renderData.elements.push_back( _face );

    }
  }

  virtual void operator()( Line& object ) {

    p._modelViewProjectionMatrix.multiplyMatrices( p._viewProjectionMatrix, modelMatrix );

    auto& vertices = object.geometry->vertices;

    auto& v1 = p._vertices.next();
    v1.positionScreen.copy( vertices[ 0 ] ).applyMatrix4( p._modelViewProjectionMatrix );

    // Handle LineStrip and LinePieces
    auto step = object.lineType == THREE::LinePieces ? 2 : 1;

    for ( size_t v = 1, vl = vertices.size(); v < vl; v ++ ) {

      auto& v1 = p._vertices.next();
      v1.positionScreen.copy( vertices[ v ] ).applyMatrix4( p._modelViewProjectionMatrix );

      if ( ( v + 1 ) % step > 0 ) continue;

      auto& v2 = p._vertices.pool[ p._vertices.count - 2 ];

      p._clippedVertex1PositionScreen.copy(v1.positionScreen);
      p._clippedVertex2PositionScreen.copy(v2.positionScreen);

      if ( three::detail::clipLine( p._clippedVertex1PositionScreen, p._clippedVertex2PositionScreen ) ) {

        // Perform the perspective divide
        p._clippedVertex1PositionScreen.multiplyScalar( 1.f / p._clippedVertex1PositionScreen.w );
        p._clippedVertex2PositionScreen.multiplyScalar( 1.f / p._clippedVertex2PositionScreen.w );

        auto& _line = p._lines.next();
        _line.id = object.id;
        _line.v1.positionScreen.copy( p._clippedVertex1PositionScreen );
        _line.v2.positionScreen.copy( p._clippedVertex2PositionScreen );

        _line.z = Math::max( p._clippedVertex1PositionScreen.z, p._clippedVertex2PositionScreen.z );

        _line.material = object.material.get();

        if ( object.material->vertexColors == THREE::VertexColors ) {

          _line.vertexColors[ 0 ].copy( object.geometry->colors[ v ] );
          _line.vertexColors[ 1 ].copy( object.geometry->colors[ v - 1 ] );

        }

        p._renderData.elements.push_back( &_line );

      }
    }
  }

  void projectVertex( Projector::Impl& impl, RenderableVertex& vertex ) {

    auto& position = vertex.position;
    auto& positionWorld = vertex.positionWorld;
    auto& positionScreen = vertex.positionScreen;

    positionWorld.copy( position ).applyMatrix4( impl._modelMatrix );
    positionScreen.copy( positionWorld ).applyMatrix4( impl._viewProjectionMatrix );

    auto invW = 1.f / positionScreen.w;

    positionScreen.x *= invW;
    positionScreen.y *= invW;
    positionScreen.z *= invW;

    vertex.visible = positionScreen.x >= -1 && positionScreen.x <= 1 &&
                     positionScreen.y >= -1 && positionScreen.y <= 1 &&
                     positionScreen.z >= -1 && positionScreen.z <= 1;

  }

};

struct SpriteVisitor : public Visitor {

  SpriteVisitor( Projector::Impl& projectorIn, Camera& cameraIn )
    : projector( projectorIn ), camera( cameraIn ) { }

private:

  Projector::Impl& projector;
  Camera& camera;

  void operator()( Particle& object ) {

    const auto& modelMatrix = object.matrixWorld;
    auto& vector4 = projector._vector4;

    vector4.set( modelMatrix.elements[12], modelMatrix.elements[13], modelMatrix.elements[14], 1 );
    vector4.applyMatrix4( projector._viewProjectionMatrix );

    float invW = 1 / vector4.w;

    vector4.z *= invW;

    if ( vector4.z > 0 && vector4.z < 1 ) {

      auto& _particle = projector._particles.next();
      _particle.object = &object;
      _particle.x = vector4.x / vector4.w;
      _particle.y = vector4.y / vector4.w;
      _particle.z = vector4.z;

      _particle.rotation = object.rotation().z;

      _particle.scale.x = object.scale().x * Math::abs( _particle.x - ( vector4.x + camera.projectionMatrix.elements[0] ) / ( vector4.w + camera.projectionMatrix.elements[12] ) );
      _particle.scale.y = object.scale().y * Math::abs( _particle.y - ( vector4.y + camera.projectionMatrix.elements[5] ) / ( vector4.w + camera.projectionMatrix.elements[13] ) );

      _particle.material = object.material.get();

      projector._renderData.elements.push_back( &_particle );

    }

  }

};

//

} // namespace detail

Projector::Projector() : impl( new Impl() ) { }

Vector3& Projector::projectVector( Vector3& vector, const Camera& camera ) {

  auto& d = *impl;

  camera.matrixWorldInverse.getInverse( camera.matrixWorld );

  d._viewProjectionMatrix.multiplyMatrices( camera.projectionMatrix, camera.matrixWorldInverse );

  return vector.applyProjection( d._viewProjectionMatrix );

}

Vector3& Projector::unprojectVector( Vector3& vector, const Camera& camera ) {

  auto& d = *impl;

  auto projectionMatrixInverse = Matrix4();

  projectionMatrixInverse.getInverse( camera.projectionMatrix );
  d._viewProjectionMatrix.multiplyMatrices( camera.matrixWorld, projectionMatrixInverse );

  return vector.applyProjection( d._viewProjectionMatrix );
}

Ray Projector::pickingRay( Vector3 vector, const Camera& camera ) {

  // set two vectors with opposing z values
  vector.z = -1.0f;
  auto end = Vector3( vector.x, vector.y, 1.0 );

  unprojectVector( vector, camera );
  unprojectVector( end, camera );

  // find direction from vector to end
  end.sub( vector ).normalize();

  return Ray( vector, end );

}

Projector::RenderData& Projector::projectGraph( Object3D& root, bool sort ) {

  auto& d = *impl;

  d._objects.reset();

  d._renderData.objects.clear();
  d._renderData.sprites.clear();
  d._renderData.lights.clear();

  std::function<void( Object3D& )> projectObject;
  projectObject = [&projectObject, &d]( Object3D & object ) {

    if ( !object.visible ) return;

    if ( ( object.type() == THREE::Mesh || object.type() == THREE::Line ) &&
         ( !object.frustumCulled || d._frustum.intersectsObject( object ) ) ) {

      auto& vector3 = d._vector3;

      vector3.copy( object.matrixWorld.getPosition() );
      d._viewProjectionMatrix.multiplyVector3( vector3 );

      auto& renderable = d._objects.next();
      renderable.object = &object;
      renderable.z = vector3.z;

      d._renderData.objects.push_back( renderable );

    } else if ( object.type() == THREE::Sprite || object.type() == THREE::Particle ) {

      auto& vector3 = d._vector3;

      vector3.copy( object.matrixWorld.getPosition() );
      d._viewProjectionMatrix.multiplyVector3( vector3 );

      auto& renderable = d._objects.next();
      renderable.object = &object;
      renderable.z = vector3.z;

      d._renderData.sprites.push_back( renderable );

    } else if ( object.type() == THREE::Light ) {

      d._renderData.lights.push_back( &object );

    }

    for ( auto& child : object.children ) {

      projectObject( *child );

    }

  };

  projectObject( root );

  if ( sort ) {
    std::sort( d._renderData.objects.begin(),
               d._renderData.objects.end(),
               PainterSort() );
  }

  return d._renderData;

}

Projector::RenderData& Projector::projectScene( Scene& scene, Camera& camera, bool sortObjects, bool sortElements ) {

  auto& d = *impl;

  d._faces.reset();
  d._lines.reset();
  d._particles.reset();

  d._renderData.elements.clear();

  if ( scene.autoUpdate == true ) scene.updateMatrixWorld();
  if ( camera.parent == nullptr ) camera.updateMatrixWorld();

  d._viewMatrix.copy( camera.matrixWorldInverse.getInverse( camera.matrixWorld ) );
  d._viewProjectionMatrix.multiplyMatrices( camera.projectionMatrix, camera.matrixWorldInverse );

  d._normalViewMatrix.getNormalMatrix( d._viewMatrix );

  d._frustum.setFromMatrix( d._viewProjectionMatrix );

  d._renderData = projectGraph( scene, sortObjects );

  for ( auto& renderObject : d._renderData.objects ) {

    auto& object = *renderObject.object;

    auto& modelMatrix = object.matrixWorld;

    d._vertices.reset();

    detail::SceneVisitor visitor( d, camera, modelMatrix, d._viewProjectionMatrix );

    object.visit( visitor );

  }

  for ( auto& renderSprite : d._renderData.sprites ) {

    auto& object = *renderSprite.object;

    if(object.type() == THREE::Particle ) {

      detail::SpriteVisitor visitor( d, camera );

      object.visit( visitor );

    }

  }

  if ( sortElements ) {
    std::sort( d._renderData.elements.begin(),
               d._renderData.elements.end(),
               PainterSort() );
  }

  return d._renderData;

}


} // namespace three
