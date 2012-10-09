#ifndef THREE_STATS_HPP
#define THREE_STATS_HPP

#include <three/utils/noncopyable.hpp>

#include <memory>

namespace three {
class GLRenderer;

namespace stats {

class Stats : public NonCopyable {
public:
  THREE_DECL Stats();
  THREE_DECL ~Stats();
  
  void update( float deltaTime ) { updateImpl( deltaTime ); }
  void update( float deltaTime, GLRenderer& renderer ) { updateImpl( deltaTime, &renderer ); }

private:

  THREE_DECL void updateImpl( float deltaTime, GLRenderer* renderer = nullptr );

  struct Impl;
  std::unique_ptr<Impl> impl;
};

} // namespace stats
} // namespace three

#if defined(THREE_HEADER_ONLY)
# include <three/extras/impl/stats.ipp>
#endif // defined(THREE_HEADER_ONLY)

#endif // THREE_ANIM_HPP