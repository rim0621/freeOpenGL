#pragma once
#include <string>

class Shader
{
public:
	static void check_gl_error(const std::string& op);

	static int create_program(const std::string& vertex_source, const std::string& fragment_source);

	static int create_shader(int shaderType, const std::string& filename);
};

