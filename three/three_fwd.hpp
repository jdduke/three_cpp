#ifndef THREE_FWD_HPP
#define THREE_FWD_HPP

namespace std {

template < typename T >
class shared_ptr;

template < typename T, class... Args >
shared_ptr<T> make_shared( Args... args );

}

namespace three {

class Object3D;
class Bone;
class Camera;
class Light;
class Scene;
class Particle;
class Sprite;
class Mesh;
class Line;
class Vector2;
class Vector3;
class Vector4;
class Matrix4;
class Quaternion;
class Ray;
class Spline;
class Material;
class Shader;

template < typename T >
using shared_ptr = std::shared_ptr<T>;
using std::make_shared;

}

#endif