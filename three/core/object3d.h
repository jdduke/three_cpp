#ifndef THREE_OBJECT3D_H
#define THREE_OBJECT3D_H

#include <three/common.h>


#include <three/math/vector3.h>
#include <three/math/quaternion.h>
#include <three/math/euler.h>
#include <three/math/matrix3.h>
#include <three/math/matrix4.h>

#include <three/materials/material.h>
#include <three/core/geometry.h>

#include <three/utils/memory.h>
#include <three/utils/noncopyable.h>
#include <three/utils/macros.h>

#include <algorithm>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace three {

class Object3D : NonCopyable {
public:

  typedef std::shared_ptr<Object3D> Ptr;

  static Ptr create() {
    return three::make_shared<Object3D>();
  }

  virtual enums::Type type() const {
    return enums::Object3D;
  }
  virtual void visit( Visitor& v ) { };
  virtual void visit( ConstVisitor& v ) const { };
  virtual void visit( ConstRawPointerVisitor& v ) const { };
  virtual ~Object3D();

  int id;

  std::string uuid;

  std::string name;

  Object3D* parent;
  std::vector<Object3D::Ptr> children;

  Vector3 up;

  Vector3 position;
  Euler _rotation;
  THREE_TODO("Private?")
  Quaternion _quaternion;
  THREE_TODO("Private?")
  Vector3 scale;

  // keep rotation and quaternion in sync

  //this._rotation._quaternion = this.quaternion;
  //this._quaternion._euler = this.rotation;

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

  bool sortParticles;

  bool useVertexTexture;
  THREE_TODO("Bone shouldnt be a pointer") std::vector<Bone*> bones;
  std::vector<Matrix4> boneMatrices;
  Texture::Ptr boneTexture;
  int boneTextureWidth, boneTextureHeight;

  int morphTargetBase;
  std::vector<int> morphTargetForcedOrder;

  Material::Ptr material;
  Geometry::Ptr geometry;

  struct GLData {
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
      //_normalMatrixArray.clear();
      //_modelViewMatrixArray.clear();
      //_modelMatrixArray.clear();
      __glMorphTargetInfluences.clear();
    }
  } glData;

  struct ImmediateGLData {
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

  virtual void lookAt( const Vector3& vector );
  void applyMatrix( Matrix4& m );
  void translate( float distance, Vector3 axis );
  void translateX( float distance );
  void translateY( float distance );
  void translateZ( float distance );

  void add( const Ptr& object );
  void remove( const Ptr& object );

  void traverse( const std::function<void(const Object3D&)> traverseCallback );

  Ptr getChildByName( const std::string& name, bool recursive );
  std::vector<Object3D::Ptr>& getDescendants( std::vector<Object3D::Ptr>& array ) const;

  void updateMatrix();
  void updateMatrixWorld( bool force = false );

  Vector3 worldToLocal( Vector3& vector ) const;
  Vector3 localToWorld( Vector3& vector ) const;

  void render( const std::function<void( Object3D& )> renderCallback );

protected:

  Object3D( const Material::Ptr& material = Material::Ptr(),
            const Geometry::Ptr& geometry = Geometry::Ptr() );

  virtual void __addObject( const Ptr& object );
  virtual void __removeObject( const Ptr& object );

private:

  static int& Object3DIdCount() {
    static int sObject3DIdCount = 0;
    return sObject3DIdCount;
  }

};

} // namespace three

#endif // THREE_OBJECT3D_H