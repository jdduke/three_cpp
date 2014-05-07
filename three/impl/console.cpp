#include <three/console.h>

#include <memory>
#include <iostream>
#include <sstream>

namespace three {
namespace {

void stdcout( const char* msg ) { std::cout << msg << std::endl; }

} // namespace

Console& Console::instance() {
  static Console sConsole;
  return sConsole;
 }

Console::Console() : output( stdcout ) { }

void Console::info( const char* msg ) const  { info()  << msg; }
void Console::log( const char* msg ) const   { log()   << msg; }
void Console::debug( const char* msg ) const { debug() << msg; }
void Console::warn( const char* msg ) const  { warn()  << msg; }
void Console::error( const char* msg ) const { error() << msg; }

Console::LogProxy Console::info()  const { return LogProxy( output, "INFO:  " ); }
Console::LogProxy Console::log()   const { return LogProxy( output, "LOG:   " ); }
Console::LogProxy Console::debug() const { return LogProxy( output, "DEBUG: " ); }
Console::LogProxy Console::warn()  const { return LogProxy( output, "WARN:  " ); }
Console::LogProxy Console::error() const { return LogProxy( output, "ERROR: " ); }

void Console::setLog( LogP log ) { output = log; }

Console::LogProxy::~LogProxy() {
  if ( stream && log ) {
    log( stream->str().c_str() );
  }
}

Console::LogProxy::LogProxy( LogProxy && other )
  : log( nullptr ) {
  std::swap( log, other.log );
  std::swap( stream, other.stream );
}

Console::LogProxy::LogProxy( LogP log, const char* msg )
  : stream( new std::stringstream() ), log( log ) {
  if ( msg ) {
    ( *this ) << msg;
  }
}

Console& console() { return Console::instance(); }

} // namespace three

