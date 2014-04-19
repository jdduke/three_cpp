#ifndef THREE_STATS_H
#define THREE_STATS_H

#include <three/utils/noncopyable.h>

#include <memory>

namespace three {
class GLRenderer;
}

namespace three_examples {

class Stats : public three::NonCopyable {
public:
  Stats( three::GLRenderer& renderer );
  ~Stats();

  void update( float deltaTime, bool render = true );

private:

  struct Impl;
  std::unique_ptr<Impl> impl;
};

} // namespace three_examples

#endif // THREE_STATS_H
