#ifndef THREE_EXAMPLES_SDL_H
#define THREE_EXAMPLES_SDL_H

#include "three/gl.h"
#include "three/math/color.h"
#include "three/utils/noncopyable.h"
#include "three/renderers/renderer_parameters.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>

#include <functional>
#include <map>
#include <vector>

namespace three_examples {

class GLWindow : public three::NonCopyable {
public:
  typedef SDL_Event                         Event;
  typedef SDL_EventType                     EventType;
  typedef std::function<void(const Event&)> EventListener;

  GLWindow( const three::RendererParameters& );
  ~GLWindow();

  three::GLInterface createGLInterface();

  void addEventListener( EventType, EventListener );

  typedef std::function<bool(float)> Update;
  void animate( Update update );

  //void animate( Update update, Render render, float frameRate = 60 );

  bool valid() const;

private:
  void swapBuffers();
  bool processEvents();

  SDL_Window* window;
  SDL_GLContext context;
  std::map<EventType, std::vector<EventListener>> listeners;
  bool renderStats;
};

} // namespace three_examples

#endif // THREE_EXAMPLES_SDL_H
