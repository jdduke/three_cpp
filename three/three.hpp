#ifndef _THREE_HPP_
#define _THREE_HPP_

namespace three {

namespace THREE {

enum {
	FrontSide  = 0,
	BackSide   = 1,
	DoubleSide = 2
};

enum {
	NoShading     = 0,
	FlatShading   = 1,
	SmoothShading = 2
};

enum {
	NoColors     = 0,
	FaceColors   = 1,
	VertexColors = 2
};

enum {
	NoBlending          = 0,
	NormalBlending      = 1,
	AdditiveBlending    = 2,
	SubtractiveBlending = 3,
	MultiplyBlending    = 4,
	CustomBlending      = 5
};

enum Order {
	XYZ,
	YXZ,
	ZXY,
	ZYX,
	YZX,
	XZY
};

enum Type {
	Object3D = 0,
	Bone
	Camera,
	Light,
	Scene,
};

}

class Object3D;
class Bone;
class Camera;
class Light;
class Scene;

struct Visitor {
	virtual void operator() (const Object3D&) { }
	virtual void operator() (const Bone&)     { }
	virtual void operator() (const Camera&)   { }
	virtual void operator() (const Light&)    { }
	virtual void operator() (const Scene&)    { }
	virtual ~Visitor() { }
};

class Vector2;
class Vector3;
class Vector4;
class Matrix4;
class Quaternion;
class Ray;

class Spline;

class Material;
class Shader;



}

#endif