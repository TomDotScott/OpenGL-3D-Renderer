#include "Shader.h"

#include <cassert>

std::string Get_File_Contents(const std::string& filename)
{
	std::ifstream in(filename, std::ios::binary);

	if(in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());

		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();

		return contents;
	}

	// Throw an assert if the shader can't be loaded!
	assert(false);
	return "";
}

Shader::Shader(const std::string& vertexFilename, const std::string& fragmentFilename)
{
	const std::string vertexCode = Get_File_Contents(vertexFilename);
	const char* vertexSource = vertexCode.c_str();

	const std::string fragmentCode = Get_File_Contents(fragmentFilename);
	const char* fragmentSource = fragmentCode.c_str();


	// Create the shaders and get a reference value for them
	const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Tell OpenGL where to get the source code from...
	glShaderSource(vertexShader, 1, &vertexSource, nullptr);
	// Finally, compile the shader
	glCompileShader(vertexShader);
	CompileErrors(vertexShader, "VERTEX");

	// Same for the Fragment Shader...
	const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
	glCompileShader(fragmentShader);
	CompileErrors(fragmentShader, "FRAGMENT");

	// To use the shaders, we need to create a shader program
	m_ID = glCreateProgram();

	// Attach the shaders to the shader program
	glAttachShader(m_ID, vertexShader);
	glAttachShader(m_ID, fragmentShader);

	glLinkProgram(m_ID);
	CompileErrors(m_ID, "PROGRAM");

	// Because we've linked them, we can delete the references
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Activate() const
{
	glUseProgram(m_ID);
}

void Shader::Delete() const
{
	glDeleteProgram(m_ID);
}

void Shader::CompileErrors(const GLuint shader, const std::string& type)
{
	GLint hasCompiled;
	char infoLog[1024];

	if(type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);

		glGetShaderInfoLog(shader, 1024, nullptr, infoLog);

		if(hasCompiled == GL_FALSE)
		{
			std::cout << "SHADER COMPILATION ERROR FOR: " << type << "\n";
		}
	}
	else
	{
		glGetProgramiv(shader, GL_COMPILE_STATUS, &hasCompiled);

		glGetProgramInfoLog(shader, 1024, nullptr, infoLog);

		if (hasCompiled == GL_FALSE)
		{
			std::cout << "SHADER LINKING ERROR FOR: " << type << "\n";
		}
	}

	std::cout << infoLog << std::endl;
}
