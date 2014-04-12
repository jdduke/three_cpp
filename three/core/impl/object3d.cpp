#ifndef THREE_OBJECT3D_CPP
#define THREE_OBJECT3D_CPP

#include <three/core/object3d.h>

#include <three/math/math.h>

#include <three/console.h>

namespace three {

Object3D& Object3D::applyMatrix( Matrix4& matrix ) {

  matrix.multiplyMatrices( matrix, this->matrix );

  matrix.decompose( position, _quaternion, scale );

  return *this;

}

Object3D& Object3D::setRotationFromAxisAngle( Vector3& axis, float angle ) {
  
  _quaternion.setFromAxisAngle( axis, angle );

  return *this;

}

Object3D& Object3D::setRotationFromEuler( Euler& euler ) {
  
  _quaternion.setFromEuler( euler, true );

  return *this;

}

Object3D& Object3D::setRotationFromMatrix( Matrix4& m ) {

  // assumes the upper 3x3 of m is a pure rotation matrix (i.e, unscaled)

  _quaternion.setFromRotationMatrix( m );

  return *this;

}

Object3D& Object3D::setRotationFromQuaternion( Quaternion& q ) {

    // assumes q is normalized

    _quaternion.copy( q );

    return *this;

}

Object3D& Object3D::rotateOnAxis( const Vector3& axis, float angle ) {

  // rotate object on axis in object space
  // axis is assumed to be normalized

  auto q1 = Quaternion();

  q1.setFromAxisAngle( axis, angle );

  _quaternion.multiply( q1 );

  return *this;

} 

Object3D& Object3D::rotateX( float angle ) {

  auto v1 = Vector3( 1, 0, 0 );

  return rotateOnAxis( v1, angle );

}

Object3D& Object3D::rotateY( float angle ) {

  auto v1 = Vector3( 0, 1, 0 );

  return rotateOnAxis( v1, angle );

}

Object3D& Object3D::rotateZ( float angle ) {

  auto v1 = Vector3( 0, 0, 1 );

  return rotateOnAxis( v1, angle );

}

Object3D& Object3D::translateOnAxis( const Vector3& axis, float distance ) {

  // translate object by distance along axis in object space
  // axis is assumed to be normalized

  auto v1 = Vector3();

  v1.copy( axis );

  v1.applyQuaternion( _quaternion );

  position.add( v1.multiplyScalar( distance ) );

  return *this;
  
}

Object3D& Object3D::translateX( float distance ) {
    
  auto v = Vector3( 1, 0, 0 );
  translateOnAxis( v, distance );
  
  return *this;

}

Object3D& Object3D::translateY( float distance ) {
    
  auto v = Vector3( 0, 1, 0 );
  translateOnAxis( v, distance );

  return *this;

}

Object3D& Object3D::translateZ( float distance ) {
    
  auto v = Vector3( 0, 0, 1 );
  translateOnAxis( v, distance );

  return *this;

}

Vector3& Object3D::localToWorld( Vector3& vector ) const {

  return vector.applyMatrix4( matrixWorld );

}

Vector3& Object3D::worldToLocal( Vector3& vector ) const {

  auto m1 = Matrix4();

  return vector.applyMatrix4( m1.getInverse( matrixWorld ) );
  
}

void Object3D::lookAt( const Vector3& vector ) {

  // This routine does not support objects with rotated and/or translated parent(s)
  // @todo priv member
  auto m1 = Matrix4();

  m1.lookAt( vector, position, up );

  _quaternion.setFromRotationMatrix( m1 );

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
  // TODO "EA: Dispatch event"

  //object.dispatchEvent( { type: 'added' } );
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
    
    // TODO "EA: Dispatch event"

    //object.dispatchEvent( { type: 'removed' } );

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

Object3D& Object3D::traverse( const std::function<void(const Object3D&)> traverseCallback ) {
  
  traverseCallback( *this );

  for ( auto it = children.begin(); it != children.end(); it++ ) {
    (*it)->traverse( traverseCallback );
  }

  return *this;

}

Object3D::Ptr Object3D::getObjectById( unsigned int id, bool recursive ) const {
  
  for ( size_t i = 0, l = children.size(); i < l; i ++ ) {

    auto& child = children[ i ];

    if ( child->id == id ) {

      return child;

    }

    if ( recursive == true ) {

      auto recursive_child = child->getObjectById( id, recursive );

      if ( recursive_child != nullptr ) {

        return recursive_child;

      }

    }

  }

  return nullptr;

}

Object3D::Ptr Object3D::getObjectByName( const std::string& name, bool recursive ) {

  for ( auto& child : children ) {

    if ( child->name == name ) {
      return child;
    }

    if ( recursive ) {

      auto recursive_child = child->getObjectByName( name, recursive );
      if ( recursive_child ) {
        return recursive_child;
      }

    }

  }

  return Object3D::Ptr();

}

std::vector<Object3D::Ptr>& Object3D::getDescendants(std::vector<Object3D::Ptr>& descendants) const {

  descendants.insert(descendants.end(), children.begin(), children.end());

  for ( auto& child : children ) {

    child->getDescendants( descendants );

  }

  return descendants;

}


Object3D& Object3D::updateMatrix() {

  matrix.compose( position, _quaternion, scale );

  matrixWorldNeedsUpdate = true;

  return *this;

}

Object3D& Object3D::updateMatrixWorld( bool force ) {

  if ( matrixAutoUpdate == true ) updateMatrix();

  if ( matrixWorldNeedsUpdate == true || force == true ) {

    if ( ! parent ) {

      matrixWorld.copy( matrix );

    } else {

      matrixWorld.multiplyMatrices( parent->matrixWorld, matrix );

    }

    matrixWorldNeedsUpdate = false;

    force = true;

    // update children

    for ( size_t i = 0, l = children.size(); i < l; i ++ ) {

      children[ i ]->updateMatrixWorld( force );

    }

  }

  // update children

  for ( auto& child : children ) {
    child->updateMatrixWorld( force );
  }

  return *this;

}

Object3D::Object3D( const Material::Ptr& material ,
                    const Geometry::Ptr& geometry )
  : id( Object3DIdCount()++ ),
    uuid( Math::generateUUID() ),
    parent( nullptr ),
    up( 0, 1, 0 ),
    scale( 1, 1, 1 ),
    renderDepth( 0 ),
    rotationAutoUpdate( true ),
    matrix( Matrix4() ),
    matrixWorld( Matrix4() ),
    matrixAutoUpdate( true ),
    matrixWorldNeedsUpdate( true ),
    visible( true ),
    castShadow( false ),
    receiveShadow( false ),
    frustumCulled( true ),
    useVertexTexture( false ),
    boneTextureWidth( 0 ),
    boneTextureHeight( 0 ),
    morphTargetBase( -1 ),
    material( material ),
    geometry( geometry ),
    _rotation( Euler() ),
    _quaternion( Quaternion() ) { 
      
      THREE_REVIEW("EA: Does this work/initialize properly?")

      if(_rotation._quaternion == nullptr) {
        _rotation._quaternion = make_shared<Quaternion>();
      }
      _rotation._quaternion->copy(quaternion());

      if(_quaternion._euler == nullptr) {
        _quaternion._euler = make_shared<Euler>();
      }
      _quaternion._euler->copy(rotation());
    }

Object3D::~Object3D() { }

void Object3D::__addObject( const Ptr& object ) { }

void Object3D::__removeObject( const Ptr& object ) { }

void Object3D::render( const std::function<void( Object3D& )> renderCallback ) {
  if ( renderCallback ) {
    renderCallback( *this );
  }
}

} // namespace three

#endif // THREE_OBJECT3D_CPP