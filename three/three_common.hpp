#ifndef THREE_COMMON_HPP
#define THREE_COMMON_HPP

#include "three_config.hpp"
#include "three_fwd.hpp"

namespace three {

class THREE {
public:

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
		Bone,
		Camera,
		Light,
		Scene,
		Particle,
		Sprite,
		Mesh,
		Line
	};

	enum FaceType {
		Face3 = 0,
		Face4
	};

	enum MaterialType {
		MeshFaceMaterial = 0
	};

};

struct Visitor {
	virtual void operator() (const Object3D&) { }
	virtual void operator() (const Bone&)     { }
	virtual void operator() (const Camera&)   { }
	virtual void operator() (const Light&)    { }
	virtual void operator() (const Scene&)    { }
	virtual void operator() (const Particle&) { }
	virtual void operator() (const Sprite&)   { }
	virtual void operator() (const Mesh&)     { }
	virtual void operator() (const Line&)     { }
	virtual ~Visitor() { }
};

}

#endif