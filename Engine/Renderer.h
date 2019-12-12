#pragma once
#include "utils.h"
#include "Shader.h"
#include "Camera.h"

namespace Engine {

	class ENGINE_API Renderer {
	protected:
		int m_width;
		int m_height;
		float m_countinous_time;

		Camera cam;

		virtual bool InitRenderingTechniques(void) = 0;
		virtual bool InitIntermediateShaderBuffers(void) = 0;
		virtual bool InitCommonItems(void) = 0;
		virtual bool InitGeometricMeshes(void) = 0;

		Shader m_program;
	public:
		Renderer(void);
		~Renderer(void);
		bool Init(int SCREEN_WIDTH, int SCREEN_HEIGHT);
		virtual void Update(float dt);
		bool ResizeBuffers(int SCREEN_WIDTH, int SCREEN_HEIGHT);
		bool ReloadShaders(void);
		void Render(void);

		// Passes
		virtual void RenderGeometry(void) = 0 ;
	};

}