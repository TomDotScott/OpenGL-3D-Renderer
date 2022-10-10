#include "Model.h"
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

Model::Model(const std::string& filename) :
	m_filename(filename)
{
	std::string text = Get_File_Contents(filename);

	m_json = nlohmann::json::parse(text);

	m_data = GetGltfData();

	TraverseNode(0);
}

void Model::Render(const Shader& shader, const Camera& camera)
{
	for (int i = 0; i < m_meshes.size(); ++i)
	{
		m_meshes[i].Render(shader, camera, m_transformationMatrices[i]);
	}
}

void Model::LoadMeshData(const unsigned meshIndex)
{
	const unsigned positionAccessorIndex = m_json["meshes"][meshIndex]["primitives"][0]["attributes"]["POSITION"];
	const unsigned normalAccessorIndex = m_json["meshes"][meshIndex]["primitives"][0]["attributes"]["NORMAL"];
	const unsigned uvAccessorIndex = m_json["meshes"][meshIndex]["primitives"][0]["attributes"]["TEXCOORD_0"];
	const unsigned indicesAccessorIndex = m_json["meshes"][meshIndex]["primitives"][0]["indices"];

	// Load the Position, Normal and UV data for the vertices
	const std::vector<glm::vec3> positions = GroupFloatsVec3(GetFloats(m_json["accessors"][positionAccessorIndex]));

	const std::vector<glm::vec3> normals = GroupFloatsVec3(GetFloats(m_json["accessors"][normalAccessorIndex]));

	const std::vector<glm::vec2> uvs = GroupFloatsVec2(GetFloats(m_json["accessors"][uvAccessorIndex]));

	std::vector<Vertex> vertices = AssembleVertices(positions, normals, uvs);

	std::vector<GLuint> indices = GetIndices(m_json["accessors"][indicesAccessorIndex]);

	std::vector<Texture> textures = GetTextures();

	m_meshes.emplace_back(vertices, indices, textures);
}

glm::vec3 MakeVec3(const nlohmann::json& node, const std::string& name)
{
	float translationValues[3];

	for (unsigned i = 0; i < node[name].size(); ++i)
	{
		translationValues[i] = node[name][i];
	}

	return glm::make_vec3(translationValues);
}

void Model::TraverseNode(unsigned nextNode, const glm::mat4& transformationMatrix)
{
	nlohmann::json node = m_json["nodes"][nextNode];

	glm::vec3 translation = glm::vec3(0.f);
	if (node.find("translation") != node.end())
	{
		translation = MakeVec3(node, "translation");
	}

	glm::quat rotation = glm::quat(1.f, 0.f, 0.f, 0.f);
	if (node.find("rotation") != node.end())
	{
		// GLTF is XYZW, GLM is WXYZ
		float rotationValues[4]
		{
			node["rotation"][3],
			node["rotation"][0],
			node["rotation"][1],
			node["rotation"][2]
		};

		rotation = glm::make_quat(rotationValues);
	}

	glm::vec3 scale = glm::vec3(1.f);
	if (node.find("scale") != node.end())
	{
		scale = MakeVec3(node, "scale");
	}

	glm::mat4 nodeMatrix = glm::mat4(1.f);
	if (node.find("matrix") != node.end())
	{
		float matrixValues[16];

		for (unsigned i = 0; i < node["matrix"].size(); ++i)
		{
			matrixValues[i] = node["matrix"][i];
		}

		nodeMatrix = glm::make_mat4(matrixValues);
	}

	// Combine all the transformations
	glm::mat4 trans = glm::translate(glm::mat4(1.f), translation);
	trans = glm::scale(trans, scale);

	const glm::mat4 rot = glm::mat4_cast(rotation);

	glm::mat4 matrix = transformationMatrix * nodeMatrix * trans * rot;

	if (node.find("mesh") != node.end())
	{
		m_translations.emplace_back(translation);
		m_rotations.emplace_back(rotation);
		m_scales.emplace_back(scale);

		m_transformationMatrices.emplace_back(matrix);

		LoadMeshData(node["mesh"]);
	}

	if (node.find("children") != node.end())
	{
		for (auto& i : node["children"])
		{
			TraverseNode(i, matrix);
		}
	}
}

std::vector<unsigned char> Model::GetGltfData()
{
	// uri tells us where to get the binary data for the model
	const std::string uri = m_json["buffers"][0]["uri"];

	const std::string fileString = m_filename;
	const std::string fileDirectory = fileString.substr(0, fileString.find_last_of('\\') + 1);

	std::string bytesText = Get_File_Contents(fileDirectory + uri);

	std::vector<unsigned char> data(bytesText.begin(), bytesText.end());

	return data;
}

std::vector<float> Model::GetFloats(nlohmann::json& accessor)
{
	// Grab some data from the GLTF file
	const unsigned bufferViewIndex = accessor.value("bufferView", 1);
	const unsigned count = accessor["count"];
	const unsigned accessorByteOffset = accessor.value("byteOffset", 0);
	const std::string type = accessor["type"];

	// Now we know which index we're looking at, we can loop over the offsets to get the float data
	nlohmann::json bufferView = m_json["bufferViews"][bufferViewIndex];
	const unsigned byteOffset = bufferView["byteOffset"];

	unsigned numPerVert;

	if (type == "SCALAR")
	{
		numPerVert = 1;
	}
	else if (type == "VEC2")
	{
		numPerVert = 2;
	}
	else if (type == "VEC3")
	{
		numPerVert = 3;
	}
	else if (type == "VEC4")
	{
		numPerVert = 4;
	}
	else
	{
		std::cout << "Type is invalid (not SCALAR, VEC2, VEC3 or VEC4!)\n" << std::endl;
		assert(false);
		return {};
	}

	const unsigned beginningOfData = byteOffset + accessorByteOffset;
	const unsigned lengthOfData = count * sizeof(float) * numPerVert;

	std::vector<float> floats;

	for (unsigned i = beginningOfData; i < beginningOfData + lengthOfData; i)
	{
		const unsigned char bytes[] = { m_data[i++], m_data[i++], m_data[i++], m_data[i++] };

		float value;

		std::memcpy(&value, bytes, sizeof(float));
		floats.emplace_back(value);
	}

	return floats;
}

