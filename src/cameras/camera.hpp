#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include "three.hpp"

#include "core/object3d.hpp"

namespace three {

class Camera : public Object3D {
public:

	typedef std::shared_ptr<Camera> Ptr;

	Matrix4 matrixWorldInverse;
	Matrix projectionMatrix;
	Matrix projectionMatrixInverse;

	/////////////////////////////////////////////////////////////////////////

	static Ptr create() { return make_shared<Camera>(); }

	/////////////////////////////////////////////////////////////////////////

	void lookAt ( const Vector3& vector ) {

		matrix.lookAt( position, vector, up );

		if ( rotationAutoUpdate === true ) {

			rotation.setEulerFromRotationMatrix( matrix, eulerOrder );

		}

	}

	/////////////////////////////////////////////////////////////////////////

protected:

	Camera ()
	: Object3D() { }

	Camera ( const Camera& ) = delete;
	Camera& operator= ( const Camera& ) = delete;

	/////////////////////////////////////////////////////////////////////////

	virtual THREE::Type getType() const { return THREE::Camera; }

	virtual void visit()( Visitor& v ) { v( *this ); }

};

}

#endif