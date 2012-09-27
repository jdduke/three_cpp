#ifndef THREE_OBJECT3D_HPP
#define THREE_OBJECT3D_HPP

#include <three/common.hpp>
#include <three/utils.hpp>

#include <three/core/bone.hpp>
#include <three/core/vector3.hpp>
#include <three/core/quaternion.hpp>
#include <three/core/matrix4.hpp>

#include <three/materials/material.hpp>
#include <three/core/geometry.hpp>

#include <three/extras/noncopyable.hpp>

#include <algorithm>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace three {

class Object3D : NonCopyable {
public:

  typedef std::shared_ptr<Object3D> Ptr;

  /////////////////////////////////////////////////////////////////////////

  virtual THREE::Type type() const { return THREE::Object3D; }

  virtual void visit( Visitor& v ) = 0;
  virtual void visit( ConstVisitor& v ) const = 0;

  virtual ~Object3D() { }

  /////////////////////////////////////////////////////////////////////////

  int id;

  std::string name;

  //properties = {};

  Object3D* parent;
  std::vector<Object3D::Ptr> children;

  Vector3 up;

  Vector3 position;
  Vector3 rotation;
  THREE::Order eulerOrder;
  Vector3 scale;

  float renderDepth;

  bool rotationAutoUpdate;

  Matrix4 matrix;
  Matrix4 matrixWorld;
  mutable Matrix4 matrixRotationWorld;

  bool matrixAutoUpdate;
  bool matrixWorldNeedsUpdate;

  Quaternion quaternion;
  bool useQuaternion;

  float boundRadius;
  float boundRadiusScale;

  bool visible;

  bool castShadow;
  bool receiveShadow;

  bool frustumCulled;

  bool sortParticles;

  bool useVertexTexture;
  std::vector<Bone> bones;
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
    Matrix4 _normalMatrix;

    std::vector<float> _normalMatrixArray;
    std::vector<float> _modelViewMatrixArray;
    std::vector<float> _modelMatrixArray;
    std::vector<int>   __glMorphTargetInfluences;

    void clear() {
      __glInit = false;
      __glActive = false;
      _modelViewMatrix.identity();
      _normalMatrix.identity();
      _normalMatrixArray.clear();
      _modelViewMatrixArray.clear();
      _modelMatrixArray.clear();
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
  } glImmediateData ;

  typedef std::function<void( const Program&, void*, const Frustum& )> RenderCallback;
  RenderCallback immediateRenderCallback;

  /////////////////////////////////////////////////////////////////////////

  THREE_DECL void applyMatrix( const Matrix4& m );
  THREE_DECL void translate( float distance, Vector3 axis );
  THREE_DECL void translateX( float distance );
  THREE_DECL void translateY( float distance );
  THREE_DECL void translateZ( float distance );
  THREE_DECL void lookAt( const Vector3& vector );

  THREE_DECL void add( Ptr object );
  THREE_DECL void remove( Ptr object );

  THREE_DECL Ptr getChildByName( const std::string& name, bool recursive );

  THREE_DECL void updateMatrix();
  THREE_DECL void updateMatrixWorld( bool force = false );

  THREE_DECL Vector3 worldToLocal( const Vector3& vector );
  THREE_DECL Vector3 localToWorld( const Vector3& vector );

  THREE_DECL void render( const std::function<void( Object3D& )> renderCallback );

protected:

  THREE_DECL Object3D( Material::Ptr material = Material::Ptr(),
            Geometry::Ptr geometry = Geometry::Ptr() );

  THREE_DECL virtual void __addObject( Ptr& object );
  THREE_DECL virtual void __removeObject( Ptr& object );

private:

  static int& Object3DCount() {
    static int sObject3DCount = 0;
    return sObject3DCount;
  }

};

} // namespace three

#if defined(THREE_HEADER_ONLY)
# include <three/core/impl/object3d.ipp>
#endif // defined(THREE_HEADER_ONLY)

#endif // THREE_OBJECT3D_HPP