#ifndef Texture_H
#define Texture_H

#include <gl\glew.h>

class Texture
{
public:
	void LoadTexture(const char* file);
	void UseTexture();
	~Texture();

	GLuint GetTextureID() const{return textureID;};
	GLuint GetSamplerID() const{return samplerID;};
private:
	GLuint textureID;
	GLuint samplerID;
};

#endif