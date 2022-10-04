#include "VertexArrayObject.h"


VertexArrayObject::VertexArrayObject() : OpenGLObject(0)
{
	glGenVertexArrays(1, &m_ID);
}

void VertexArrayObject::LinkAttrib(const VertexBufferObject& VBO, const GLuint layout, const GLint numComponents, const GLenum type, const GLsizeiptr stride, const void* offset)
{
	VBO.Bind();

	glVertexAttribPointer(
		layout,
		numComponents,
		type,
		GL_FALSE,
		stride,
		offset
	);

	glEnableVertexAttribArray(layout);

	VBO.Unbind();
}

void VertexArrayObject::Bind() const
{
	glBindVertexArray(m_ID);
}

void VertexArrayObject::Unbind() const
{
	glBindVertexArray(0);
}

void VertexArrayObject::Delete() const
{
	glDeleteVertexArrays(1, &m_ID);
}
