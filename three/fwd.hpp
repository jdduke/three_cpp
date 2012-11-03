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
class Face;
class Line;
class Rectangle;
class Color;
class Vector2;
class Vector3;
class Vector4;
class Matrix4;
class Quaternion;
class Ray;
class Frustum;

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