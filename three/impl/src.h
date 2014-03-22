#ifndef THREE_SRC_H
#define THREE_SRC_H

#include <three/config.h>

#if defined(THREE_HEADER_ONLY)
# error Do not compile Three.cpp library source with THREE_HEADER_ONLY defined
#endif

#include <three/core/impl/buffer_geometry.cpp>
#include <three/core/impl/object3d.cpp>

#include <three/math/impl/euler.cpp>
#include <three/math/impl/frustum.cpp>
#include <three/math/impl/matrix3.cpp>
#include <three/math/impl/matrix4.cpp>
#include <three/math/impl/plane.cpp>
#include <three/math/impl/quaternion.cpp>
#include <three/math/impl/ray.cpp>
#include <three/math/impl/sphere.cpp>
#include <three/math/impl/spline.cpp>
#include <three/math/impl/triangle.cpp>
#include <three/math/impl/vector3.cpp>
#include <three/math/impl/vector4.cpp>

#include <three/core/impl/projector.cpp>


#include <three/objects/impl/mesh.cpp>

#include <three/materials/impl/material.cpp>
#include <three/materials/impl/text_2d_material.cpp>
#include <three/materials/impl/uniform.cpp>

#include <three/renderers/impl/gl_shaders.cpp>
#include <three/renderers/impl/gl_renderer.cpp>

#include <three/scenes/impl/scene.cpp>

#endif