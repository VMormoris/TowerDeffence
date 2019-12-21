#include "engpch.h"
#include "utils.h"



#define START_WIDTH 2//Used for anchoring window on the left of the screen on my PC
#define START_HEIGHT 30//Used for anchoring window on the top of the screen on my PC

/**
* (Helper function) Converts Upper case letters to lower
* @param str string to be converted
* @return The converted string
*/
std::string tolowerCase(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}


namespace Engine {
	namespace utils {

		void GPU_INFO(void){
			const unsigned char* vendor;
			const unsigned char* renderer;
			GLCall(vendor = glGetString(GL_VENDOR));
			GLCall(renderer = glGetString(GL_RENDERER));
			std::cout << "Manifacturer: " << vendor << std::endl << "Model: " << renderer << std::endl << std::endl;
		}

		void GLClearError() {
			while (glGetError() != GL_NO_ERROR);//Calling glGetError in loop in order to make sure that no more flags exist
		}

		bool GLLogCall(const char* function, const char* file, int line) {
			while (GLenum error = glGetError()) {//Loop until no more Error flags
				std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ": " << line << std::endl;//Output function name, file name and line where the error occuried
				return false;
			}
			return true;
		}

		//Copy code from the lab
		SDL_Window* CreateWindow(const char* WINDOW_NAME, int WINDOW_WIDTH, int WINDOW_HEIGHT, SDL_GLContext& gContext) {
			//Initialize SDL
			if (SDL_Init(SDL_INIT_EVERYTHING) < 0) return NULL;

			//use Double Buffering
			if (SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) == -1) std::cout << "Error: No double buffering" << std::endl;

			//set OpenGL Version to 3.3
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

			//Create Window
			SDL_Window* window = SDL_CreateWindow(WINDOW_NAME, START_WIDTH, START_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
			if (window == NULL) {
				std::cout << "Could not create window: " << SDL_GetError() << std::endl;
				return NULL;
			}

			//Create Context
			gContext = SDL_GL_CreateContext(window);
			if (gContext == NULL) {
				std::cout << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << std::endl;
				return NULL;
			}

			// Disable Vsync
			if (SDL_GL_SetSwapInterval(0) == -1) std::cout << "Warning: Unable to disable VSync! SDL Error: " << SDL_GetError() << std::endl;

			//Load GLEW
			GLenum err;
			GLCall(err = glewInit());
			if (err != GLEW_OK) {
				std::cout << "Error loading GLEW" << std::endl;
				return NULL;
			}

			return window;
		}

		SDL_Window* CreateFullscreenWindow(const char* WINDOW_NAME, SDL_GLContext& gContext) {
			//Initialize SDL
			if (SDL_Init(SDL_INIT_EVERYTHING) < 0) return NULL;

			//use Double Buffering
			if (SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) == -1) std::cout << "Error: No double buffering" << std::endl;

			//set OpenGL Version to 3.3
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

			//Create Window
			SDL_Window* window = SDL_CreateWindow(WINDOW_NAME, START_WIDTH, START_HEIGHT, 0, 0, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);
			if (window == NULL) {
				std::cout << "Could not create window: " << SDL_GetError() << std::endl;
				return NULL;
			}

			//Create Context
			gContext = SDL_GL_CreateContext(window);
			if (gContext == NULL) {
				std::cout << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << std::endl;
				return NULL;
			}

			// Disable Vsync
			if (SDL_GL_SetSwapInterval(0) == -1) std::cout << "Warning: Unable to disable VSync! SDL Error: " << SDL_GetError() << std::endl;

			//Load GLEW
			GLenum err;
			GLCall(err = glewInit());
			if (err != GLEW_OK) {
				std::cout << "Error loading GLEW" << std::endl;
				return NULL;
			}

			return window;
		}

		//Using code from the labratory
		char* readfileAsString(const char* filename) {
			// C++ code 
			std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
			if (!in.is_open())
				return nullptr;
			size_t length = in.tellg();

			in.seekg(0, in.beg);

			char* buffer = new char[length + 1];
			in.read(buffer, length);
			buffer[length] = '\0';

			if (!in)
			{
				std::cout << "Error occuried while reading" << std::endl;
				// error in reading the file
				delete[] buffer;
				buffer = nullptr;
			}
			in.close();

			return buffer;
		}

		bool compareStringIgnoreCase(std::string str1, std::string str2)
		{
			str1 = tolowerCase(str1);
			str2 = tolowerCase(str2);
			return (str2.compare(str1) == 0);
		}

		std::string GetFolderPath(const char* filename)
		{
			std::string str(filename);
			size_t found;
			found = str.find_last_of("/\\");
			return str.substr(0, found + 1);
		}

		glm::vec2 project(glm::vec3 coords, glm::mat4 projection, glm::mat4 view, float WIDTH, float HEIGHT) {
			glm::vec4 temp = projection * (view * glm::vec4(coords, 1.0));
			glm::vec3 ndcSpacePos = glm::vec3(temp.x, temp.y, temp.z) / temp.w;
			glm::vec2 data = ((glm::vec2(ndcSpacePos.x, ndcSpacePos.y) + 1.0f) / 2.0f) * glm::vec2(WIDTH, HEIGHT) + glm::vec2(0.f);
			return data;
		}


	}
	
}


