#ifndef THREE_SCENE_HPP
#define THREE_SCENE_HPP

#include <three/common.hpp>

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

	std::vector<Object3D*> __objects;
	std::vector<Light*>    __lights;

	std::vector<Object3D::Ptr> __objectsAdded;
	std::vector<Object3D::Ptr> __objectsRemoved;

	/////////////////////////////////////////////////////////////////////////

protected:

	template <typename T>
	static bool contains( std::vector<T>& v, T& elem ) {
		return std::find( v.begin(), v.end(), elem ) != v.end();
	}

	template <typename T>
	static bool erase( std::vector<T>& v, T& elem ) {
		auto i = std::find( v.begin(), v.end(), elem );
		if ( i != v.end() ) {
			v.erase( i );
			return true;
		}
		return false;
	}

	struct Add {
		Add( Scene& s, Object3D::Ptr& o ) : scene ( s ), object ( o ) { }

		void operator() ( Object3D& o ) {
			if ( !contains( s.__objects, &o ) ) {

				s.__objects.push( &o );
				s.__objectsAdded.push( object );

				erase( s.__objectsRemoved, object );
			}
		}
		void operator() ( Scene& s ) { (*this)(static_cast<Object3D&>(s)); }
		void operator() ( Light& l ) {
			if ( !contains( s.__lights, &l) {
				s.__lights.push_back( &l );
			}
			if ( l.target && l.target.parent == nullptr ) {
				s.add( l.target );
			}
		}
		void operator() ( Bone& ) { }
		void operator() ( Camera& ) { }

		Scene& s;
		Object3D::Ptr object;
	};

	virtual void __addObject( Object3D::Ptr& object ) {

		if (!object)
			return;

		object.visit( Add( *this, object ) );

		for ( auto& child : object->children ) {
			__addObject( child );
		}

	}


	struct Remove {
		Remove( Scene& s, Object3D::Ptr& o ) : scene ( s ), object ( o ) { }
		void operator() ( Object3D& o ) {
			auto i = std::find( s.__objects.begin(), s.__objects.end(), &o);
			if ( i != s.__objects.end() ) {

				s.__objects.erase( i );
				s.__objectsRemoved.push( object );

				erase ( s.__objectsAdded, object );
			}
		}

		void operator() ( Scene& s ) { (*this)(static_cast<Object3D&>(s)); }
		void operator() ( Light& l ) { erase( s.__lights, &l ); }
		void operator() ( Bone& ) { }
		void operator() ( Camera& ) { }

		Scene& s;
		Object3D::Ptr object;
	};

	virtual void __removeObject( Object3D::Ptr& object ) {
		if (!object)
			return;

		object.visit( Remove( *this, object ) );

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

	virtual THREE::Type getType() const { return THREE::Scene; }

	virtual void visit()( Visitor& v ) { v( *this ); }

};

} // namespace three

#endif // THREE_SCENE_HPP