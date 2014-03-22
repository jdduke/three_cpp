#ifndef THREE_STATS_H
#define THREE_STATS_H

#include <three/utils/noncopyable.h>

#include <memory>

namespace three {
class GLRenderer;

namespace stats {

class Stats : public NonCopyable {
public:
  Stats( GLRenderer& renderer );
  ~Stats();

  void update( float deltaTime, bool render = true );

private:

  struct Impl;
  std::unique_ptr<Impl> impl;
};

} // namespace stats
} // namespace three

#endif // THREE_ANIM_H