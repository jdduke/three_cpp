#ifndef THREE_FOG_EXP2_HPP
#define THREE_FOG_EXP2_HPP

#include <three/common.hpp>

#include <three/math/color.hpp>

#include <three/utils/noncopyable.hpp>

namespace three {

class FogExp2 : public IFog, NonCopyable {

public:

  typedef std::shared_ptr<FogExp2> Ptr;

  static Ptr create( int hex, float density = 0.00025f ) {

    return three::make_shared<FogExp2>( hex, density );

  }

  virtual enums::FogType type() const { return enums::FogExp2; }

  Ptr clone() {

    return FogExp2::create( color.getHex(), density );
    
  }

  std::string name;

  Color color;
  float density;

protected:

   FogExp2( int hex, float density )
    : IFog(), color( hex ), density( density ) { }

};

} // namespace three

#endif // THREE_FOG_EXP2_HPP