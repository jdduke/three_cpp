#ifndef THREE_AMBIENT_LIGHT_H
#define THREE_AMBIENT_LIGHT_H

#include <three/common.h>

#include <three/lights/light.h>

namespace three {

class AmbientLight : public Light {

public:

  typedef std::shared_ptr<AmbientLight> Ptr;

  static Ptr create( int hex ) { 

  	return make_shared<AmbientLight>( hex ); 

  }

  virtual enums::Type type() const { 

  	return enums::AmbientLight; 

  }

  Ptr clone() {
  	
  	auto light = Light::clone();

  	return std::static_pointer_cast<AmbientLight>(light);
  	
  }

protected:

  AmbientLight( int hex ) 
  	: Light( hex ) {}

};

} // namespace three

#endif // THREE_AMBIENT_LIGHT_H