#pragma once
#ifndef SCENE_H
#define SCENE_H
#include "Camera.h"
#include "FrameBuffer.h"
#include "GameObject.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

namespace Engine {

	class ENGINE_API Scene : public EventDispatcher{
	protected:
		/**
		* Main camera used for this scene
		*/
		Camera* cam;

		int width;
		int height;

		VertexBuffer fvbo;
		VertexArray fvao;

		/**
		* Frame buffer used for intermidate rendering
		*/
		FrameBuffer intermidiate_buffer;

		/**
		* Placeholder for all the GameObjects in our Scene
		*/
		std::vector<GameObject*> gmobjects;


		bool InitIntermediateShaderBuffers(void);
		bool InitCommonItems(void);
		virtual bool InitRenderingTechniques(void) = 0;
		virtual bool InitGeometricMeshes(void) = 0;
		virtual bool InitLightSources(void) = 0;

		Shader shadowmap_program, geometry_program, postprocces_program;
		
		bool paused;

		float timestamp;

	public:

		Scene(Camera* camera);
		~Scene(void);

		/**
		* Render Scene
		*/
		void Render(void);

		/**
		* Initialize Scene
		*/
		bool Init(int SCREEN_WIDTH, int SCREEN_HEIGHT);



		bool ResizeBuffers(int SCREEN_WIDTH, int SCREEN_HEIGHT);
		
		bool ReloadShaders(void);

		/**
		* Pass for rendering shadowmap
		*/
		virtual void RenderShadowmap(void) = 0;
		/**
		* Pass for rendering geometries
		*/
		virtual void RenderGeometry(void) = 0;
		/**
		* Final pass rendering on screen
		*/
		virtual void RenderOutFB(void) = 0;
		/**
		* Update Assets inside scene
		*/
		virtual void Update(float dt);

		/**
		* Toogle paused
		*/
		void ChangePauseState(void);

	};

}

#endif // !SCENE_H
