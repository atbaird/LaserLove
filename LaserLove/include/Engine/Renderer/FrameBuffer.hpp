#pragma once
#include "Engine/Renderer/Texture.hpp"
#include <vector>

#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

//glGenFrameBuffers
//glBindFrameBuffer
//glFramebufferTexture
//glCheckFramebufferStatus
//glDeleteFrameBuffers
//
//
//*glDrawBuffer //Sets single render Target
//glDrawBuffers //Sets render targets


class FrameBuffer
{
public:
	unsigned int fbo_handle;

	Texture** color_targetPtr;
	std::vector<Texture*> color_targets;
	Texture* depth_stencil_target;

	uint32_t pixel_width;
	uint32_t pixel_height;

	Texture* get_color_texture(size_t idx) const 
	{
		//if (idx > color_targets.size())
		//{
		//	return nullptr;
		//}
		return color_targets[idx];
	}
	Texture* get_depth_stencil_target()
	{
		return depth_stencil_target;
	}
};

void BindFrameBuffer(FrameBuffer* fbo);
void FramebufferDelete(FrameBuffer*& fbo);
FrameBuffer* FramebufferCreate(const size_t& color_count, Texture** colors, Texture* depth_stencil_target);
FrameBuffer* FramebufferCreate(const uint32_t& width, const uint32_t& height, const std::vector<eTextureFormat>& color_format, const eTextureFormat& depth_stencil_format);

#endif