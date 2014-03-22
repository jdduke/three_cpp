#ifndef THREE_EVENT_TARGET_HPP
#define THREE_EVENT_TARGET_HPP

#include <three/common.hpp>

namespace three {

class EventTarget {
public:

  // TODO: Implement

  /*
    var listeners = {};

    template < typename T >
    void addEventListener ( type, listener ) {

      if ( listeners[ type ] === undefined ) {

        listeners[ type ] = [];

      }

      if ( listeners[ type ].indexOf( listener ) === - 1 ) {

        listeners[ type ].push( listener );

      }

    }

    void dispatchEvent ( event ) {

      for ( var listener in listeners[ event.type ] ) {

        listeners[ event.type ][ listener ]( event );

      }

    }

    void removeEventListener ( type, listener ) {

      var index = listeners[ type ].indexOf( listener );

      if ( index !== - 1 ) {

        listeners[ type ].splice( index, 1 );

      }

    }
  */

};

} // namespace three

#endif // THREE_EVENT_TARGET_HPP