#pragma once

#include <string>
#include <unordered_map>

using namespace std;

struct ShaderProgramSource {
	string vertexSource;
	string FragmentSource;
};

class Shader {
private:
	string _Filepath;
	unsigned int _RendererID;
	//caching for uniforms
	unordered_map<string, int> _UniformLocationCache;

	unsigned int compileShader(unsigned int type, const string& source);
	unsigned int createShader(const string& vertexShader, const string& fragmentShader);
	ShaderProgramSource parseShader(const string& filepath);

	


public:
	Shader(const string& filepath, unsigned int id);
	~Shader();

	unsigned int getUniformLocation(const string& name);

	void setUniform4f(const string&name, float f0, float f1, float f2, float f3);
	void setUniform3f(const string& name, float f0, float f1, float f2);
	void setUniform1i(const string& name, int i);
	void setUniform1f(const string& name, float f);

	void bind() const;
	void unbind() const;

	void printShader();

};
