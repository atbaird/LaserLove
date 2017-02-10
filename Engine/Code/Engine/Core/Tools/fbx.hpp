#pragma once
#if !defined(__TOOLS_FBX__)
#define __TOOLS_FBX__

//Includes
#include "Engine/Console/Console.hpp"
#include "Engine/Core/Tools/fbx.hpp"
#include "Engine/Renderer/3DModelling/MeshBuilder.hpp"
#include "Engine/Renderer/3DModelling/Skeleton.hpp"
#include "Engine/Renderer/3DModelling/Matrix44Stack.hpp"
#include "Engine/Renderer/3DModelling/Motion.hpp"
#include <vector>

//Classes
class SceneImport
{
public:
	std::vector<MeshBuilder> meshes;
	std::vector<Skeleton*> skeletons;
	std::vector<Motion*> motions;
};


//Function Declarations
void FbxListScene(char const *filename);
SceneImport* FbxLoadSceneFromFile(const char* fbx_filename, const Matrix44& engine_basis, bool is_engine_basis_right_handed, const Matrix44& transform);

#endif