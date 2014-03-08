#ifndef THREE_SDL_HPP
#define THREE_SDL_HPP

#include <three/gl.hpp>
#include <three/math/color.hpp>
#include <three/utils/noncopyable.hpp>
#include <three/renderers/renderer_parameters.hpp>

#include <SDL_main.h>
#include <SDL_events.h>

#include <functional>

namespace three {
namespace sdl {

typedef SDL_Event                         Event;
typedef SDL_EventType                     EventType;
typedef std::pair<SDL_EventType, int>     EventKey;
typedef std::function<void(const Event&)> EventListener;

/////////////////////////////////////////////////////////////////////////

bool init( RendererParameters& parameters );
bool swapBuffers();
void quit();

/////////////////////////////////////////////////////////////////////////

EventKey addEventListener( EventType, EventListener );
void removeEventListener( EventKey );
void clearEventListeners();
void processEvents();

/////////////////////////////////////////////////////////////////////////

class ScopedEventListener : public NonCopyable {
public:
  ScopedEventListener( EventType, EventListener );
  ~ScopedEventListener();
private:
  EventKey eventKey;
};

} // namespace sdl
} // namespace three

#if defined(THREE_HEADER_ONLY)
# include <three/extras/impl/sdl.ipp>
#endif // defined(THREE_HEADER_ONLY)

#endif // THREE_SDL_HPP
