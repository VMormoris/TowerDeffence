#pragma once
#ifndef FRAME_BUFFER
#define FRAME_BUFFER

#include "GLObject.h"
#include "Texture.h"

namespace Engine {

	class ENGINE_API FrameBuffer : public GLObject {
	private:

		Texture* color_texture;
		Texture* depth_texture;

	public:

		FrameBuffer(void);
		~FrameBuffer(void);
		void Generate(void) override;
		void AddColorTexture(Texture* texture = new Texture("defaultColorText", false)); 
		void AddDepthTexture(Texture* texture = new Texture("defaultDolorText", false));
		void Bind(void) const;
		void Unbind(void) const;
		void Resize(int NEW_WIDTH, int NEW_HEIGHT);
		void BindColorTexture(void)const;
		void BindDepthTexture(void)const;
	};

}

#endif // !FRAME_BUFFER
