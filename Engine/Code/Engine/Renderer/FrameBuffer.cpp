#include "Engine/Renderer/FrameBuffer.hpp"
#include "Engine/Renderer/OpenGLExtensions.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

//----------------------------------------------------------------------------------------------
void BindFrameBuffer(FrameBuffer* fbo)
{
	if (fbo == nullptr)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, NULL);
		return;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, fbo->fbo_handle);
}

//----------------------------------------------------------------------------------------------
void FramebufferDelete(FrameBuffer*& fbo)
{
	//if (Renderer::GetBoundFramebuffer() == fbo)
	//{
	//	Renderer::BindFramebuffer(nullptr);
	//}
	glDeleteFramebuffers(1, &(fbo->fbo_handle));
	for (size_t i = 0; i < fbo->color_targets.size(); i++)
	{
		if (fbo->color_targets.at(i) != nullptr)
		{
			delete fbo->color_targets[i];
			fbo->color_targets[i] = nullptr;
		}
	}
	delete[] fbo->color_targetPtr;
	delete fbo->depth_stencil_target;
	delete fbo;
	fbo = nullptr;
}

//----------------------------------------------------------------------------------------------
//count of color targets, All color textures that used as targets, depth stencil texture - optional.
FrameBuffer* FramebufferCreate(const size_t& color_count, Texture** colors, Texture* depth_stencil_target)
{
	//CODE TO PROTECT ME FROM MYSELF.
	ASSERT_OR_DIE(color_count >= 0, "ColorCount for Framebuffer less than or equal to 0.");
	Texture* color0 = colors[0];
	uint32_t width = color0->m_texelSize.x;
	uint32_t height = color0->m_texelSize.y;

	for (uint32_t i = 0; i < color_count; i++)
	{
		Texture* color = colors[i];
		ASSERT_OR_DIE(((uint32_t)color->m_texelSize.x == width) 
			&& ((uint32_t)color->m_texelSize.y == height), "Colors passed to framebuffer have exact same width and height");
	}
	if (nullptr != depth_stencil_target)
	{
		ASSERT_OR_DIE(((uint32_t)depth_stencil_target->m_texelSize.x == width)
			&& ((uint32_t)depth_stencil_target->m_texelSize.y == height), "depth_stencil_target passed to framebuffer has exact same width and height");
	}

	unsigned int fbo_handle;
	glGenFramebuffers(1, &fbo_handle);
	ASSERT_OR_DIE(fbo_handle != NULL, "");

	FrameBuffer* fbo = new FrameBuffer();
	fbo->fbo_handle = fbo_handle;
	fbo->pixel_width = width;
	fbo->pixel_height = height;

	for (uint32_t i = 0; i < color_count; i++)
	{
		fbo->color_targets.push_back(colors[i]);
	}
	fbo->depth_stencil_target = depth_stencil_target;
	fbo->color_targetPtr = colors;

	//OpenGL stuff.
	//if you bound a framebuffer to your Renderer -
	//be careful you didn't unbind just now.
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_handle);

	//Bind our color targets to our FBO
	for (uint32_t i = 0; i < color_count; i++)
	{
		Texture* tex = colors[i];
		glFramebufferTexture(GL_FRAMEBUFFER, //what we're attaching it to
			GL_COLOR_ATTACHMENT0 + i, //where we're attaching to
			tex->m_openglTextureID, // openGL id
			0); //level - probably mipmap level.... eh. :P
	}
	//bind stencil.
	if (nullptr != depth_stencil_target)
	{
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, depth_stencil_target->m_openglTextureID, 0);
	}
	int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		FramebufferDelete(fbo);
		ERROR_AND_DIE("Framebuffer encountered an error.");
	}
	//Revert to old state
	glBindFramebuffer(GL_FRAMEBUFFER, NULL);
	return fbo;
}

FrameBuffer* FramebufferCreate(const uint32_t& width, const uint32_t& height, const std::vector<eTextureFormat>& color_format, const eTextureFormat& depth_stencil_format)
{
	width;
	height;
	color_format;
	depth_stencil_format;
	//Create teh textures
	//then do steps below
	//have FrameBufferDelete also destroy the textures created
	return nullptr;
}