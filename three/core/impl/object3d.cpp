#ifndef THREE_OBJECT3D_CPP
#define THREE_OBJECT3D_CPP

#include <three/core/object3d.h>

#include <three/console.h>

namespace three {

void Object3D::applyMatrix( Matrix4& matrix ) {
  matrix.multiplyMatrices( matrix, this->matrix );
  matrix.decompose( position, quaternion, scale );
}

void Object3D::translate( float distance, Vector3 axis ) {
  matrix.rotateAxis( axis );
  position.add( axis.multiplyScalar( distance ) );
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

  // This routine does not support objects with rotated and/or translated parent(s)
  // @todo priv member
  auto m1 = Matrix4();

  m1.lookAt( vector, position, up );

  quaternion.setFromRotationMatrix( m1 );

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

void Object3D::traverse( const std::function<void(const Object3D&)> traverseCallback ) {
  traverseCallback( *this );
  for ( auto it = children.begin(); it != children.end(); it++ ) {
    (*it)->traverse( traverseCallback );
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
  matrix.compose( position, quaternion, scale );
  matrixWorldNeedsUpdate = true;
}

void Object3D::updateMatrixWorld( bool force /*= false*/ ) {

  if ( matrixAutoUpdate == true ) updateMatrix();

  if ( matrixWorldNeedsUpdate == true || force == true ) {

    if ( parent ) {

      matrixWorld.copy( matrix );

    } else {

      matrixWorld.multiplyMatrices( parent->matrixWorld, matrix );

    }

    matrixWorldNeedsUpdate = false;

    force = true;

  }

  // update children

  for ( auto& child : children ) {
    child->updateMatrixWorld( force );
  }

}

Vector3 Object3D::worldToLocal( Vector3& vector ) const {
  return Matrix4().getInverse( matrixWorld ).multiplyVector3( vector );
}

Vector3 Object3D::localToWorld( Vector3& vector ) const {
  return matrixWorld.multiplyVector3( vector );
}

void Object3D::render( const std::function<void( Object3D& )> renderCallback ) {
  if ( renderCallback ) {
    renderCallback( *this );
  }
}

Object3D::Object3D( const Material::Ptr& material /*= Material::Ptr()*/,
                    const Geometry::Ptr& geometry /*= Geometry::Ptr()*/ )
  : id( Object3DCount()++ ),
    parent( nullptr ),
    up( 0, 1, 0 ),
    eulerOrder( enums::XYZ ),
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

Object3D::~Object3D() { }

void Object3D::__addObject( const Ptr& object ) { }

void Object3D::__removeObject( const Ptr& object ) { }

} // namespace three

#endif // THREE_OBJECT3D_CPP