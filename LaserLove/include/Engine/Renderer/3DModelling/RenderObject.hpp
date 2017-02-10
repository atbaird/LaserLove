#pragma once

#ifndef RENDEROBJECT_HPP
#define RENDEROBJECT_HPP
#include <string>
#include <vector>
#include <map>

//class RenderObjectTemplate
//{
//public:
//	//model is for the instance
//	//so what do we need - for render meshes
//
//	//Need a:
//	// - Mesh
//	// - Material
//	struct RenderMeshTemplate
//	{
//		std::string mesh_name;
//		std::string material_name;
//	};
//	std::vector<RenderMeshTemplate> render_mesh_templates;
//	static std::map<std::string, RenderObjectTemplate*> g_loaded_templates;
//	static RenderObjectTemplate* Get(std::string name)
//	{
//		std::map<std::string, RenderObjectTemplate*>::iterator it = g_loaded_templates.find(name);
//		if (it != g_loaded_templates.end())
//		{
//			return it->second;
//		}
//		else
//		{
//			RenderObjectTemplate* rot = RenderObjectTemplateFromXML(name);
//			if (nullptr != rot)
//			{
//				g_loaded_templates.insert(std::pair<std::string, RenderObjectTemplate*>(name, rot));
//			}
//			return rot;
//		}
//	}
//};

//class RenderMesh
//{
//public:
//
//};

//class RenderObject
//{
//public:
//	std::vector<RenderMesh*> m_renderMeshes;
//	Matrix44 m_modelMatrix;
//
//	void Render()
//	{
//		for (int i = 0; i < m_renderMeshes.size(); i++)
//		{
//			RenderMesh* renderMesh = m_renderMeshes.at(i);
//			renderMesh->SetMatrix("gModel", m_modelMatrix);
//			renderMesh->Render();
//		}
//	}
//	static RenderObject::Create(const std::string& id)
//	{
//		RenderObjectTemplate* templa = RenderObjectTemplate::Get(id);
//
//		for (int i = 0; i < templa->render_meshes.size(); i++)
//		{
//			RenderMeshTemplate render_mesh_template = templa->at(i);
//			Material* material = Material::Get(render_mesh_template.material_name);
//			Mesh* mesh = Mesh::Get(render_mesh_template.mesh_name);
//			RenderMesh* render_mesh = new RenderMesh(material, mesh);
//			m_renderMeshes.push_back(render_mesh);
//		}
//	}
//};


//Lets us do
//Define RenderObject's as data
//Now has a file; unitychan.xml
/*
<renderobject>
	<rendermesh>
		<mesh>unitychan_body</mesh>
		<material>unitychan_body</material>
	</rendermesh>
	<rendermesh>
		<mesh>unitchan_skin</mesh> //all files in data/mesh ->converts to data/mesh/unitchan_body.mesh
		<material>unitchan_skin</material>
	</rendermesh>
	...
</renderobject>
*/

//Material File Example
//unitchan_body.xml
/*
<material>
	<Shader>default_light</Shader>
	<Texture>
		<uniformname>gTexDiffuse</uniformname>
		<filename>body_01_diff.tga</filename>
	</Texture>
	...
</material>
*/


//Shader file example
//default_light.shader
/*
<shader>
	<vertex>default_light.vert</vertex>
	<frag>default_light.frag</frag>
	<zwrite>true</zwrite>
	<src_blend>one</src_blend>
	...
</shader>
*/
#endif