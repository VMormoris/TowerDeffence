#include "utils.h"
#include "glm.hpp"
#include <algorithm>
#include <iostream>
#include <fstream>
//#include <sstream>

#define START_WIDTH 2
#define START_HEIGHT 30
//	SDL_WINDOWPOS_UNDEFINED


namespace Engine {
	namespace utils {
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

		unsigned int getFrameBufferStatus(unsigned int framebufferID) {
			GLCall(glBindFramebuffer(GL_FRAMEBUFFER, framebufferID));
			unsigned int status;
			GLCall(status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
			if (status != GL_FRAMEBUFFER_COMPLETE)
			{
				printf("glCheckFramebufferStatus: error %u\n", status);
				switch (status)
				{
				case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
					printf("Incomplete attatchement\n");
					break;
				case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
					printf("Incomplete missing attachment\n");
					break;
				case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
					printf("Incomplete draw buffer\n");
					break;
				case GL_FRAMEBUFFER_UNSUPPORTED:
					printf("Unsupported\n");
					break;
				case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
					printf("Incomplete layer targets\n");
					break;
				default:
					printf("Default error\n");
					break;
				}
			}
			GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
			return status;
		}

		std::string tolowerCase(std::string str)
		{
			std::transform(str.begin(), str.end(), str.begin(), ::tolower);
			return str;
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
	}

	/*
	Mesh* loadObject(const char* filename) {

		printf("Start ObjReading MeshNext reading\n");

		std::vector<glm::vec3>sharedvertices;// .clear();
		std::vector<glm::vec3>sharednormals;// .clear();
		std::vector<glm::vec2>sharedtextcoord;// .clear();
		std::vector<unsigned int>elements;// .clear();
		std::vector<Face>sharedfaces;

		bool hasTextures = false; bool hasNormals = false;

		char buff[1024];
		char str[1024];

		FILE* pFile;
		std::string folderPath = utils::GetFolderPath(filename);
		pFile = fopen(filename, "r");
		if (pFile == NULL)
		{
			printf("ObjLoaderMeshNext: Error opening file %s \n", filename);
			return nullptr;
		}

		Mesh* mesh = new Mesh();

		int str_pos;

		// add a default material
		mesh->materials.push_back(Material());

		//add a default meshObject
		Object defaultOb;
		defaultOb.start = 0;
		defaultOb.material_id = 0;
		mesh->objects.push_back(defaultOb);
		int currentMaterialID = 0;

		//read the file
		while (fgets(buff, 1024, pFile) != NULL)
		{
			if (sscanf_s(buff, "%s %n", str, &str_pos) >= 1)
			{
				if (strcmp(str, "v") == 0) read_vertex(buff + str_pos, sharedvertices);
				else if (strcmp(str, "vt") == 0) read_texcoord(buff + str_pos, sharedtextcoord);
				else if (strcmp(str, "vn") == 0) read_normal(buff + str_pos, sharednormals);
				else if (strcmp(str, "f") == 0) read_faceLimited(buff + str_pos, sharedvertices, sharedtextcoord, sharednormals, elements, sharedfaces);
				else if (strcmp(str, "usemtl") == 0) read_usemtl(buff + str_pos, currentMaterialID, sharedfaces.size(), *mesh);
				else if (strcmp(str, "mtllib") == 0) read_mtllib(buff + str_pos, folderPath, *mesh);
				else if (strcmp(str, "g") == 0 || strcmp(str, "o") == 0) add_new_group(buff + str_pos, currentMaterialID, sharedfaces.size(), *mesh);
				else if (strcmp(str, "#") == 0) { /* ignoring this line *//* }
				else { /* ignoring this line *//* }
			}
		}
		fclose(pFile);

		// Generate vertices and other data from faces
		generateDataFromFaces(*mesh, sharedvertices, sharedtextcoord, sharednormals, sharedfaces);

		// close the last object
		mesh->objects.back().end = mesh->pos_index;

		printf("Done reading OBJ file \n");

		// remove empty objects
		mesh->objects.erase(std::remove_if(mesh->objects.begin(), mesh->objects.end(), [](Object ob) { return ob.start == ob.end; }), mesh->objects.end());

		// if normals doesn't exist create them
		if (mesh->norm_index == 0)
		{
			printf("normals not found\n");

			bool flatShading = false;
			//if (flatShading) calculate_flat_normals();
			//else calculate_avg_normals(sharedvertices, mesh->norms, elements);
		}

		sharedvertices.clear();
		sharednormals.clear();
		sharedtextcoord.clear();
		elements.clear();
		sharedfaces.clear();

		mesh->setBuffers();

		return mesh;
	}
	*/
	
}


