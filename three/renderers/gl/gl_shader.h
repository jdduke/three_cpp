#ifndef THREE_GL_SHADER_H
#define THREE_GL_SHADER_H

#include <three/materials/uniform.h>

namespace three {

class Shader {

public:
	
  Shader( Uniforms uniforms, std::string vertexShader, std::string fragmentShader )
    : uniforms( std::move( uniforms ) ), 
      vertexShader( std::move( vertexShader ) ), 
      fragmentShader( std::move( fragmentShader ) ) { }

  Uniforms uniforms;

  std::string vertexShader, fragmentShader;

};

} // namespace three

#endif // THREE_GL_SHADER_H