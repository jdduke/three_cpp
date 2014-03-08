#ifndef THREE_ANIM_HPP
#define THREE_ANIM_HPP

#include <functional>

namespace three {
namespace anim {

typedef std::function<bool( float )> Update;
typedef std::function<bool( float )> Render;

void gameLoop( Update update, float frameRate = 60 );
void gameLoop( Update update, Render render, float frameRate = 60 );

} // namespace anim
} // namespace three

#if defined(THREE_HEADER_ONLY)
# include <three/extras/impl/anim.ipp>
#endif // defined(THREE_HEADER_ONLY)

#endif // THREE_ANIM_HPP