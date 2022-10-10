#pragma once
#include <string>

#include "OpenGLObject.h"

struct Shader;

enum class eTextureType
{
	e_diffuse,
	e_specular
};


struct Texture : OpenGLObject
{
	Texture(const std::string& filename, eTextureType textureType, GLuint textureSlot);

	void Bind() const override;
	void Unbind() const override;
	void Delete() const override;

	void SendToShader(const Shader& shader, const std::string& uniform, GLint unit);

	eTextureType GetType() const;

private:
	GLuint m_unit;
	eTextureType m_type;
};
