#ifndef THREE_ANIM_H
#define THREE_ANIM_H

#include <functional>

namespace three {
namespace anim {

typedef std::function<bool( float )> Update;
typedef std::function<bool( float )> Render;

void gameLoop( Update update, float frameRate = 60 );
void gameLoop( Update update, Render render, float frameRate = 60 );

} // namespace anim
} // namespace three

#endif // THREE_ANIM_H