#ifndef THREE_OBJECT3D_IPP
#define THREE_OBJECT3D_IPP

#include <three/core/object3d.hpp>

namespace three {

void Object3D::applyMatrix( const Matrix4& m ) {

  matrix.multiply( m, matrix );

  scale = matrix.getScale();

  auto mat = Matrix4().extractRotation( matrix );
  rotation = mat.getEulerRotation( eulerOrder );

  position = matrix.getPosition();

}

void Object3D::translate( float distance, Vector3 axis ) {
  matrix.rotateAxis( axis );
  position.addSelf( axis.multiplyScalar( distance ) );
}

void Object3D::translateX( float distance ) {
  translate( distance, Vector3( 1, 0, 0 ) );
}

void Object3D::translateY( float distance ) {
  translate( distance, Vector3( 0, 1, 0 ) );
}

void Object3D::translateZ( float distance ) {
  translate( distance, Vector3( 0, 0, 1 ) );
}

void Object3D::lookAt( const Vector3& vector ) {

  // TODO: Add hierarchy support.
  matrix.lookAt( vector, position, up );

  if ( rotationAutoUpdate ) {
    rotation = matrix.getEulerRotation( eulerOrder );
  }

}

void Object3D::add( const Object3D::Ptr& object ) {

  if ( !object )
    return;

  if ( object.get() == this ) {
    console().warn( "Three.Object3D.add: An object can't be added as a child of itself." );
    return;
  }

  if ( object->parent != nullptr ) {
    object->parent->remove( object );
  }

  object->parent = this;
  children.push_back( object );

  // add to scene

  auto scene = this;

  while ( scene->parent != nullptr ) {
    scene = scene->parent;
  }

  if ( scene != nullptr )  {
    scene->__addObject( object );
  }

}

void Object3D::remove( const Object3D::Ptr& object ) {

  auto index = std::find( children.begin(), children.end(), object );

  if ( index != children.end() ) {

    object->parent = nullptr;
    children.erase( index );

    // remove from scene

    auto scene = this;

    while ( scene->parent != nullptr ) {
      scene = scene->parent;
    }

    if ( scene != nullptr ) {
      scene->__removeObject( object );
    }

  }

}

Object3D::Ptr Object3D::getChildByName( const std::string& name, bool recursive ) {

  for ( auto& child : children ) {

    if ( child->name == name ) {
      return child;
    }

    if ( recursive ) {

      auto recursive_child = child->getChildByName( name, recursive );
      if ( recursive_child ) {
        return recursive_child;
      }

    }

  }

  return Object3D::Ptr();

}

void Object3D::updateMatrix() {

  matrix.setPosition( position );

  if ( useQuaternion )  {
    matrix.setRotationFromQuaternion( quaternion );
  } else {
    matrix.setRotationFromEuler( rotation, eulerOrder );
  }

  if ( scale.x != 1.f || scale.y != 1.f || scale.z != 1.f ) {
    matrix.scale( scale );
    boundRadiusScale = std::max( scale.x, std::max( scale.y, scale.z ) );
  }

  matrixWorldNeedsUpdate = true;

}

void Object3D::updateMatrixWorld( bool force /*= false*/ ) {

  if ( matrixAutoUpdate ) updateMatrix();

  if ( matrixWorldNeedsUpdate || force ) {

    if ( parent != nullptr ) {
      matrixWorld.multiply( parent->matrixWorld, matrix );
    } else {
      matrixWorld.copy( matrix );
    }

    matrixWorldNeedsUpdate = false;
    force = true;

  }

  // update children

  for ( auto& child : children ) {
    child->updateMatrixWorld( force );
  }

}

Vector3 Object3D::worldToLocal( const Vector3& vector ) {
  return Matrix4().getInverse( matrixWorld ).multiplyVector3( vector );
}

Vector3 Object3D::localToWorld( const Vector3& vector ) {
  return matrixWorld.multiplyVector3( vector );
}

void Object3D::render( const std::function<void( Object3D& )> renderCallback ) {
  if ( renderCallback ) {
    renderCallback( *this );
  }
}

/////////////////////////////////////////////////////////////////////////

Object3D::Object3D( const Material::Ptr& material /*= Material::Ptr()*/,
                    const Geometry::Ptr& geometry /*= Geometry::Ptr()*/ )
  : id( Object3DCount()++ ),
    parent( nullptr ),
    up( 0, 1, 0 ),
    eulerOrder( THREE::XYZ ),
    scale( 1, 1, 1 ),
    renderDepth( 0 ),
    rotationAutoUpdate( true ),
    matrixAutoUpdate( true ),
    matrixWorldNeedsUpdate( true ),
    useQuaternion( false ),
    boundRadius( 0.0f ),
    boundRadiusScale( 1.0f ),
    visible( true ),
    castShadow( false ),
    receiveShadow( false ),
    frustumCulled( true ),
    sortParticles( false ),
    useVertexTexture( false ),
    boneTextureWidth( 0 ),
    boneTextureHeight( 0 ),
    morphTargetBase( -1 ),
    material( material ),
    geometry( geometry ) { }

void Object3D::__addObject( const Ptr& object ) { }

void Object3D::__removeObject( const Ptr& object ) { }

} // namespace three

#endif // THREE_OBJECT3D_IPP