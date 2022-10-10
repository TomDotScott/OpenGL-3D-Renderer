#include "Mesh.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.inl>

#include "Vertex.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::vector<Texture>& textures) :
	m_vertices(vertices),
	m_indices(indices),
	m_textures(textures),
	m_vao()
{
	m_vao.Bind();

	VertexBufferObject vbo(m_vertices);
	ElementBufferObject ebo(m_indices);

	// Attributes for the Position data per vertex
	m_vao.LinkAttrib(
		vbo,
		0,
		3,
		GL_FLOAT,
		sizeof(Vertex),
		nullptr
	);

	// Attributes for the Normals data per vertex
	m_vao.LinkAttrib(
		vbo,
		1,
		3,
		GL_FLOAT,
		sizeof(Vertex),
		(void*)(3 * sizeof(float))
	);

	// Attributes for the Colour data per vertex
	m_vao.LinkAttrib(
		vbo,
		2,
		3,
		GL_FLOAT,
		sizeof(Vertex),
		(void*)(6 * sizeof(float))
	);

	// Attributes for the UV data per vertex
	m_vao.LinkAttrib(
		vbo,
		3,
		2,
		GL_FLOAT,
		sizeof(Vertex),
		(void*)(9 * sizeof(float))
	);

	m_vao.Unbind();
	vbo.Unbind();
	ebo.Unbind();
}

void Mesh::Render(const Shader& shader, const Camera& camera, const glm::mat4& modelMatrix, const glm::vec3& translation, const glm::quat& rotation, const glm::vec3& scale)
{
	shader.Activate();
	m_vao.Bind();

	unsigned numDiffuse = 0;
	unsigned numSpecular = 0;

	// Find and bind the correct textures for the mesh to the shaders
	for (int i = 0; i < static_cast<int>(m_textures.size()); ++i)
	{
		std::string uniform;

		switch (m_textures[i].GetType()) {
		case eTextureType::e_diffuse:
			uniform = "diffuse" + std::to_string(numDiffuse++);
			break;
		case eTextureType::e_specular:
			uniform = "specular" + std::to_string(numSpecular++);
			break;
		default:
			std::cout << "UNKNOWN TEXTURE TYPE\n";
			continue;
		}

		m_textures[i].SendToShader(shader, uniform, i);
		m_textures[i].Bind();
	}

	// Send the camera position
	glUniform3f(glGetUniformLocation(shader.m_ID, "camPos"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

	camera.SendMatrixToShader(shader, "camMatrix");

	glm::mat4 trans = glm::mat4(1.f);
	glm::mat4 rot = glm::mat4(1.f);
	glm::mat4 sca = glm::mat4(1.f);

	trans = glm::translate(trans, translation);
	rot = glm::mat4_cast(rotation);
	sca = glm::scale(sca, scale);

	glUniformMatrix4fv(glGetUniformLocation(shader.m_ID, "translation"), 1, GL_FALSE, glm::value_ptr(trans));
	glUniformMatrix4fv(glGetUniformLocation(shader.m_ID, "rotation"), 1, GL_FALSE, glm::value_ptr(rot));
	glUniformMatrix4fv(glGetUniformLocation(shader.m_ID, "scale"), 1, GL_FALSE, glm::value_ptr(scale));
	glUniformMatrix4fv(glGetUniformLocation(shader.m_ID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));


	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
}

