#pragma once
#include <glad/glad.h>

struct OpenGLObject
{
	OpenGLObject(const GLuint id) : m_ID(id) {}

	OpenGLObject(const OpenGLObject& other) = default;
	OpenGLObject(OpenGLObject&& other) noexcept = default;
	OpenGLObject& operator=(const OpenGLObject& other) = default;
	OpenGLObject& operator=(OpenGLObject&& other) noexcept = default;

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;
	virtual void Delete() const = 0;
	virtual ~OpenGLObject() = default;


	GLuint m_ID;
};
