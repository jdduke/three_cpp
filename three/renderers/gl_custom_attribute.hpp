#ifndef THREE_GL_CUSTOM_ATTRIBUTE_HPP
#define THREE_GL_CUSTOM_ATTRIBUTE_HPP

#include <three/common.hpp>

namespace three {

class GLCustomAttribute {
public:

	explicit GLCustomAttribute( THREE::AttributeType type = THREE::V3, int arraySize = 0 )
	: type ( type ),
	  buffer ( 0 ),
	  createUniqueBuffers ( false ),
	  needsUpdate ( false ),
	  size ( 0 ),
	  __glInitialized ( false ),
	  __original ( nullptr ) {

		if (arraySize > 0)
			array.resize( arraySize );

	}

	//GLCustomAttribute(GLCustomAttribute&)            THREE_DECL_DELETE;
	//GLCustomAttribute& operator=(GLCustomAttribute&) THREE_DECL_DELETE;

	/*GLCustomAttribute clone() {
		GLCustomAttribute attribute ( type );
		attribute.array.resize( array.size() );
		attribute.belongsToAttribute = belongsToAttribute;
		attribute.boundTo = boundTo;
		attribute.needsUpdate = true;
		attribute.size =
	}*/

	/////////////////////////////////////////////////////////////////////////

	THREE::AttributeType type;

	std::vector<float> array;
	std::vector<Vector4> value;
	std::string belongsToAttribute;
	std::string boundTo;
	Buffer buffer;
	bool createUniqueBuffers;
	bool needsUpdate;
	int size;

	bool __glInitialized;
	GLCustomAttribute* __original;
};

} // namespace three

#endif // THREE_GL_CUSTOM_ATTRIBUTE_HPP