std::vector<GLuint> Model::GetIndices(nlohmann::json& accessor)
{
	// Grab some data from the GLTF file
	const unsigned bufferViewIndex = accessor.value("bufferView", 0);
	const unsigned count = accessor["count"];
	const unsigned accessorByteOffset = accessor.value("byteOffset", 0);
	const unsigned componentType = accessor["componentType"];

	nlohmann::json bufferView = m_json["bufferViews"][bufferViewIndex];
	unsigned byteOffset = bufferView.value("byteOffset", 0);

	/*if (byteOffset == -1)
	{
		std::cout << "NO BUFFER VIEW FOUND, MOVING ON" << std::endl;
		return { };
	}*/

	unsigned beginningOfData = byteOffset + accessorByteOffset;

	std::vector<GLuint> indices;
	if (componentType == GL_UNSIGNED_INT)
	{
		for (int i = beginningOfData; i < byteOffset + accessorByteOffset + count * sizeof(unsigned); i)
		{
			const unsigned char bytes[] = { m_data[i++], m_data[i++], m_data[i++], m_data[i++] };

			unsigned value;
			memcpy(&value, bytes, sizeof(unsigned));

			indices.emplace_back(value);
		}
	}
	else if (componentType == GL_UNSIGNED_SHORT)
	{
		for (int i = beginningOfData; i < byteOffset + accessorByteOffset + count * sizeof(unsigned short); i)
		{
			const unsigned char bytes[] = { m_data[i++], m_data[i++] };

			unsigned short value;
			memcpy(&value, bytes, sizeof(unsigned short));

			indices.emplace_back(value);
		}
	}
	else if (componentType == GL_SHORT)
	{
		for (int i = beginningOfData; i < byteOffset + accessorByteOffset + count * sizeof(short); i)
		{
			const unsigned char bytes[] = { m_data[i++], m_data[i++] };

			unsigned short value;
			memcpy(&value, bytes, sizeof(short));

			indices.emplace_back(value);
		}
	}

	return indices;
}

std::vector<Texture> Model::GetTextures()
{
	std::vector<Texture> textures;

	std::string fileDirectory = m_filename.substr(0, m_filename.find_last_of('\\') + 1);

	// Loop over all the textures referenced in the GLTF
	unsigned unit = 0;
	for (unsigned i = 0; i < m_json["images"].size(); ++i)
	{
		std::string texturePath = m_json["images"][i]["uri"];

		bool skip = false;
		for (int j = 0; j < m_loadedTextureNames.size(); ++j)
		{
			if (m_loadedTextureNames[j] == texturePath)
			{
				textures.emplace_back(m_loadedTextures[j]);
				skip = true;
				break;
			}
		}

		if (!skip) {
			// If we find the texture, create it with the correct tags 
			if (texturePath.find("baseColor") != std::string::npos)
			{
				const Texture diffuse(fileDirectory + texturePath, eTextureType::e_diffuse, m_loadedTextures.size());
				textures.emplace_back(diffuse);
				m_loadedTextures.emplace_back(diffuse);
			}
			else if (texturePath.find("metallicRoughness") != std::string::npos)
			{
				// Because most GLTF models will be PBR, I'm using roughness for specular for now...
				Texture specular(fileDirectory + texturePath, eTextureType::e_specular, m_loadedTextures.size());
				textures.emplace_back(specular);
				m_loadedTextures.emplace_back(specular);
			}
		}
	}

	return textures;
}

std::vector<Vertex> Model::AssembleVertices(const std::vector<glm::vec3>& positions,
	const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& uvs)
{
	std::vector<Vertex> vertices;
	vertices.reserve(positions.size());

	for (int i = 0; i < positions.size(); ++i)
	{
		vertices.push_back({
			positions[i],
			normals[i],
			glm::vec3(1.f),
			uvs[i]
			}
		);
	}

	return vertices;
}

std::vector<glm::vec2> Model::GroupFloatsVec2(const std::vector<float>& floats)
{
	std::vector<glm::vec2> vectors;

	vectors.reserve(floats.size() / 2);

	for (int i = 0; i < floats.size(); i)
	{
		vectors.emplace_back(floats[i++], floats[i++]);
	}
	return vectors;
}

std::vector<glm::vec3> Model::GroupFloatsVec3(const std::vector<float>& floats)
{
	std::vector<glm::vec3> vectors;

	vectors.reserve(floats.size() / 3);

	for (int i = 0; i < floats.size(); i)
	{
		vectors.emplace_back(floats[i++], floats[i++], floats[i++]);
	}
	return vectors;
}

std::vector<glm::vec4> Model::GroupFloatsVec4(const std::vector<float>& floats)
{
	std::vector<glm::vec4> vectors;

	vectors.reserve(floats.size() / 4);

	for (int i = 0; i < floats.size(); i)
	{
		vectors.emplace_back(floats[i++], floats[i++], floats[i++], floats[i++]);
	}
	return vectors;
}


