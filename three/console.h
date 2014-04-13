#ifndef THREE_CONSOLE_H
#define THREE_CONSOLE_H

#include <three/config.h>

#include <memory>
#include <iostream>
#include <sstream>

namespace three {

class Console {
private:
  class LogProxy;

public:

  typedef void( *LogP )( const char* );

  static Console& instance();

  void info( const char* msg ) const;
  void log( const char* msg ) const;
  void debug( const char* msg ) const;
  void warn( const char* msg ) const;
  void error( const char* msg ) const;

  LogProxy info()  const;
  LogProxy log()   const;
  LogProxy debug() const;
  LogProxy warn()  const;
  LogProxy error() const;

  void setLog( LogP log );

private:

  LogP output;

  class LogProxy {
  public:
    LogProxy( LogProxy && other );
    ~LogProxy();

    template <class T>
    LogProxy& operator<<( const T& rhs ) {
      *stream << rhs;
      return *this;
    }

  private:

    friend class Console;

    explicit LogProxy( LogP log, const char* msg = nullptr );

    LogProxy& operator= ( LogProxy && other )     THREE_DECL_DELETE;
    LogProxy( const LogProxy& other )             THREE_DECL_DELETE;
    LogProxy& operator= ( const LogProxy& other ) THREE_DECL_DELETE;

    std::unique_ptr<std::stringstream> stream;
    LogP log;
  };

private:
  Console();
};

Console& console();

} // namespace three

#endif // THREE_CONSOLE_H
