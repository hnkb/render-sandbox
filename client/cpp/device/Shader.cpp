
#include "Shader.h"
#include "../utils/File.h"
#include <algorithm>
#include <emscripten/html5.h>


ShaderProgram::ShaderProgram(
	const std::filesystem::path& vertexShaderSource,
	const std::filesystem::path& fragmentShaderSource)
{
	std::vector<GLuint> shaderList;

	shaderList.push_back(createShader(GL_VERTEX_SHADER, vertexShaderSource));
	shaderList.push_back(createShader(GL_FRAGMENT_SHADER, fragmentShaderSource));

	handle = createProgram(shaderList);

	std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);

	uniforms.camera = glGetUniformLocation(handle, "camera");
}

GLuint ShaderProgram::createShader(GLenum eShaderType, const std::filesystem::path& shaderSource)
{
	GLuint shader = glCreateShader(eShaderType);

	const auto sourceCode = File::readAll<char>(shaderSource);
	const auto sourceCodeData = sourceCode.data();
	const auto sourceCodeSize = (GLint)sourceCode.size();
	glShaderSource(shader, 1, &sourceCodeData, &sourceCodeSize);

	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar* strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

		const char* strShaderType = NULL;
		switch (eShaderType)
		{
			case GL_VERTEX_SHADER:
				strShaderType = "vertex";
				break;
			// case GL_GEOMETRY_SHADER:
			// 	strShaderType = "geometry";
			// 	break;
			case GL_FRAGMENT_SHADER:
				strShaderType = "fragment";
				break;
		}

		emscripten_console_errorf("Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
		delete[] strInfoLog;
	}

	return shader;
}

GLuint ShaderProgram::createProgram(const std::vector<GLuint>& shaderList)
{
	GLuint program = glCreateProgram();

	for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glAttachShader(program, shaderList[iLoop]);

	glLinkProgram(program);

	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar* strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		emscripten_console_errorf("Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}

	for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glDetachShader(program, shaderList[iLoop]);

	return program;
}
