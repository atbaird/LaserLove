#pragma once
#include "Engine/Renderer/RendererEnums.hpp"

#ifndef RENDERSTATE_HPP
#define RENDERSTATE_HPP

class RenderState
{
public:
	RenderState();

	PrimitiveType shapeType;
	VBOusage drawMode;
	DataType iboDataType;
	bool enableDepthTest;
	bool enableCulling;
	bool enableWriteToDepthBuffer;
};
#endif