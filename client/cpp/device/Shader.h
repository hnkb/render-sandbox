#pragma once

#include <GLES3/gl3.h>
#include <filesystem>
#include <vector>


class ShaderProgram
{
public:
	struct
	{
		GLint camera;
	} uniforms;

	ShaderProgram(
		const std::filesystem::path& vertexShaderSource,
		const std::filesystem::path& fragmentShaderSource);
	operator GLuint() const { return handle; }

private:
	GLuint handle = 0;

	static GLuint createShader(GLenum eShaderType, const std::filesystem::path& shaderSource);
	static GLuint createProgram(const std::vector<GLuint>& shaderList);
};
