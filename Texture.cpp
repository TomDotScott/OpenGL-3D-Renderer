#include "Texture.h"

#include <stb_image.h>

#include "Shader.h"

Texture::Texture(const std::string& filename, const eTextureType textureType, const GLuint textureSlot, const GLenum format, const GLenum pixelType) :
	OpenGLObject(0),
	m_unit(textureSlot),
	m_type(textureType)
{
	// Tell stb to flip the image because OpenGL loads from bottom left
	stbi_set_flip_vertically_on_load(true);

	// Set up drawing our texture. We need floats to store the width, height and amount of colour channels in the image
	int width, height, numColourChannels;

	unsigned char* imagePixels = stbi_load(
		filename.c_str(),
		&width,
		&height,
		&numColourChannels,
		0
	);

	// In order to use the texture, we need to create the reference and generate the GL Object
	glGenTextures(1, &m_ID);

	// After, we need to activate the texture and bind it so that OpenGL can draw it
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	glBindTexture(GL_TEXTURE_2D, m_ID);

	// Now we've bound the texture, we need to change the image filtering mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Then we can set up the repeating mode for the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Finally, we generate the image to be displayed on screen
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		width,
		height,
		0,
		static_cast<GLint>(format),
		static_cast<GLint>(pixelType),
		imagePixels
	);

	// And then we'll generate the mipmaps for that image
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(imagePixels);

	// Unbind the texture
	Texture::Unbind();
}

void Texture::Bind() const
{
	glActiveTexture(GL_TEXTURE0 + m_unit);
	glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete() const
{
	glDeleteTextures(1, &m_ID);
}


void Texture::SendToShader(const Shader& shader, const std::string& uniform, const GLint unit)
{
	const GLint texUni = glGetUniformLocation(shader.m_ID, uniform.c_str());

	shader.Activate();

	glUniform1i(texUni, unit);
}

eTextureType Texture::GetType() const
{
	return m_type;
}



