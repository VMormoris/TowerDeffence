#pragma once
#ifndef SHADER_H
#define SHADER_H
#include "Core/utils.h"

namespace Engine{

	/**
	* This class containg the functionality for using shader files
	*	Most of the code for the implementation of this class is same with the Lab (Computer Graphics at AUEB 2019-2020)
	*   We just added comments rename some variables or methods names and wrap every
	*   call of a function that OpenGL provides to the GLCall macro
	* @see Renderer.h
	*/
	class ENGINE_API Shader {
	private:

		/**
		* Filepath for the Vertex Shader
		*/
		const char* vertexShader;

		/**
		* Filepath for the Fragment Shader
		*/
		const char* fragmentShader;

		/**
		* Program's ID
		*/
		unsigned int program_ID;

		/**
		* Vertex shader's ID
		*/
		unsigned int vs;

		/**
		* Fragment shader's ID
		*/
		unsigned int fs;

		/**
		* Hash map with uniform indices
		*/
		std::unordered_map<std::string, int> uniforms;

		/**
		* Create the shader
		* @return true if shader created succesfully and false otherwise
		*/
		bool CreateProgramShader(void);


		/**
		* Load the shader from the disk
		* @param filename Filepath to the file containing the code of the shader
		* @param shader_type it must be either GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
		* @return ID of the shader if loading was successfull, 0 otherwise
		*/
		unsigned int GenerateShader(const char* filename, GLenum shader_type);

	public:

		Shader();
		~Shader();

		/**
		* Copy vertex shader file path
		* @param filename File path to be copied
		* @return Always 0
		*/
		int setVertexShader(const char* filename);

		/**
		* Copy fragment shader file path
		* @param filename File path to be copied
		* @return Always 0
		*/
		int setFragmentShader(const char* filename);

		/**
		* Create the program using the provided vertex and fragment shader
		* @return true if the creation was successfull, false otherwise
		*/
		bool CreateProgram(void);

		/**
		* Delete and reload shaders
		* @return true operation was successfull, false otherwise
		*/
		bool ReloadProgram();

		/**
		* Bind the program to use
		*/
		void Bind(void) const;

		/**
		* Unbind the program
		*/
		void Unbind(void) const;

		/**
		* Load the index of the uniform
		* @param uniform Uniform's name
		*/
		void LoadUniform(const std::string uniform);

		/**
		* Access index of the uniform
		* @param key "Index" describing the uniform
		* @return ID of the uniform
		*/
		int operator[](const std::string key);

		/**
		* Acces index of the uniform
		* @param key "Index" describing the uniform
		* @return ID of the uniform
		*/
		int getIndex(const std::string key);

	};

}
#endif // !SHADER_H
