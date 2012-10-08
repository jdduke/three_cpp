#ifndef THREE_SRC_EXTRAS_HPP
#define THREE_SRC_EXTRAS_HPP

#include <three/config.hpp>

#if defined(THREE_HEADER_ONLY)
# error Do not compile Three.cpp library source with THREE_HEADER_ONLY defined
#endif

//#include <three/extras/utils/impl/font.ipp>
#include <three/extras/utils/impl/font2.ipp>

#include <three/extras/impl/image_utils.ipp>
#include <three/extras/impl/sdl.ipp>
#include <three/extras/impl/anim.ipp>
#include <three/extras/impl/stats.ipp>


#endif