#ifndef THREE_NONCOPYABLE_H
#define THREE_NONCOPYABLE_H

#include <three/common.h>

namespace three {

class NonCopyable {
protected:
  NonCopyable() {}
  ~NonCopyable() {}
private:
  NonCopyable( const NonCopyable& ) THREE_DECL_DELETE;
  const NonCopyable& operator=( const NonCopyable& ) THREE_DECL_DELETE;
};

} // namespace three

#endif // THREE_NONCOPYABLE_H
