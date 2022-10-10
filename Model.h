#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <nlohmann/json.hpp>

#include "Mesh.h"

class Model
{
public:
	Model(const std::string& filename);

	void Render(const Shader& shader, const Camera& camera);

private:
	std::string m_filename;
	std::vector<unsigned char> m_data;
	nlohmann::json m_json;

	// Store the names of the loaded textures, to prevent them loading twice
	std::vector<std::string> m_loadedTextureNames;
	std::vector<Texture> m_loadedTextures;

	// As a model is made up of multiple meshes, we want to store them
	std::vector<Mesh> m_meshes;

	// The transforms per mesh
	std::vector<glm::vec3> m_translations;
	std::vector<glm::quat> m_rotations;
	std::vector<glm::vec3> m_scales;
	std::vector<glm::mat4> m_transformationMatrices;

	void LoadMeshData(unsigned meshIndex);
	void TraverseNode(unsigned nextNode, const glm::mat4& transformationMatrix = glm::mat4(1.f));

	std::vector<unsigned char> GetGltfData();

	// Read the data from the JSON files
	std::vector<float> GetFloats(nlohmann::json& accessor);
	std::vector<GLuint> GetIndices(nlohmann::json& accessor);

	std::vector<Texture> GetTextures();

	// Assemble Vertices based on the data read in
	std::vector<Vertex> AssembleVertices(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& uvs);


	// Construct glm::vectors based on the data read
	std::vector<glm::vec2> GroupFloatsVec2(const std::vector<float>& floats);
	std::vector<glm::vec3> GroupFloatsVec3(const std::vector<float>& floats);
	std::vector<glm::vec4> GroupFloatsVec4(const std::vector<float>& floats);
};
