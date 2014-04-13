#ifndef THREE_SDL_H
#define THREE_SDL_H

#include <three/gl.h>
#include <three/math/color.h>
#include <three/utils/noncopyable.h>
#include <three/renderers/renderer_parameters.h>

#include <SDL_main.h>
#include <SDL_events.h>

#include <functional>

namespace three {
namespace sdl {

typedef SDL_Event                         Event;
typedef SDL_EventType                     EventType;
typedef std::pair<SDL_EventType, int>     EventKey;
typedef std::function<void(const Event&)> EventListener;

bool init( RendererParameters& parameters );
bool swapBuffers();
void quit();

EventKey addEventListener( EventType, EventListener );
void removeEventListener( EventKey );
void clearEventListeners();
void processEvents();

class ScopedEventListener : public NonCopyable {
public:
  ScopedEventListener( EventType, EventListener );
  ~ScopedEventListener();
private:
  EventKey eventKey;
};

} // namespace sdl
} // namespace three

#endif // THREE_SDL_H
