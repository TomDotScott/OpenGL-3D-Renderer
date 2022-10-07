#include "VertexBufferObject.h"
#include "Vertex.h"

VertexBufferObject::VertexBufferObject(const std::vector<Vertex>& vertices) :
	OpenGLObject(0)
{
	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

void VertexBufferObject::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}

void VertexBufferObject::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBufferObject::Delete() const
{
	glDeleteBuffers(1, &m_ID);
}
