#ifndef THREE_SRC_EXTRAS_HPP
#define THREE_SRC_EXTRAS_HPP

#include <three/config.h>

#if defined(THREE_HEADER_ONLY)
# error Do not compile Three.cpp library source with THREE_HEADER_ONLY defined
#endif

#include <three/extras/geometries/impl/text_2d_geometry.cpp>

#include <three/extras/utils/impl/font.cpp>

#include <three/extras/impl/image_utils.cpp>
#include <three/extras/impl/sdl.cpp>
#include <three/extras/impl/anim.cpp>
#include <three/extras/impl/stats.cpp>


#endif