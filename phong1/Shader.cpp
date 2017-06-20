#include "Shader.h"
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <string>

void Shader::check_gl_error(const std::string& op)
{
	int error;
	while ((error = glGetError()) != GL_NO_ERROR)
	{
		std::string errorStr = "UNKNOWN";
		switch (error) {
		case 0:
			errorStr = "GL_NO_ERROR";
			break;
		case 1280:
			errorStr = "GL_INVALID_ENUM";
			break;
		case 1281:
			errorStr = "GL_INVALID_VALUE";
			break;
		case 1282:
			errorStr = "GL_INVALID_OPERATION";
			break;
		case 1283:
			errorStr = "GL_STACK_OVERFLOW";
			break;
		case 1284:
			errorStr = "GL_STACK_UNDERFLOW";
			break;
		case 1285:
			errorStr = "GL_OUT_OF_MEMORY";
			break;
		default:
			break;
		}
		std::cerr << op << " glError " << errorStr << std::endl;
	}
}

int Shader::create_program(const std::string& vertex_filename, const std::string& fragment_filename)
{
	GLuint vertex_shader = create_shader(GL_VERTEX_SHADER, vertex_filename);
	if (vertex_shader == 0) 
	{
		return 0;
	}

	GLuint fragment_shader = create_shader(GL_FRAGMENT_SHADER, fragment_filename);
	if (fragment_shader == 0)
	{
		return 0;
	}

	GLuint program = glCreateProgram();
	if (program != 0)
	{
		glAttachShader(program, vertex_shader);		
		glAttachShader(program, fragment_shader);

		glLinkProgram(program);

		int link_status;
		glGetProgramiv(program, GL_LINK_STATUS, &link_status);
		if (link_status != GL_TRUE)
		{
			std::cerr << "could not link program:" << std::endl;
			
			int bufflen;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufflen);

			GLchar* infolog = new GLchar[bufflen + 1];
			glGetProgramInfoLog(program, bufflen, 0, infolog);
			std::cerr << infolog << std::endl;
			delete infolog;

			glDeleteProgram(program);
			program = 0;
		}
	}

	return program;
}

int Shader::create_shader(int shader_type, const std::string& filename)
{
	GLuint  shader = 0;
	shader = glCreateShader(shader_type);
	
	if (shader != 0) 
	{
		std::ifstream shader_file(filename.c_str());
		std::string   shader_string;

		shader_string.assign(
			(std::istreambuf_iterator<char>(shader_file)),
			std::istreambuf_iterator<char>());

		const GLchar* shader_src = shader_string.c_str();
		glShaderSource(shader, 1, (const GLchar**)&shader_src, NULL);
		glCompileShader(shader);

		int compiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (compiled != GL_TRUE)
		{
			std::cerr << "could not compile shader:" << std::endl;

			int bufflen;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufflen);
			
			GLchar* infolog = new GLchar[bufflen + 1];
			glGetShaderInfoLog(shader, bufflen, 0, infolog);
			std::cerr << infolog << std::endl;
			delete infolog;

			glDeleteShader(shader);
			shader = 0;
		}
	}

	return shader;
}
