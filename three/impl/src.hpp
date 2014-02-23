#ifndef THREE_SRC_HPP
#define THREE_SRC_HPP

#include <three/config.hpp>

#if defined(THREE_HEADER_ONLY)
# error Do not compile Three.cpp library source with THREE_HEADER_ONLY defined
#endif

#include <three/core/impl/buffer_geometry.ipp>
#include <three/math/impl/frustum.ipp>
#include <three/math/impl/geometry.ipp>
#include <three/math/impl/box2.ipp>
#include <three/math/impl/box3.ipp>
#include <three/math/impl/matrix4.ipp>
#include <three/math/impl/matrix3.ipp>
#include <three/math/impl/vector4.ipp>
#include <three/math/impl/plane.ipp>
#include <three/core/impl/object3d.ipp>
#include <three/math/impl/projector.ipp>
#include <three/math/impl/euler.ipp>
#include <three/math/impl/quaternion.ipp>



#include <three/objects/impl/mesh.ipp>

#include <three/materials/impl/material.ipp>
#include <three/materials/impl/text_2d_material.ipp>
#include <three/materials/impl/uniform.ipp>

#include <three/renderers/impl/gl_shaders.ipp>
#include <three/renderers/impl/gl_renderer.ipp>

#include <three/scenes/impl/scene.ipp>

#endif