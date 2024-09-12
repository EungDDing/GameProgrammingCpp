#include "Texture.h"
#include "SOIL/SOIL.h"
#include "GL/glew.h"
#include "SDL2/SDL.h"

Texture::Texture()
	:mTextureID(0)
	, mWidth(0)
	, mHeight(0)
{

}

Texture::~Texture()
{

}

bool Texture::Load(const std::string& fileName) {
	int channels = 0;

	unsigned char* image = SOIL_load_image(
		fileName.c_str(),	// file name
		&mWidth,			// save a width
		&mHeight,			// save a height
		&channels,			// save the number of channel
		SOIL_LOAD_AUTO		// type of image file or auto
	);

	if (image == nullptr) {
		SDL_Log("SOIL failed to load image %s: %s", fileName.c_str(), SOIL_last_result());
		return false;
	}

	int format = GL_RGB;
	if (channels == 4) {
		format = GL_RGBA;
	}

	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	glTexImage2D(
		GL_TEXTURE_2D,	  // target of texture
		0,				  // LOD (Level Of Detail)
		format,			  // color format that OpenGL should use
		mWidth,			  // width
		mHeight,		  // height
		0,				  // boarder (0)
		format,			  // color format of input data
		GL_UNSIGNED_BYTE, // bit depth of input data (unsigned byte means 8bit channel
		image			  // pointer of image data
	);

	SOIL_free_image_data(image);

	// Enable bilinear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;
}

void Texture::Unload() {
	glDeleteTextures(1, &mTextureID);
}

void Texture::SetActive() {
	glBindTexture(GL_TEXTURE_2D, mTextureID);
}