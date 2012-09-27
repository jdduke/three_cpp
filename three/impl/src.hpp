#ifndef THREE_SRC_HPP
#define THREE_SRC_HPP

#include <three/config.hpp>

#if defined(THREE_HEADER_ONLY)
# error Do not compile Three.cpp library source with THREE_HEADER_ONLY defined
#endif

#include <three/core/impl/buffer_geometry.ipp>
#include <three/core/impl/geometry.ipp>
#include <three/core/impl/matrix4.ipp>
#include <three/core/impl/object3d.ipp>

#include <three/materials/impl/material.ipp>

#include <three/renderers/impl/gl_shaders.ipp>


#endif