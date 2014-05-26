#ifndef THREE_INTERFACES_H
#define THREE_INTERFACES_H

#include <three/fwd.h>
#include <three/constants.h>

#include <memory>

namespace three {

class IFog {
public:
  typedef std::shared_ptr<IFog> Ptr;
  virtual enums::FogType type() const = 0;
  virtual Ptr clone() const = 0;
  virtual ~IFog() { }
};

class THREE_DECL IGeometry {
public:
  virtual void applyMatrix( Matrix4& ) = 0;
  virtual void computeBoundingBox() = 0;
  virtual void computeBoundingSphere() = 0;
  virtual void computeVertexNormals( bool areaWeighted = false ) = 0;
  virtual void computeTangents() = 0;
  virtual ~IGeometry() { }
};

class THREE_DECL IPlugin {
public:
  typedef std::shared_ptr<IPlugin> Ptr;
  virtual void init( GLRenderer& renderer ) = 0;
  virtual void update( Scene& scene, Camera& camera ) = 0;
  virtual void render( Scene& scene, Camera& camera, int width, int height ) = 0;
  virtual ~IPlugin() { }
};

}

#endif // THREE_INTERFACES_H