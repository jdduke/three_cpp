#ifndef THREE_SCENE_HPP
#define THREE_SCENE_HPP

#include <three/common.hpp>
#include <three/utils.hpp>

#include <three/core/object3d.hpp>

namespace three {

class Scene : public Object3D {
public:

	typedef std::shared_ptr<Scene> Ptr;

	static Ptr create() { return make_shared<Scene>(); }

	/////////////////////////////////////////////////////////////////////////

	void* fog;
	Material* overrideMaterial;
	bool matrixAutoUpdate;

	/////////////////////////////////////////////////////////////////////////

protected:

	struct Add : public Visitor {
		Add( Scene& s, Object3D::Ptr& o )
		 : s ( s ), object ( o ) { }

		void operator() ( Object3D& o ) {
			if ( push_unique( s.__objects, &o ) ) {

				s.__objectsAdded.push_back( object );

				erase( s.__objectsRemoved, object );
			}
		}
		void operator() ( Scene& s ) { fallback ( s ); }
		void operator() ( Light& l ) {
			push_unique( s.__lights, &l );
			// TODO:
			/*if ( l.target && l.target->parent == nullptr ) {
				s.add( l.target );
			}*/
		}
		void operator() ( Bone& ) { }
		void operator() ( Camera& ) { }

		Scene& s;
		Object3D::Ptr& object;
	};

	virtual void __addObject( Object3D::Ptr& object ) {

		if (!object)
			return;

		Add objectAdd( *this, object );
		object->visit( objectAdd );

		for ( auto& child : object->children ) {
			__addObject( child );
		}

	}


	struct Remove : public Visitor {
		Remove( Scene& s, Object3D::Ptr& o ) : s ( s ), object ( o ) { }
		void operator() ( Object3D& o ) {
			if ( erase( s.__objects, &o) ) {
				s.__objectsRemoved.push_back( object );
				erase ( s.__objectsAdded, object );
			}
		}

		void operator() ( Scene& s ) { fallback( s ); }
		void operator() ( Light& l ) { erase( s.__lights, &l ); }
		void operator() ( Bone& ) { }
		void operator() ( Camera& ) { }

		Scene& s;
		Object3D::Ptr& object;
	};

	virtual void __removeObject( Object3D::Ptr& object ) {
		if (!object)
			return;

		Remove objectRemove( *this, object );
		object->visit( objectRemove );

		for ( auto& child : object->children ) {
			__removeObject( child );
		}

	}

protected:

	Scene ()
	: Object3D(),
	fog ( nullptr ),
	overrideMaterial ( nullptr ),
	matrixAutoUpdate ( false ) { }

	virtual THREE::Type type() const { return THREE::Scene; }

	virtual void visit( Visitor& v ) { v( *this ); }
	virtual void visit( ConstVisitor& v ) const { v( *this ); }

	std::vector<Object3D*> __objects;
	std::vector<Light*>    __lights;

	std::vector<Object3D::Ptr> __objectsAdded;
	std::vector<Object3D::Ptr> __objectsRemoved;

};

} // namespace three

#endif // THREE_SCENE_HPP