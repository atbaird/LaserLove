#pragma once

#ifndef SHADERUNIFORMBLOCK_HPP
#define SHADERUNIFORMBLOCK_HPP

#include "Engine/Renderer/Shaders/ShaderProgram.hpp"

class ShaderUniformBlock
{
public:
	ShaderUniformBlock();
	~ShaderUniformBlock();

	static ShaderUniformBlock* CreateOrGetUniformBlock();

	unsigned int m_ubBind;
};
#endif // !SHADERUNIFORMBLOCK_HPP
