#ifndef THREE_FOG_H
#define THREE_FOG_H

#include <three/common.h>

#include <three/math/color.h>

#include <three/utils/noncopyable.h>

namespace three {

class Fog : public IFog, NonCopyable {

public:

  typedef std::shared_ptr<Fog> Ptr;

  static Ptr create( int hex, float near = 1, float far = 1000 ) {

    return three::make_shared<Fog>( hex, near, far );

  }

  virtual enums::FogType type() const THREE_OVERRIDE { return enums::Fog; }

  virtual IFog::Ptr clone() const THREE_OVERRIDE {

    return Fog::create(color.getHex(), near, far);

  }

  std::string name;

  Color color;

  float near;
  float far;

protected:

  Fog( int hex, float near, float far )
    : IFog(), color( hex ), near( near ), far( far ) { }

};

} // namespace three

#endif // THREE_FOG_H