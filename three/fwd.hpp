#ifndef THREE_FWD_HPP
#define THREE_FWD_HPP

namespace three {

class Object3D;
class Bone;
class Camera;
class Light;
class Scene;
class Particle;
class Sprite;
class Mesh;

// Math
class Box2;
class Box3;
class Color;
class Euler;
class Face;
class Frustum;
class Line;
class Line3;
class Rectangle;
class Vector2;
class Vector4;
class Vector3;
class Sphere;

class Matrix3;
class Matrix4;
class Quaternion;
class Ray;

class Visitor;
class ConstVisitor;

class Font;

class RenderableFace;
class RenderableObject;
class RenderableLine;
class RenderableParticle;
class RenderableVertex;

class Spline;

class BufferGeometry;
class Geometry;
class GeometryBuffer;
class GeometryGroup;
//class Vertex;
typedef Vector3 Vertex;
class Material;
class Program;
class Texture;
class Shader;
class Uniform;

class GLRenderer;

template < typename Key, typename Value >
class Properties;

} // namespace three

#endif // THREE_FWD_HPP