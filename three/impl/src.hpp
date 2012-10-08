#ifndef THREE_SRC_HPP
#define THREE_SRC_HPP

#include <three/config.hpp>

#if defined(THREE_HEADER_ONLY)
# error Do not compile Three.cpp library source with THREE_HEADER_ONLY defined
#endif

#include <three/core/impl/buffer_geometry.ipp>
#include <three/core/impl/frustum.ipp>
#include <three/core/impl/geometry.ipp>
#include <three/core/impl/matrix4.ipp>
#include <three/core/impl/matrix3.ipp>
#include <three/core/impl/object3d.ipp>
#include <three/core/impl/quaternion.ipp>

#include <three/objects/impl/mesh.ipp>

#include <three/materials/impl/material.ipp>
#include <three/materials/impl/text_2d_material.ipp>
#include <three/materials/impl/uniform.ipp>

#include <three/renderers/impl/gl_shaders.ipp>
#include <three/renderers/impl/gl_renderer.ipp>

#include <three/scenes/impl/scene.ipp>

#endif