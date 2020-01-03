#include "../engpch.h"
#include "Scene.h"

namespace Engine {

	Scene::Scene(Camera* camera) {
		cam = camera;
		width = 0; 
		height = 0;
		paused = false;
		timestamp = 0.f;
	}


	Scene::~Scene(void) {
		for (GameObject* entity : gmobjects) delete entity;
	}
	
	
	bool Scene::Init(int SCREEN_WIDTH, int SCREEN_HEIGHT){
		width = SCREEN_WIDTH; height = SCREEN_HEIGHT;
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
		bool light_initialization = InitLightSources();
		bool meshes_initialization = InitGeometricMeshes();
		

		//If everything initialized
		return techniques_initialization && items_initialization && buffers_initialization && meshes_initialization && light_initialization;		
	}


	void Scene::Update(float dt)
	{
		float movement_speed = 2.0f;
		cam->Update(dt);
		if (paused) return;
		timestamp += dt;
	}

	bool Scene::ReloadShaders(void)
	{
		bool reloaded = true;
		// rendering techniques
		geometry_program.ReloadProgram();
		shadowmap_program.ReloadProgram();
		postprocces_program.ReloadProgram();

		return reloaded;
	}

	bool Scene::InitCommonItems(void) {

		float vertices[] = {
			-1.f, -1.f,
			1.f, -1.f,
			-1.f, 1.f,
			1.f, 1.f
		};
		fvao.Generate();
		fvao.Bind();

		fvbo.Generate();
		fvbo.FillBuffer(vertices, 2 * 4 * sizeof(float));

		Attribute twofloats(2);
		fvao.AddAttribute(twofloats);

		fvao.Unbind();

		return true;

	}

	bool Scene::InitIntermediateShaderBuffers(void) {
		intermidiate_buffer.Generate();
		return ResizeBuffers(width, height);
	}

	bool Scene::ResizeBuffers(int width, int height)
	{
		this->width = width;
		this->height = height;

		intermidiate_buffer.Resize(width, height);

		cam->setMatrices(width, height);

		return true;
	}

	void Scene::Render(void)
	{
		//Draw the shadowmap
		RenderShadowmap();

		// Draw the geometry
		RenderGeometry();

		//Draw output Buffer
		RenderOutFB();

	}

	void Scene::ChangePauseState(void) { paused = !paused; }



}
