#include "Renderer.h"


namespace Engine {
	Renderer::Renderer(void) {
		m_height = 0;
		m_width = 0;
		m_countinous_time = 0.f;
	}

	Renderer::~Renderer(void) {}

	bool Renderer::Init(int SCREEN_WIDTH, int SCREEN_HEIGHT) {
		
		m_width = SCREEN_WIDTH;
		m_height = SCREEN_HEIGHT;

		// Initialize OpenGL functions

		//Set clear color
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));

		//This enables depth and stencil testing
		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glDepthFunc(GL_LEQUAL));
		GLCall(glClearDepth(1.0f));
		// glClearDepth sets the value the depth buffer is set at, when we clear it

		// Enable back face culling
		GLCall(glEnable(GL_CULL_FACE));
		GLCall(glCullFace(GL_BACK));

		//GLCall(glEnable(GL_BLEND));
		//GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		// open the viewport
		GLCall(glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT)); //we set up our viewport

		bool techniques_initialization = InitRenderingTechniques();
		bool buffers_initialization = InitIntermediateShaderBuffers();
		bool items_initialization = InitCommonItems();
		bool meshes_initialization = InitGeometricMeshes();

		//If everything initialized
		return techniques_initialization && items_initialization && buffers_initialization && meshes_initialization;
	}

	void Renderer::Update(float dt)
	{
		float movement_speed = 2.0f;
		cam.Update(dt);
		m_countinous_time += dt;
	}

	bool Renderer::ReloadShaders(void)
	{
		bool reloaded = true;
		// rendering techniques
		m_program.ReloadProgram();

		return reloaded;
	}

	bool Renderer::ResizeBuffers(int width, int height)
	{
		m_width = width;
		m_height = height;

		cam.SetMatrixes(width, height);

		return true;
	}

	void Renderer::Render(void)
	{
		// Draw the geometry
		RenderGeometry();
	}

}