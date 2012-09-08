#ifndef THREE_ORTHOGRAPHIC_CAMERA_HPP
#define THREE_ORTHOGRAPHIC_CAMERA_HPP

#include <three/common.hpp>

#include <three/cameras/camera.hpp>

namespace three {

class OrthographicCamera : public Camera {
public:

	typedef std::shared_ptr<OrthographicCamera> Ptr;

	static Ptr create( float left, float right, float top, float bottom, float near = 0.1f, float far = 2000 ) {
		return std::make_shared<OrthographicCamera>( left, right, top, bottom, near, far );
	}

	/////////////////////////////////////////////////////////////////////////

	float left, right, top, bottom;
	float near, far;

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

} // namespace three

#endif // THREE_ORTHOGRAPHIC_CAMERA_HPP