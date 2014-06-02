#ifndef THREE_EXAMPLES_SDL_H
#define THREE_EXAMPLES_SDL_H

#include "three/gl.h"
#include "three/math/color.h"
#include "three/utils/noncopyable.h"
#include "three/core/event_dispatcher.h"
#include "three/renderers/renderer_parameters.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>

#include <functional>
#include <map>
#include <vector>

namespace three_examples {

using namespace three;
    
typedef SDL_Event                         SdlEvent;
typedef SDL_EventType                     SdlEventType;

typedef three::EventDispatcher<unsigned int, SdlEvent> SdlEventDispatcher;
typedef three::EventListener<SdlEvent> SdlEventListener;
    
class GLWindow : public three::NonCopyable, public SdlEventDispatcher {
public:
  

  GLWindow( const three::RendererParameters& );
  ~GLWindow();

  three::GLInterface createGLInterface();

  typedef std::function<bool(float)> Update;
  void animate( Update update );

  bool valid() const;

private:
  void swapBuffers();
  bool processEvents();

  SDL_Window* window;
  SDL_GLContext context;
  bool renderStats;
};



} // namespace three_examples

#endif // THREE_EXAMPLES_SDL_H
