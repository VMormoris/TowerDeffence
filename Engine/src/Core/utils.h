#pragma once
#ifndef UTILS_H
#define UTILS_H
#include "../../engpch.h"
#include "Core.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) Engine::utils::GLClearError();\
	x;\
	ASSERT(Engine::utils::GLLogCall(#x, __FILE__, __LINE__))

namespace Engine{
	namespace utils {

		/**
		* Printing GPU details
		*/
		ENGINE_API void GPU_INFO(void);
		/**
		*	Used for cleaning OpenGL stack of errors;
		*/
		ENGINE_API void GLClearError();

		/**
		*	Used for outputing in human readable form OpenGL Error flags
		*/
		ENGINE_API bool GLLogCall(const char* function, const char* file, int line);

		/**
		* Create a window inorder to put the graphics from OpenGL using SDL
		* @param WINDOW_NAME the title displayed for the window
		* @param WINDOW_WIDTH the desired width of the window
		* @param WINDOW_HEIGHT the desired height of the window
		* @param (out) gContext Loads the the context of the created window
		* @return A pointer to SDL_Window that describes the created window or null if failed
		*/
		ENGINE_API SDL_Window* CreateWindow(const char* WINDOW_NAME, int WINDOW_WIDTH, int WINDOW_HEIGHT, SDL_GLContext& gContext);


		/**
		* Create a Full screen window inorder to put the graphics from OpenGL using SDL
		* @param WINDOW_NAME the title displayed for the window
		* @param (out) gContext Loads the the context of the created window
		* @return A pointer to SDL_Window that describes the created window or null if failed
		*/
		ENGINE_API SDL_Window* CreateFullscreenWindow(const char* WINDOW_NAME, SDL_GLContext& gContext);

		/**
		* Read files as a string(c style)
		* @param filename The Filepath that you want to read
		* @return The content of the file as string(c style)
		*/
		ENGINE_API char* readfileAsString(const char* filename);

		//ENGINE_API unsigned int getFrameBufferStatus(unsigned int framebufferID);
		
		/**
		* Extract the path to the folder where filename is located
		* @param filename path to a file
		* @return A string that contains the path to the folder of a file
		*/
		ENGINE_API std::string GetFolderPath(const char* filename);

		/**
		* Check if two strings are equals (Case Insesitive)
		* @param str1 First string for equality check
		* @param str2 Second string for equality check
		* @return true if the two strings are equals, false otherwise
		*/
		ENGINE_API bool compareStringIgnoreCase(std::string str1, std::string str2);

		/**
		* Tranforms World Coordinates to Screen Coordinates
		* @param coords Coordinates on the World
		* @param projection Projection matrix
		* @param view View matrix
		* @return Coordinates on Screen
		*/
		ENGINE_API glm::vec2 project(glm::vec3 coords, glm::mat4 projection, glm::mat4 view, float WIDTH, float HEIGHT);

		/**
		* Function to calculate Distance Between a point and a Box
		* @param mins The minimum values as the coordinates of the Box
		* @param maxes The maximum values as the coordinates of the Box
		* @param point Coordinates of point
		* @return The distance between the given box and point
		*/
		ENGINE_API float BoxPointDistance(glm::vec3 mins, glm::vec3 maxes, glm::vec3 point);

	}

}

#endif // !UTILS_H
