#ifndef _ORTHOGRAPHIC_CAMERA_HPP_
#define _ORTHOGRAPHIC_CAMERA_HPP_

#include "three.hpp"

#include "camera.hpp"

namespace three {

class OrthographicCamera : public Camera {
public:

	typedef shared_ptr<OrthographicCamera> Ptr;

	float left, right, top, bottom;
	float near, far;

	/////////////////////////////////////////////////////////////////////////

	static Ptr create( float left, float right, float top, float bottom, float near = 0.1f, float far = 2000 ) {

		return make_shared<OrthographicCamera>( left, right, top, bottom, near, far );

	}

	/////////////////////////////////////////////////////////////////////////

	void updateProjectionMatrix() {

		projectionMatrix.makeOrthographic ( left, right, top, bottom, near, far );

	}

	/////////////////////////////////////////////////////////////////////////

protected:

	OrthographicCamera ( float left, float right, float top, float bottom, float near, float far )
	: Camera(), left ( left ), right ( right ), top ( top ), bottom ( bottom ), near ( near ), far ( far ) {

		updateProjectionMatrix();

	 }

	OrthographicCamera ( const OrthographicCamera& ) = delete;
	OrthographicCamera& operator= ( const OrthographicCamera& ) = delete;

};

}

#endif