#ifndef THREE_OBJECT3D_H
#define THREE_OBJECT3D_H

#include <three/common.h>

#include <three/core/geometry.h>
#include <three/core/event_dispatcher.h>

#include <three/math/vector3.h>
#include <three/math/quaternion.h>
#include <three/math/euler.h>
#include <three/math/matrix3.h>
#include <three/math/matrix4.h>

#include <three/materials/material.h>

#include <three/utils/memory.h>
#include <three/utils/noncopyable.h>
#include <three/utils/macros.h>

#include <three/visitor.h>

#include <algorithm>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace three {

class THREE_DECL Object3D : public DefaultEventDispatcher, NonCopyable {
public:

  typedef std::shared_ptr<Object3D> Ptr;

  static Ptr create() { return three::make_shared<Object3D>(); }

  virtual ~Object3D();

  virtual THREE::Type type() const { return THREE::Object3D; }

  virtual void visit( Visitor& v ) { }
  virtual void visit( ConstVisitor& v ) const { }
  virtual void visit( ConstRawPointerVisitor& v ) const { }

  unsigned int id;

  std::string uuid;

  std::string name;

  Object3D* parent;
  std::vector<Object3D::Ptr> children;

  float renderDepth;

  bool rotationAutoUpdate;

  Matrix4 matrix;
  Matrix4 matrixWorld;

  bool matrixAutoUpdate;
  bool matrixWorldNeedsUpdate;

  bool visible;

  bool castShadow;
  bool receiveShadow;

  bool frustumCulled;

  // TODO
  //this.userdata = {};

  inline const Vector3& position() const { return _position; }
  inline Vector3& position() { return _position; }

  inline const Vector3& scale() const { return _scale; }
  inline Vector3& scale() { return _scale; }

  inline const Vector3& up() const { return _up; }
  inline Vector3& up() { return _up; }

  inline const Euler& rotation() const { return _transform.rotation(); }
  inline Euler& rotation() { return _transform.rotation(); }

  inline const Quaternion& quaternion() const { return _transform.quaternion(); }
  inline Quaternion& quaternion() { return _transform.quaternion(); }

  Object3D& applyMatrix( Matrix4& m );

  Object3D& setRotationFromAxisAngle( Vector3& axis, float angle );

  Object3D& setRotationFromEuler( Euler& euler );

  Object3D& setRotationFromMatrix( Matrix4& m );

  Object3D& setRotationFromQuaternion( Quaternion& q );

  Object3D& rotateOnAxis( const Vector3& axis, float angle );

  Object3D& rotateX( float angle );

  Object3D& rotateY( float angle );

  Object3D& rotateZ( float angle );

  Object3D& translateOnAxis( const Vector3& axis, float distance );

  Object3D& translateX( float distance );

  Object3D& translateY( float distance );

  Object3D& translateZ( float distance );

  Vector3& localToWorld( Vector3& vector ) const;

  Vector3& worldToLocal( Vector3& vector ) const;

  virtual void lookAt( const Vector3& vector );

  void add( const Ptr& object );

  void remove( const Ptr& object );

  Object3D& traverse( const std::function<void(const Object3D&)> traverseCallback );

  Object3D::Ptr getObjectById( unsigned int id, bool recursive = true ) const;

  Object3D::Ptr getObjectByName( const std::string& name, bool recursive );

  std::vector<Object3D::Ptr>& getDescendants( std::vector<Object3D::Ptr>& array ) const;

  Object3D& updateMatrix();
  Object3D& updateMatrixWorld( bool force = false );

  Ptr clone( bool recursive = true ) const;

  bool sortParticles;

  bool useVertexTexture;
  std::vector<Bone*> bones;
  std::vector<Matrix4> boneMatrices;
  Texture::Ptr boneTexture;
  int boneTextureWidth, boneTextureHeight;

  int morphTargetBase;
  std::vector<int> morphTargetForcedOrder;

  Material::Ptr material;
  Geometry::Ptr geometry;

  struct THREE_DECL GLData {
    GLData() : __glInit( false ), __glActive( false ) { }

    bool __glInit;
    bool __glActive;
    Matrix4 _modelViewMatrix;
    Matrix3 _normalMatrix;

    // Obsolete?
    std::array<float, 16> _normalMatrixArray;
    std::array<float, 16> _modelViewMatrixArray;
    std::array<float, 16> _modelMatrixArray;
    std::vector<int>   __glMorphTargetInfluences;

    void clear() {
      __glInit = false;
      __glActive = false;
      _modelViewMatrix.identity();
      _normalMatrix.identity();
      __glMorphTargetInfluences.clear();
    }
  } glData;

  struct THREE_DECL ImmediateGLData {
    ImmediateGLData()
      : count( 0 ),
        hasPositions( false ), hasNormals( false ), hasUvs( false ), hasColors( false ),
        __glVertexBuffer( 0 ), __glNormalBuffer( 0 ), __glUvBuffer( 0 ), __glColorBuffer( 0 ) { }

    int count;
    bool hasPositions, hasNormals, hasUvs, hasColors;
    Buffer __glVertexBuffer, __glNormalBuffer, __glUvBuffer, __glColorBuffer;
    std::vector<float> positionArray, normalArray, uvArray, colorArray;
  } glImmediateData;

  typedef std::function<void( const Program*, void*, const Frustum* )> RenderCallback;
  RenderCallback immediateRenderCallback;

  void render( const std::function<void( Object3D& )> renderCallback );

protected:

  Object3D( const Material::Ptr& material = Material::Ptr(),
            const Geometry::Ptr& geometry = Geometry::Ptr() );

  // Fallback implementation. Used in scene
  virtual void __addObject( const Ptr& object );
  virtual void __removeObject( const Ptr& object );
  virtual void __clone( Ptr& cloned, bool recursive ) const;

private:

  class THREE_DECL SyncedEulerQuaternion {
  public:
    SyncedEulerQuaternion();

    Euler& rotation();
    const Euler& rotation() const;

    Quaternion& quaternion();
    const Quaternion& quaternion() const;

  private:

    enum class LastUpdatedRotationType {
        None,
        Skip,
        Euler,
        Quaternion,
    } mutable _lastUpdated;
      
    void updateBeforeRotation() const;
    void updateBeforeQuaternion() const;

    mutable Euler _rotation;
    mutable Euler _prevRotation;
    mutable Quaternion _quaternion;
    mutable Quaternion _prevQuaternion;
      
  } _transform;

 



  Vector3 _up;
  Vector3 _position;
  Vector3 _scale;

};

} // namespace three

#endif // THREE_OBJECT3D_H
