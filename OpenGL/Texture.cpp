#include "Texture.h"

Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
{
	type = texType;
	int imageWidth;
	int imageHeight;
	int colorChannelNumber;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load(image, &imageWidth, &imageHeight, &colorChannelNumber, 0);

	glGenTextures(1, &ID);
	glActiveTexture(slot);
	glBindTexture(texType, ID);

	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_NEAREST);
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_NEAREST);

	glTexImage2D(texType, 0, GL_RGBA, imageWidth, imageHeight, 0, format, pixelType, bytes);
	glGenerateMipmap(texType);

	stbi_image_free(bytes);
	glBindTexture(texType, 0);
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	shader.activate();
	glUniform1i(texUni, unit);
}

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::destroy()
{
	glDeleteTextures(1, &ID);
}