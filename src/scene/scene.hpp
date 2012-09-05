#ifndef _SCENE_HPP_
#define _SCENE_HPP_

#include "core/object3d.hpp"

#include <memory>

namespace three {

class Scene : public Object3D {
public:

	void* fog;
	void* overrideMaterial;

	bool matrixAutoUpdate;

	std::vector<Object3D::Ptr> __objects;
	std::vector<Light::Ptr> __lights;

	std::vector<Object3D::Ptr> __objectsAdded;
	std::vector<Object3D::Ptr> __objectsRemoved;

	static Object3D::Ptr create() {
		return make_shared<Scene>();
	}

	struct Add {
		Add(Scene& s) : scene ( s ) { }
		void operator() ( Light::Ptr )
	};

private:

	Scene ()
	: Object3D(),
	fog ( nulptr ),
	overrideMaterial ( nullptr ),
	matrixAutoUpdate ( false ) { }

};

}

#endif