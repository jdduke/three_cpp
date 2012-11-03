#ifndef THREE_STATS_HPP
#define THREE_STATS_HPP

#include <three/utils/noncopyable.hpp>

#include <memory>

namespace three {
class GLRenderer;

namespace stats {

class Stats : public NonCopyable {
public:
  THREE_DECL Stats( GLRenderer& renderer );
  THREE_DECL ~Stats();

  THREE_DECL void update( float deltaTime, bool render = true );

private:

  struct Impl;
  std::unique_ptr<Impl> impl;
};

} // namespace stats
} // namespace three

#if defined(THREE_HEADER_ONLY)
# include <three/extras/impl/stats.ipp>
#endif // defined(THREE_HEADER_ONLY)

#endif // THREE_ANIM_HPP