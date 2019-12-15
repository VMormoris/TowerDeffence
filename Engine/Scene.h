#pragma once
#include "utils.h"
#include "Shader.h"
#include "Camera.h"
#include "FrameBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

namespace Engine {

	class ENGINE_API Scene {
	protected:

		bool paused;

		FrameBuffer intermediate_buffer;

		VertexBuffer vb_fbo;
		VertexArray va_fbo;

		int m_width;
		int m_height;

		float m_countinous_time;

		Camera* cam;

		bool InitIntermediateShaderBuffers(void);
		bool InitCommonItems(void);
		virtual bool InitRenderingTechniques(void) = 0;
		virtual bool InitGeometricMeshes(void) = 0;
		virtual bool InitLightSources(void) = 0;

		Shader m_program, postprocces_program, shadowmap_program;
		
	public:
		Scene(void);
		~Scene(void);
		bool Init(int SCREEN_WIDTH, int SCREEN_HEIGHT);
		virtual void Update(float dt);
		bool ResizeBuffers(int SCREEN_WIDTH, int SCREEN_HEIGHT);
		bool ReloadShaders(void);
		void Render(void);

		// Passes
		virtual void RenderShadowmap(void) = 0;
		virtual void RenderGeometry(void) = 0 ;
		virtual void RenderOutFB(void) = 0;

		void ChangePauseState(void);
	};

}