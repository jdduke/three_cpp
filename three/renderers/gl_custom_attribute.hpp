#ifndef THREE_GL_CUSTOM_ATTRIBUTE_HPP
#define THREE_GL_CUSTOM_ATTRIBUTE_HPP

namespace three {

class GLCustomAttribute {
public:

	explicit GLCustomAttribute(THREE::AttributeType type)
	: type ( type ),
	  buffer ( 0 ),
	  createUniqueBuffers ( false ),
	  needsUpdate ( false ),
	  size ( 0 ),
	  __webGLInitialized( false ) { }

	GLCustomAttribute(GLCustomAttribute&)            THREE_DECL_DELETE;
	GLCustomAttribute& operator=(GLCustomAttribute&) THREE_DECL_DELETE;

	/////////////////////////////////////////////////////////////////////////

	THREE::AttributeType type;

	std::vector<float> array;
	std::string belongsToAttribute;
	std::string boundTo;
	Buffer buffer;
	bool createUniqueBuffers;
	bool needsUpdate;
	int size;

	bool __webGLInitialized;
};

} // namespace three

#endif // THREE_GL_CUSTOM_ATTRIBUTE_HPP