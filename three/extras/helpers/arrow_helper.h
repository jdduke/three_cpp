#ifndef THREE_EXTRAS_ARROW_HELPER_H
#define THREE_EXTRAS_ARROW_HELPER_H

#include <three/core/object3d.h>
#include <three/extras/geometries/cylinder_geometry.h>
#include <three/objects/mesh.h>
#include <three/objects/line.h>

namespace three {

class ArrowHelper : public Object3D {
public:

  typedef std::shared_ptr<ArrowHelper> Ptr;

  static Ptr create( const Vector3& dir,
                     const Vector3& origin,
                     float length = 50,
                     int hex = 0xffff00,
                     float headLength = 10,
                     float headWidth = 2 );

  Line::Ptr line;
  Mesh::Ptr cone;

  void setDirection ( const Vector3& dir );

  void setLength ( float length );

  void setLength ( float length, float headLength );

  void setLength ( float length, float headLength, float headWidth );

  void setColor ( int hex );

protected:

  ArrowHelper( const Vector3& dir,
               const Vector3& origin,
               float length ,
               int hex,
               float headLength,
               float headWidth );

};

} // end namespace three

#endif // THREE_EXTRAS_ARROW_HELPER_H