#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <glad/glad.h>

ShaderProgramSource Shader::parseShader(const string& filepath)
{
	ifstream stream(filepath);
	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};
	string line;
	stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line)) {
		if (line.find("#shader") != string::npos) {
			if (line.find("vertex") != string::npos) {
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != string::npos) {
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			ss[(int)type] << line << '\n';
		}
	}
	return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::compileShader(unsigned int type, const string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << endl;
		cout << message << endl;
		glDeleteShader(id);
		return 0;
	}
	return id;
}


unsigned int Shader::createShader(const string& vertexShader, const string& fragmentShader)
{
	//creates an empty program object and returns a non-zero value by which it can be referenced. A program object is an object to which shader objects can be attached
	unsigned int program = glCreateProgram();
	//creates an empty shader object and returns a non-zero value by which it can be referenced. A shader object is used to maintain the source code strings that define a shader.
	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
	//Shaders that are to be linked together in a program object must first be attached to that program object
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	//links the program object specified by program
	glLinkProgram(program);
	int result;
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	if (result == GL_FALSE) {
		char infoLog[512];
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		return 0;
	}
	//detaches the shader object specified by shader from the program object specified by program. This command can be used to undo the effect of the command glAttachShader.
	glDetachShader(program, vs);
	glDetachShader(program, fs);
	//checks to see whether the executables contained in program can execute given the current OpenGL state
	glValidateProgram(program);
	//frees the memory and invalidates the name associated with the shader object specified by shader.
	glDeleteShader(vs);
	glDeleteShader(fs);
	return program;
}


Shader::Shader(const string& filepath, unsigned int id)
	: _Filepath(filepath), _RendererID(id)
{
	if (filepath == "-1") {
		return;
	}
	ShaderProgramSource source = parseShader(filepath);
	_RendererID = createShader(source.vertexSource, source.FragmentSource);
}

Shader::~Shader()
{
	glDeleteProgram(_RendererID);
}

void Shader::bind() const
{
	glUseProgram(_RendererID);
}

void Shader::unbind() const
{
	glUseProgram(0);
}

void Shader::printShader()
{
	std::cout << "Shader filepath: " << _Filepath << std::endl;
	std::cout << "Shader id: " << _RendererID << std::endl;
}

unsigned int Shader::getUniformLocation(const string& name)
{
	if (_UniformLocationCache.find(name) != _UniformLocationCache.end())
		return _UniformLocationCache[name];
	int location = glGetUniformLocation(_RendererID, name.c_str());
	if (location == -1) {
		cout << "Warning: uniform '" << name << "' doesn't exist!" << endl;
	}
	_UniformLocationCache[name] = location;
	return location;
}

void Shader::setUniform4f(const string& name, float f0, float f1, float f2, float f3)
{
	//glUniform operates on the program object that was made part of current state by calling glUseProgram.
	glUniform4f(getUniformLocation(name), f0, f1, f2, f3);
}

void Shader::setUniform3f(const string& name, float f0, float f1, float f2)
{
	glUniform3f(getUniformLocation(name), f0, f1, f2);
}

void Shader::setUniform1i(const string& name, int i)
{
	glUniform1i(getUniformLocation(name), i);
}

void Shader::setUniform1f(const string& name, float f)
{
	glUniform1f(getUniformLocation(name), f);
}
