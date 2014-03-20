#ifndef THREE_DATA_TEXTURE_HPP
#define THREE_DATA_TEXTURE_HPP

#include <three/common.hpp>

#include <three/textures/texture.hpp>

namespace three{

class DataTexture : public Texture {

public:

	struct Image {

		std::vector<float> data;

		float width;
		float height;

	};

	typedef std::shared_ptr<DataTexture> Ptr;

	static Ptr create( const std::vector<float> data, float width, float height, const TextureDesc& desc ) {

		return make_shared<DataTexture>( data, width, height, desc );

	};

	Image image;

	virtual enums::TextureType type() const {

		return enums::DataTexture;
		
	}
	
protected:

	DataTexture( const std::vector<float>& dataIn, float widthIn, float heightIn, const TextureDesc& desc )
		: Texture( desc ) {

			image.data = dataIn;
			image.width = widthIn;
			image.height = heightIn;

	}

};

}

#endif // THREE_DATA_TEXTURE_HPP