#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <glad/glad.h>

extern std::string Get_File_Contents(const std::string& filename);

struct Shader
{
	Shader(const std::string& vertexFilename, const std::string& fragmentFilename);

	void Activate() const;
	void Delete() const;

	static void CompileErrors(GLuint shader, const std::string& type);

	GLuint m_ID;
};
