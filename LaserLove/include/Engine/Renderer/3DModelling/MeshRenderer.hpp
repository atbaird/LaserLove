#pragma once

#ifndef MESHRENDERER_HPP
#define MESHRENDERER_HPP
#include "Engine/Renderer/3DModelling/Material.hpp"
#include "Engine/Renderer/3DModelling/Mesh.hpp"

class MeshRenderer
{
private:
	//Variables
	Mesh* m_mesh;
	Material* m_mat;
public:
	//Constructors
	MeshRenderer(Mesh* mesh = nullptr, Material* mat = nullptr);
	~MeshRenderer();

	//Setters
	void SetMesh(Mesh* mesh);
	void SetMaterial(Material* mat);

	//Getters
	Mesh* GetMesh() const;
	Material* GetMaterial() const;

	//Renders
	void Render() const;
	static void Render(Mesh* mesh, Material* mat);

};

#endif