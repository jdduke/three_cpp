#ifndef THREE_INTERFACES_HPP
#define THREE_INTERFACES_HPP

#include <three/fwd.hpp>

namespace three {

class IFog {
public:
  typedef std::shared_ptr<IFog> Ptr;
  virtual THREE::FogType type() const = 0;
  virtual ~IFog() { }
};

class IGeometry {
public:
  virtual void applyMatrix( const Matrix4& ) = 0;
  virtual void computeBoundingBox()          = 0;
  virtual void computeBoundingSphere()       = 0;
  virtual void computeVertexNormals()        = 0;
  virtual void computeTangents()             = 0;
  virtual ~IGeometry() { }
};

class IPlugin {
public:
  typedef std::shared_ptr<IPlugin> Ptr;
  virtual void init( GLRenderer& renderer ) = 0;
  virtual void update( Scene& scene, Camera& camera ) = 0;
  virtual void render( Scene& scene, Camera& camera, int width, int height ) = 0;
  virtual ~IPlugin() { }
};

}

#endif // THREE_INTERFACES_HPP