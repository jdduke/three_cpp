#ifndef _THREE_HPP_
#define _THREE_HPP_

namespace three {

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
}

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

}

#endif