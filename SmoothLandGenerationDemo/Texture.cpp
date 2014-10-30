#include "Texture.h"
#include "Singletons.h"
#include <SOIL.h>

Texture::~Texture()
{
	glDeleteSamplers(1,&samplerID);
	glDeleteTextures(1,&textureID);
}

/****************************
* LoadTexture - Loads the texture from file
****************************/
void Texture::LoadTexture(const char* file)
{
	glGenTextures(1,&textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	textureID=SOIL_load_OGL_texture(file,SOIL_LOAD_AUTO,textureID,SOIL_FLAG_MIPMAPS);

	glGenSamplers(1,&samplerID);
	glBindSampler(ogl->textureSamplerID,samplerID);
	glSamplerParameteri(samplerID,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glSamplerParameteri(samplerID,GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_LINEAR);
}

/****************************
* UseTexture - Tells OpenGL to use the texture
****************************/
void Texture::UseTexture()
{
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D,textureID);
	glBindSampler(0,samplerID);
}