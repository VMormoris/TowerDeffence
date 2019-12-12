#include "Shader.h"
#include <iostream>

namespace Engine{

	Shader::Shader() {
		program_ID = 0;
		vertexShader = NULL;
		fragmentShader = NULL;
		vs = 0;
		fs = 0;
	}

	Shader::~Shader() {
		if (vertexShader != NULL) delete[] vertexShader;
		if (fragmentShader != NULL) delete[] fragmentShader;
		if (program_ID != 0) GLCall(glDeleteProgram(program_ID));
	}


	int Shader::setFragmentShader(const char* filename) {
		if (fragmentShader != NULL) { delete[] fragmentShader; }
		// copy vertex shader file path
		char* fname = new char[strlen(filename) + 1];
		memcpy(fname, filename, strlen(filename) + 1);
		fragmentShader = fname;
		return 0;
	}

	int Shader::setVertexShader(const char* filename) {
		if (vertexShader != NULL) { delete[] vertexShader; }
		// copy vertex shader file path
		char* fname = new char[strlen(filename) + 1];
		memcpy(fname, filename, strlen(filename) + 1);
		vertexShader = fname;
		return 0;
	}


	//Code mostly copied from the Lab with wrap in every call of function that OpenGL provide to the GLCall macro
	bool Shader::CreateProgramShader() {

		GLCall(glDeleteProgram(program_ID));

		GLCall(program_ID = glCreateProgram());//Is it working correctly not sure

		//printf("%s\n\n", vertexShader);
		// load the VS Shader
		if ((vs = GenerateShader(vertexShader, GL_VERTEX_SHADER)) == 0) return false;
		GLCall(glAttachShader(program_ID, vs));

		// load the FS Shader
		if ((fs = GenerateShader(fragmentShader, GL_FRAGMENT_SHADER)) == 0) return false;
		GLCall(glAttachShader(program_ID, fs));

		//link them
		int link_ok = GL_FALSE;
		int validate_ok = GL_FALSE;

		GLCall(glLinkProgram(program_ID));
		GLCall(glGetProgramiv(program_ID, GL_LINK_STATUS, &link_ok));
		if (!link_ok) return false; //Consider adding PrintLog Functionality if need it

		GLCall(glValidateProgram(program_ID));
		GLCall(glGetProgramiv(program_ID, GL_VALIDATE_STATUS, &validate_ok));
		if (!validate_ok) return false; //Consider adding PrintLog Functionality if need it



		return true;
	}


	//Code mostly copied from the Lab with wrap in every call of function that OpenGL provide to the GLCall macro
	unsigned int Shader::GenerateShader(const char* filename, GLenum shader_type) {

		//std::cout << "Reading: " << filename << std::endl;
		const char* source = utils::readfileAsString(filename);


		if (source == NULL) {
			printf("Error opening %s: \n", filename);
			delete[] source;
			return 0;
		}

		unsigned int res;
		GLCall(res = glCreateShader(shader_type));

		GLCall(glShaderSource(res, 1, &source, NULL));
		delete source;

		GLCall(glCompileShader(res));
		int compile_ok = GL_FALSE;
		GLCall(glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok));
		if (compile_ok == GL_FALSE) {
			printf("%s: \n", filename);
			GLCall(glDeleteShader(res));
			return 0;
		}

		return res;
	}

	bool Shader::CreateProgram(void) { return CreateProgramShader(); }

	void Shader::LoadUniform(const std::string uniform) { GLCall(uniforms[uniform] = glGetUniformLocation(program_ID, uniform.c_str())); }

	bool Shader::ReloadProgram() {
		CreateProgramShader();
		for (auto& it : uniforms) { GLCall(it.second = glGetUniformLocation(program_ID, it.first.c_str())); }
		return true;
	}


	void Shader::Bind() const { (glUseProgram(program_ID)); }


	void Shader::Unbind() const { GLCall(glUseProgram(0)); }

	int Shader::operator[](const std::string key) {
		auto it = uniforms.find(key);
		return (it != uniforms.end()) ? it->second : -1;
	}


	int Shader::getIndex(const std::string key) {
		auto it = uniforms.find(key);
		return (it != uniforms.end()) ? it->second : -1;
	}

}


