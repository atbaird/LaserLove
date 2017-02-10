#include "Engine/Renderer/3DModelling/MeshRenderer.hpp"

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//Constructors
MeshRenderer::MeshRenderer(Mesh* mesh, Material* mat)
	: m_mesh(mesh),
	m_mat(mat)
{

}
MeshRenderer::~MeshRenderer()
{
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//Setters
void MeshRenderer::SetMesh(Mesh* mesh)
{
	m_mesh = nullptr;
	m_mesh = mesh;
}
void MeshRenderer::SetMaterial(Material* mat)
{
	m_mat = nullptr;
	m_mat = mat;
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//Getters
Mesh* MeshRenderer::GetMesh() const
{
	return m_mesh;
}
Material* MeshRenderer::GetMaterial() const
{
	return m_mat;
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//Renders
void MeshRenderer::Render() const
{
	MeshRenderer::Render(m_mesh, m_mat);
}
void MeshRenderer::Render(Mesh* mesh, Material* mat)
{
	if (mesh == nullptr || mat == nullptr)
	{
		return;
	}
	mesh->DrawWith(mat);
}