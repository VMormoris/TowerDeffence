#include "FrameBuffer.h"
#include "utils.h"
namespace Engine {

	FrameBuffer::FrameBuffer(void) {
		m_ID = 0;
		color_texture = NULL;
		depth_texture = NULL;
	}

	FrameBuffer::~FrameBuffer(void) {
		if (m_ID != 0) { GLCall(glDeleteFramebuffers(1, &m_ID)); }
		if (color_texture != NULL) { delete color_texture; }
		if (depth_texture != NULL) { delete depth_texture; }
	}

	void FrameBuffer::Generate(void) {
		//Generate Frame Buffer
		GLCall(glGenFramebuffers(1, &m_ID));
	}

	void FrameBuffer::Resize(int NEW_WIDTH, int NEW_HEIGHT) {
		
		// Color texture
		/*BindTexture(false);
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, NEW_WIDTH, NEW_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
		*/
		AddColorTexture();
		color_texture->Generate();
		color_texture->Bind();
		color_texture->setParameters<int>(GL_CLAMP_TO_EDGE, GL_LINEAR);
		color_texture->SetTextureImage(GL_RGBA16F, NEW_WIDTH, NEW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

		// depth texture
		/*BindTexture();
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, NEW_WIDTH, NEW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
		UnbindTexture();*/
		AddDepthTexture();
		depth_texture->Generate();
		depth_texture->Bind();
		depth_texture->setParameters<int>(GL_CLAMP_TO_EDGE, GL_NEAREST);
		depth_texture->SetTextureImage(GL_DEPTH_COMPONENT24, NEW_WIDTH, NEW_HEIGHT, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		depth_texture->Unbind();
		// framebuffer to link to everything together
		Bind();
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_texture->GetHandle(), 0));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_texture->GetHandle(), 0));
		Unbind();

	}

	void FrameBuffer::Bind(void) const { GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_ID)); }

	void FrameBuffer::Unbind(void) const { GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0)); }

	void FrameBuffer::AddColorTexture(Texture* texture) { 
		if (color_texture != NULL) delete color_texture;
		color_texture = texture;
	}
	void FrameBuffer::AddDepthTexture(Texture* texture) { 
		if (depth_texture != NULL) delete depth_texture;
		depth_texture = texture; 
	}
	void FrameBuffer::BindColorTexture(void) const { color_texture->Bind(); }
	void FrameBuffer::BindDepthTexture(void) const { depth_texture->Bind(); }
}