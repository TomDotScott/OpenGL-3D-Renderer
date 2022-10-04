#pragma once
#include <string>

#include "OpenGLObject.h"

struct Shader;

struct Texture : OpenGLObject
{
	Texture(const std::string& filename, GLenum textureType, GLenum textureSlot, GLenum format, GLenum pixelType);

	void Bind() const override;
	void Unbind() const override;
	void Delete() const override;

	void TexUniformUnit(const Shader& shader, const std::string& uniform, GLint unit);

private:
	GLenum m_type;
};
