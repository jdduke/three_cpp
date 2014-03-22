#ifndef THREE_SDL_CPP
#define THREE_SDL_CPP

#include <three/extras/sdl.h>

#include <three/console.h>
#include <three/renderers/renderer_parameters.h>

#include <SDL.h>
#include <SDL_opengl.h>

#include <array>
#include <map>

namespace three {
namespace sdl {

bool init( RendererParameters& parameters ) {

  //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

  if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
    console().error() << "Error initializing SDL: " << SDL_GetError();
    return false;
  }

  /*
  if ( parameters.antialias ) {
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
  }
  */

  console().log() << "SDL initialized";

  int videoFlags = SDL_OPENGL | SDL_RESIZABLE;
  if ( SDL_SetVideoMode( parameters.width,
                         parameters.height,
                         parameters.precision,
                         videoFlags ) == NULL ) {
    console().error() << "Error setting SDL video mode: " << SDL_GetError();
    return false;
  }

  //SDL_GL_SetSwapInterval( parameters.vsync ? 1 : 0 );

  console().log() << "SDL video initialized";

  SDL_WM_SetCaption( "three.cpp", NULL );

  return true;
}

bool swapBuffers() {
  SDL_GL_SwapBuffers();
  return true;
}

void quit() {
  SDL_Quit();
}

/////////////////////////////////////////////////////////////////////////

namespace detail {

struct EventHandler {
  static EventHandler& instance() {
    static EventHandler sEventHandler;
    return sEventHandler;
  }

  void process() {
    SDL_Event event;
    while ( SDL_PollEvent( &event ) ) {
      processEvent( event );
    }
  }

  EventKey addListener( EventType eventType, EventListener eventListener ) {
    int eventTypeIndex = listenerCounts[eventType]++;
    listeners[ eventType ].insert( std::make_pair(eventTypeIndex, std::move(eventListener)) );
    return EventKey( eventType, eventTypeIndex );
  }

  void removeListener( EventKey eventKey ) {
    auto eventTypeIt = listeners.find( eventKey.first );
    if ( eventTypeIt != listeners.end() ) {
      eventTypeIt->second.erase( eventKey.second );
    }
  }

  void clear() {
    listeners.clear();
    listenerCounts.fill( 0 );
  }

private:
  EventHandler() {
    listenerCounts.fill( 0 );
  }

  void processEvent( const SDL_Event& event ) {

    auto eventTypeIt = listeners.find( event.type );
    if ( eventTypeIt == listeners.end() ) {
      return;
    }

    for ( auto& listener : eventTypeIt->second ) {
      listener.second( event );
    }

  }

  std::map<int, std::map<int, EventListener>> listeners;
  std::array<int, SDL_NUMEVENTS> listenerCounts;
};

} // namespace detail

EventKey addEventListener( EventType eventType, EventListener eventListener ) {
  return detail::EventHandler::instance().addListener( eventType, std::move(eventListener) );
}

void removeEventListener( EventKey eventKey ) {
  detail::EventHandler::instance().removeListener( eventKey );
}

void clearEventListeners() {
  detail::EventHandler::instance().clear();
}

void processEvents() {
  detail::EventHandler::instance().process();
}

/////////////////////////////////////////////////////////////////////////

ScopedEventListener::ScopedEventListener( EventType type, EventListener listener )
  : eventKey( addEventListener( type, std::move(listener) ) ) { }

ScopedEventListener::~ScopedEventListener() {
  removeEventListener( eventKey );
}

} // namespace sdl
} // namespace three

#endif // THREE_SDL_H