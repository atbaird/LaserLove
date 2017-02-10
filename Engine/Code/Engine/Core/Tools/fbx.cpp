#include "Engine/Core/Tools/fbx.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

//#define UNREFERENCED(x) (void)x

#if defined(TOOLS_BUILD)
	#include <fbxsdk.h>
	#pragma comment(lib, "libfbxsdk-md.lib")
//For TOOLS only

	//!-----------------
//ToEngineVector4
	static const Vector4 ToEngineVector4(const FbxDouble4& vec4)
	{
		Vector4 newVec4((float)vec4.mData[0], (float)vec4.mData[1], (float)vec4.mData[2], (float)vec4.mData[3]);
		return newVec4;
	}

	//!-----------------
	//ToEngineMatrix
	static const Matrix44 ToEngineMatrix(const FbxAMatrix& mat)
	{
		Matrix44 newMat;

		//Fbxsdk does row major and row memory.
		newMat.MatrixSetRows(ToEngineVector4(mat.mData[0]), ToEngineVector4(mat.mData[1]), ToEngineVector4(mat.mData[2]), ToEngineVector4(mat.mData[3]));
		//if doesn't work as is, try transposing.

		return newMat;
	}

	//!------------------
	//GetSceneBasis
	const Matrix44 GetSceneBasis(FbxScene* scene)
	{
		fbxsdk::FbxAxisSystem axis_system = scene->GetGlobalSettings().GetAxisSystem();
		FbxAMatrix mat;
		axis_system.GetMatrix(mat);

		Matrix44 basis = ToEngineMatrix(mat);
		return basis;
	}

	//!------------------
	//GetAttributeTypeName
	static const char* GetAttributeTypeName(FbxNodeAttribute::EType type)
	{
		switch (type) 
		{
		case FbxNodeAttribute::eUnknown: return "unidentified";
		case FbxNodeAttribute::eNull: return "null";
		case FbxNodeAttribute::eMarker: return "marker";
		case FbxNodeAttribute::eSkeleton: return "skeleton";
		case FbxNodeAttribute::eMesh: return "mesh";
		case FbxNodeAttribute::eNurbs: return "nurbs";
		case FbxNodeAttribute::ePatch: return "patch";
		case FbxNodeAttribute::eCamera: return "camera";
		case FbxNodeAttribute::eCameraStereo: return "stereo";
		case FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
		case FbxNodeAttribute::eLight: return "light";
		case FbxNodeAttribute::eOpticalReference: return "optical reference";
		case FbxNodeAttribute::eOpticalMarker: return "marker";
		case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
		case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
		case FbxNodeAttribute::eBoundary: return "boundary";
		case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
		case FbxNodeAttribute::eShape: return "shape";
		case FbxNodeAttribute::eLODGroup: return "lodgroup";
		case FbxNodeAttribute::eSubDiv: return "subdiv";
		default: return "unknown"; }
	}

	//LOCAL FUNCTIONS
	static void PrintAttribute(FbxNodeAttribute *attribute, int depth)
	{
		if (nullptr == attribute)
		{
			return;
		}
		FbxNodeAttribute::EType type = attribute->GetAttributeType();

		const char *typeName = GetAttributeTypeName(type);
		const char *attributeName = attribute->GetName();

		DebuggerPrintf("%*s- type='%s', name='%s'", depth, " ", typeName, attributeName);
	}

	static void PrintNode(FbxNode * node, int depth)
	{
		DebuggerPrintf("%*sNode [%s]", depth, " ", node->GetName());

		for (int32_t i = 0; i < node->GetChildCount(); ++i)
		{
			PrintNode(node->GetChild(i), depth + 1);
		}
	}

	//EXTERNAL FUNCTIONS
	//For TOOLS only
	void FbxListScene(char const *filename)
	{
		//
		FbxManager *fbx_manager = FbxManager::Create();
		if (nullptr == fbx_manager) {
			DebuggerPrintf("Could not create fbx manager.");
			return;
		}
		FbxIOSettings *io_settings = FbxIOSettings::Create(fbx_manager, IOSROOT);

		fbx_manager->SetIOSettings(io_settings);

		//
		//Create an importer
		FbxImporter *importer = FbxImporter::Create(fbx_manager, "");
		bool load_successful = importer->Initialize(filename, -1, fbx_manager->GetIOSettings());

		if (load_successful)
		{
			FbxScene *scene = FbxScene::Create(fbx_manager, "");
			bool import_successful = importer->Import(scene);
			if (import_successful)
			{
				FbxNode *root = scene->GetRootNode();
				PrintNode(root, 0);
			}
			FBX_SAFE_DESTROY(scene)
		}
		else
		{
			DebuggerPrintf("Could not import scene %s", filename);
		}

		bool result = importer->Initialize(filename, -1, fbx_manager->GetIOSettings());
		if (result)
		{
			//we have imported the FBX
		}
		else
		{
			DebuggerPrintf("Could not import scene: %s", filename);
		}

		FBX_SAFE_DESTROY(importer);
		FBX_SAFE_DESTROY(io_settings);
		FBX_SAFE_DESTROY(fbx_manager);
	}


	static Vector3 ToEngineVector3(FbxVector4 fbx_pos)
	{
		return Vector3((float)fbx_pos.mData[0], (float)fbx_pos.mData[1], (float)fbx_pos.mData[2]);
	}

	//!----------------
	//
	static bool GetPosition(Vector3* out_position, const Matrix44& transform, FbxMesh* mesh, int poly_idx, int vert_idx)
	{
		if (mesh == nullptr || out_position == nullptr)
		{
			return false;
		}
		FbxVector4 fbx_pos;
		int control_idx = mesh->GetPolygonVertex(poly_idx, vert_idx);

		fbx_pos = mesh->GetControlPointAt(control_idx);
		Vector4 fbxPosVec4 = ToEngineVector4(fbx_pos);
		fbxPosVec4.w = 1.f;
		fbxPosVec4 = fbxPosVec4 * transform;

		*out_position = fbxPosVec4.XYZ();
		return true;
		
	}

	template<typename ElemType, typename VarType>
	static const bool GetObjectFromElement(FbxMesh* mesh, const Matrix44& Transform, int poly_idx, int vert_idx, int uv_idx, ElemType* elements, VarType* out_var)
	{
		if (nullptr == elements)
		{
			return false;
		}

		int control_idx = mesh->GetPolygonVertex(poly_idx, vert_idx);
		int direct_vertex_idx = (poly_idx * 3) + vert_idx;
		switch (elements->GetMappingMode())
		{
		case FbxGeometryElement::eByControlPoint:
			switch (elements->GetReferenceMode())
			{
					case FbxGeometryElement::eDirect:
						if (control_idx < elements->GetDirectArray().GetCount())
						{
							*out_var = elements->GetDirectArray().GetAt(control_idx);
							return true;
						}
						break;
					case FbxGeometryElement::eIndexToDirect:
						if (control_idx < elements->GetIndexArray().GetCount())
						{
							int index = elements->GetIndexArray().GetAt(control_idx);
							*out_var = elements->GetDirectArray().GetAt(index);
							return true;
						}
						break;
					default:
						break;
			}
			break;
			case FbxGeometryElement::eByPolygonVertex:
				switch (elements->GetReferenceMode())
				{
					case FbxGeometryElement::eDirect:
						if (direct_vertex_idx < elements->GetDirectArray().GetCount())
						{
							*out_var = elements->GetDirectArray().GetAt(direct_vertex_idx);
							return true;
						}
						break;
					case FbxGeometryElement::eIndexToDirect:
						if (direct_vertex_idx < elements->GetIndexArray().GetCount())
						{
							int index = elements->GetIndexArray().GetAt(direct_vertex_idx);
							*out_var = elements->GetDirectArray().GetAt(index);
							return true;
						}
						break;
					default:
						break;
			}
			break;
		default:
			break;
		}
		return false;
	}
	


	static const bool GetUV(Vector2* out_uv, const Matrix44& transform, FbxMesh* mesh, int poly_idx, int vert_idx, int uv_idx)
	{
		FbxVector2 uv;
		FbxGeometryElementUV* uvs = mesh->GetElementUV(uv_idx);

		if (GetObjectFromElement(mesh, transform, poly_idx, vert_idx, uv_idx, uvs, &uv))
		{
			*out_uv = Vector2((float)uv.mData[0], (float)uv.mData[1]);
			return true;
		}

		return false;
	}


	static const bool GetNormal(Vector3* out_normal, const Matrix44& transform, FbxMesh* mesh, int poly_idx, int vert_idx, int uv_idx)
	{
		FbxVector4 normal;
		FbxGeometryElementNormal *uvs = mesh->GetElementNormal();
		if (GetObjectFromElement(mesh, transform, poly_idx, vert_idx, uv_idx, uvs, &normal))
		{
			Vector4 normVec4 = ToEngineVector4(normal);
			normVec4.w = 0.f;
			normVec4 = normVec4 * transform;
			*out_normal = normVec4.XYZ();
			return true;
		}

		return false;
	}
	static const bool GetTangent(Vector3* out_tangent, const Matrix44& transform, FbxMesh* mesh, int poly_idx, int vert_idx, int uv_idx)
	{
		FbxVector4 tangent;
		FbxGeometryElementTangent *uvs = mesh->GetElementTangent();
		if (GetObjectFromElement(mesh, transform, poly_idx, vert_idx, uv_idx, uvs, &tangent))
		{
			Vector4 tangVec4 = ToEngineVector4(tangent);
			tangVec4.w = 0.f;
			tangVec4 = tangVec4 * transform;
			*out_tangent = tangVec4.XYZ();
			return true;
		}

		return false;
	}
	static const bool GetBitangent(Vector3* out_bitangent, const Matrix44& transform, FbxMesh* mesh, int poly_idx, int vert_idx, int uv_idx)
	{
		FbxVector4 bitangent;
		FbxGeometryElementBinormal *uvs = mesh->GetElementBinormal();
		if (GetObjectFromElement(mesh, transform, poly_idx, vert_idx, uv_idx, uvs, &bitangent))
		{
			Vector4 bitangVec4 = ToEngineVector4(bitangent);
			bitangVec4.w = 0.f;
			bitangVec4 = bitangVec4 * transform;
			*out_bitangent = bitangVec4.XYZ();
			return true;
		}

		return false;
	}
	static const bool GetColor(Rgba* out_color, const Matrix44& transform, FbxMesh* mesh, int poly_idx, int vert_idx, int uv_idx)
	{
		FbxColor color;
		FbxGeometryElementVertexColor *uvs = mesh->GetElementVertexColor();
		if (GetObjectFromElement(mesh, transform, poly_idx, vert_idx, uv_idx, uvs, &color))
		{
			*out_color = Rgba((float)color.mRed, (float)color.mGreen, (float)color.mBlue, (float)color.mAlpha);
			return true;
		}

		return false;
	}
	static const bool GetTextureLocation()
	{

		return false;
	}
	//!----------------
	//
	static void ImportVertex(MeshBuilder* mb, const Matrix44& transform, FbxMesh* mesh, int poly_idx, int vert_idx, std::vector<skin_weight_t>& skin_weights)
	{
		Vector3 normal;
		if (GetNormal(&normal, transform, mesh, poly_idx, vert_idx, 0))
		{
			mb->SetNormal(normal);
		}


		Vector2 uv;
		if (GetUV(&uv, transform, mesh, poly_idx, vert_idx, 0))
		{
			mb->SetTexCoord0(uv);
		}

		Rgba color = Rgba(1.f,1.f,1.f,1.f);
		if (GetColor(&color, transform, mesh, poly_idx, vert_idx, 0))
		{
			mb->SetColor(color);
		}

		Vector3 tangent;
		if (GetTangent(&tangent, transform, mesh, poly_idx, vert_idx, 0))
		{
			mb->SetTangent(tangent);
		}
		Vector3 bitangent;
		if (GetBitangent(&bitangent, transform, mesh, poly_idx, vert_idx, 0))
		{
			mb->SetBitangent(bitangent);
		}

		int control_idx = mesh->GetPolygonVertex(poly_idx, vert_idx);
		if ((size_t)control_idx < skin_weights.size())
		{
			mb->SetBoneWeightsAndIndices(skin_weights[control_idx].bone_weights, skin_weights[control_idx].bone_indices);
			mb->NormalizeBoneWeights();
		}
		else
		{
			mb->SetBoneWeightsAndIndicesDefault();
		}

		Vector3 position;
		if (GetPosition(&position, transform, mesh, poly_idx, vert_idx))
		{
			mb->AddVertex(position);
		}

		
	}


	static const Matrix44 GetGeometricTransform(FbxNode* node)
	{
		Matrix44 toReturn(true);
		if (node != nullptr && (node->GetNodeAttribute() != nullptr))
		{
			const FbxVector4 geo_trans = node->GetGeometricTranslation(FbxNode::eSourcePivot);
			const FbxVector4 geo_rot = node->GetGeometricRotation(FbxNode::eSourcePivot);
			const FbxVector4 geo_scale = node->GetGeometricScaling(FbxNode::eSourcePivot);

			FbxAMatrix geo_mat;
			geo_mat.SetTRS(geo_trans, geo_rot, geo_scale);

			toReturn = ToEngineMatrix(geo_mat);
		}

		return toReturn;
	}


	//!----------------
	//
	static bool HasSkinWeights(const FbxMesh* mesh)
	{
		int deformer_count = mesh->GetDeformerCount(FbxDeformer::eSkin);
		return (deformer_count > 0);
	}

	static void AddHighestWeight(skin_weight_t* skin_weight, int joint_idx, float weight)
	{

		//Exercise to the Reader
		//Replace the lowest value with what was sent in if you are higher
		//than the lower value.
		int lowestIndex = -1;
		float currentLowest = 9999;
		bool setOnce = false;

		if (((setOnce == false && skin_weight->bone_weights.x < weight)
			|| (setOnce == true && skin_weight->bone_weights.x < currentLowest)))
		{
			setOnce = true;
			lowestIndex = 0;
			currentLowest = skin_weight->bone_weights.x;
			//skin_weight->bone_indices.x = joint_idx;
			//skin_weight->bone_weights.x = weight;
		}
		if (((setOnce == false && skin_weight->bone_weights.y < weight)
			|| (setOnce == true && skin_weight->bone_weights.y < currentLowest)))
		{
			setOnce = true;
			lowestIndex = 1;
			currentLowest = skin_weight->bone_weights.y;
			//skin_weight->bone_indices.y = joint_idx;
			//skin_weight->bone_weights.y = weight;
		}
		if (((setOnce == false && skin_weight->bone_weights.z < weight)
			|| (setOnce == true && skin_weight->bone_weights.z < currentLowest)))
		{
			setOnce = true;
			lowestIndex = 2;
			currentLowest = skin_weight->bone_weights.z;
			//skin_weight->bone_indices.z = joint_idx;
			//skin_weight->bone_weights.z = weight;
		}
		if (((setOnce == false && skin_weight->bone_weights.w < weight)
			|| (setOnce == true && skin_weight->bone_weights.w < currentLowest)))
		{
			setOnce = true;
			lowestIndex = 3;
			currentLowest = skin_weight->bone_weights.w;
			//skin_weight->bone_indices.w = joint_idx;
			//skin_weight->bone_weights.w = weight;
		}

		switch (lowestIndex)
		{
		case 0:
			skin_weight->bone_weights.x = weight;
			skin_weight->bone_indices.x = joint_idx;
			break;
		case 1:
			skin_weight->bone_weights.y = weight;
			skin_weight->bone_indices.y = joint_idx;
			break;
		case 2:
			skin_weight->bone_weights.z = weight;
			skin_weight->bone_indices.z = joint_idx;
			break;
		case 3:
			skin_weight->bone_weights.w = weight;
			skin_weight->bone_indices.w = joint_idx;
			break;
		default:
			break;
		}

	}
	static const int GetJointIndexForNode(const FbxNode* link_node, Skeleton* skeleton)
	{
		if (link_node == nullptr)
		{
			return -1;
		}
		std::vector<Skeleton_Joint*>* joints = skeleton->GetJoints();
		for (size_t i = 0; i < joints->size(); i++)
		{
			std::string jointName = joints->at(i)->GetName();
			if (strcmp(jointName.c_str(), link_node->GetName()) == 0)
			{
				return i;
			}
		}
		return -1;

	}

	static void GetSkinWeights(SceneImport* import, std::vector<skin_weight_t>& skin_weights, const FbxMesh* mesh, 
		const std::vector<FbxNode*>& nodeAndJointIdx)
	{
		size_t control_point_count = mesh->GetControlPointsCount();
		for (size_t i = 0; i < skin_weights.size(); i++)
		{
			skin_weights[i].bone_indices = UnsignedIntVector4(0, 0, 0, 0);
			skin_weights[i].bone_weights = Vector4(0.f, 0.f, 0.f, 0.f);
		}


		int deformer_count = mesh->GetDeformerCount(FbxDeformer::eSkin);

		for (int didx = 0; didx < deformer_count; ++didx)
		{
			FbxSkin* skin = (FbxSkin*)mesh->GetDeformer(didx, FbxDeformer::eSkin);
			if (nullptr == skin)
			{
				continue;
			}

			int cluster_count = skin->GetClusterCount();
			for (int cidx = 0; cidx < cluster_count; ++cidx)
			{
				FbxCluster* cluster = skin->GetCluster(cidx);
				const FbxNode* link_node = cluster->GetLink();

				//Not associated with a bone - ignore it.
				if (nullptr == link_node)
				{
					continue;
				}

				int joint_idx = GetJointIndexForNode(link_node, import->skeletons.at(0));
				if (joint_idx == -1)
				{
					continue;
				}

				int* control_point_indices = cluster->GetControlPointIndices();
				int index_count = cluster->GetControlPointIndicesCount();
				double* weights = cluster->GetControlPointWeights();
				for (int i = 0; i < index_count; i++)
				{
					int control_idx = control_point_indices[i];
					double weight = weights[i];

					skin_weight_t* skin_weight = &skin_weights[control_idx];
					AddHighestWeight(skin_weight, joint_idx, (float)weight);
				}
			}
		}
		for (size_t i = 0; i < skin_weights.size(); i++)
		{
			//Renormalize all the skin weights
			//All weights should add up to 1, so
			//things that were never added to
			//need to have a weight added to it.
			float sum = skin_weights.at(i).bone_weights.x + 
				skin_weights.at(i).bone_weights.y +
				skin_weights.at(i).bone_weights.z +
				skin_weights.at(i).bone_weights.w;
			skin_weights.at(i).bone_weights = skin_weights.at(i).bone_weights / sum;
			if (skin_weights.at(i).bone_weights.GetLength() == 0.f)
			{
				skin_weights[i].bone_weights = Vector4(1.f, 0.f, 0.f, 0.f);
			}

		}
	}

	//!------------------
	//Import Mesh of Node
	static void ImportMesh(SceneImport* import, //What we're importing into
		FbxMesh* mesh, //Object We're Importing
		Matrix44Stack& mat_stack,
		const std::vector<FbxNode*>& nodeAndJointIdx)
	{
		if (mesh == nullptr)
		{
			return;
		}
		if (mesh->IsTriangleMesh() == false)
		{
			DebuggerPrintf("A mesh is NOT a triangle Mesh!");
			DebuggerPrintf("Continueing to run.\n");
			return;
		}
		//Start a mesh builder for this mesh
		MeshBuilder mb;
		mb.Start(PRIMITIVE_TRIANGLES, false /*don't use index buffer*/);



		int control_point_count = mesh->GetControlPointsCount();
		std::vector<skin_weight_t> skin_weights;
		skin_weights.resize(control_point_count);
		if (HasSkinWeights(mesh))
		{
			GetSkinWeights(import, skin_weights, mesh, nodeAndJointIdx);
		}
		else
		{
			// Find first parent node that has a joint associated with it
			// all vertices (fully weighted)
			// all skin_weights = indices { joint_idx, 0, 0, 0} weights { 1.0f, 0.f, 0.f, 0.f };
		}

		//Geo Transform stuff
		Matrix44 geo_transform = GetGeometricTransform(mesh->GetNode());
		mat_stack.Push(geo_transform);
		Matrix44 transform = mat_stack.GetTop();

		//figure out our weights for all vertices


		//import the mesh/vertices
		int poly_count = mesh->GetPolygonCount();
		for (int poly_idx = 0; poly_idx < poly_count; poly_idx++)
		{
			int vert_count = mesh->GetPolygonSize(poly_idx);
			ASSERT_OR_DIE(vert_count == 3, "Vertice count not 3");
			for (int vert_idx = 0; vert_idx < vert_count; ++vert_idx)
			{
				ImportVertex(&mb, transform, mesh, poly_idx, vert_idx, skin_weights);
				mb.AddIndice((3 * poly_idx) + vert_idx);//+ 2 - vert_idx);
			}
		}

		mb.End();

		//pop geo transform
		mat_stack.Pop();
		//add mesh to import
		import->meshes.push_back(mb);
	}

	//!----------------
	//Import Attribute of Node
	static void ImportNodeAttribute(SceneImport* import, //What we're importing into
		FbxNodeAttribute* attrib, //object we're importing
		Matrix44Stack & mat_stack,
		const std::vector<FbxNode*>& nodeAndJointIdx) //Tracks current transform
	{
		if (attrib == nullptr)
		{
			return;
		}
		switch (attrib->GetAttributeType())
		{
			case FbxNodeAttribute::eMesh:
				ImportMesh(import, (FbxMesh*)attrib, mat_stack, nodeAndJointIdx);
				break;
		default:
			break;
		}
	}

	//!------------------
	//GetNodeTransform
	static const Matrix44 GetNodeTransform(FbxNode* node)
	{
		FbxAMatrix fbx_local_matrix = node->EvaluateLocalTransform();
		return ToEngineMatrix(fbx_local_matrix);
	}

	//!------------------
	//Import Node of Scene
	static void ImportSceneMeshes(SceneImport* import, //What we're importing into
		FbxNode *node, //Object we're importing
		Matrix44Stack& mat_stack,
		const std::vector<FbxNode*>& nodeAndJointIdx) //Tracks Current Transform
	{
		if (node == nullptr)
		{
			return;
		}
		Matrix44 node_local_transform = GetNodeTransform(node);
		mat_stack.Push(node_local_transform);

		int attrib_count = node->GetNodeAttributeCount();
		for (int attrib_idx = 0; attrib_idx < attrib_count; ++attrib_idx)
		{
			ImportNodeAttribute(import, node->GetNodeAttributeByIndex(attrib_idx), mat_stack, nodeAndJointIdx);
		}
		int child_count = node->GetChildCount();
		for (int child_idx = 0; child_idx < child_count; ++child_idx)
		{
			FbxNode* child = node->GetChild(child_idx);
			ImportSceneMeshes(import, child, mat_stack, nodeAndJointIdx);
		}
		mat_stack.Pop();
	}

	//!-----------------
	//Triangulate Scene
	static void TriangulateScene(FbxScene* scene)
	{
		FbxGeometryConverter converter(scene->GetFbxManager());
		converter.Triangulate(scene, true);
	}

	static Skeleton* ImportSkeleton(SceneImport* import, Matrix44Stack& mat_stack, Skeleton* skeleton, int parent_joint_idx, FbxSkeleton* fbx_skeleton,
		std::map<Skeleton_Joint*, FbxNode*>& node_to_joint_idx, std::vector<FbxNode*>& nodeAndJointIdx)
	{
		Skeleton* ret_skeleton = nullptr;
		if (fbx_skeleton->IsSkeletonRoot())
		{
			ret_skeleton = new Skeleton();
			import->skeletons.push_back(ret_skeleton);
		}
		else
		{
			ret_skeleton = skeleton;
			ASSERT_OR_DIE(ret_skeleton != nullptr, "\nSkeleton being added when there is no parent/root skeleton!\n");
		}

		//Get GeoTransform and add in.
		Matrix44 geoTrans = GetGeometricTransform(fbx_skeleton->GetNode());
		mat_stack.Push(geoTrans);

		//Add Joint
		Matrix44 model_space = mat_stack.GetTop();
		ret_skeleton->AddJoint(fbx_skeleton->GetNode()->GetName(), parent_joint_idx, model_space);
		FbxNode* node = fbx_skeleton->GetNode();
		node_to_joint_idx.insert(std::pair<Skeleton_Joint*, FbxNode*>(ret_skeleton->GetLastAddedJoint(), node));
		nodeAndJointIdx.push_back(node);
		mat_stack.Pop();
		return ret_skeleton;
	}

	static void ImportSkeletons(SceneImport* import, FbxNode* node, Matrix44Stack& mat_stack,
		Skeleton* skeleton, int parent_joint_index, std::map<Skeleton_Joint*, FbxNode*>& node_to_joint_idx,
		std::vector<FbxNode*>& nodeAndJointIdx)
	{
		if (nullptr == node)
		{
			return;
		}
		Matrix44 mat = GetNodeTransform(node);
		mat_stack.Push(mat);

		//walk attributes, looking for doot doots.
		int attrib_count = node->GetNodeAttributeCount();
		for (int attrib_idx = 0; attrib_idx < attrib_count; attrib_idx++)
		{
			FbxNodeAttribute* attrib = node->GetNodeAttributeByIndex(attrib_idx);
			if ((attrib != nullptr)
				&& (attrib->GetAttributeType() == FbxNodeAttribute::eSkeleton))
			{
				FbxSkeleton* fbx_skeleton = (FbxSkeleton*)attrib;
				Skeleton* new_skeleton = ImportSkeleton(import, mat_stack, skeleton, parent_joint_index, fbx_skeleton, node_to_joint_idx, nodeAndJointIdx);

				//new_skeleton will either be the same skeleton passed, or a new skeleton, or no skeleton if it was a bad node
				//if we got something back - it is what we pass on to the next generation.
				if (new_skeleton != nullptr)
				{
					skeleton = new_skeleton;
					parent_joint_index = skeleton->GetLastAddedJointIndex();
				}
			}
		}
		int child_count = node->GetChildCount();
		for (int child_idx = 0; child_idx < child_count; ++child_idx)
		{
			ImportSkeletons(import, node->GetChild(child_idx), mat_stack, skeleton, parent_joint_index, node_to_joint_idx, nodeAndJointIdx);
		}
		mat_stack.Pop();
	}

	//!------------------
	//Import Motions

	static Matrix44 GetNodeWorldTransformAtTime(FbxNode* node, FbxTime time, const Matrix44& import_transform)
	{
		if (node == nullptr)
		{
			return Matrix44::mat44_identity;
		}

		FbxAMatrix fbx_mat = node->EvaluateGlobalTransform(time);
		Matrix44 engine_matrix = ToEngineMatrix(fbx_mat);
		return engine_matrix * import_transform;
	}

	static void ImportMotions(SceneImport* import, const Matrix44& import_transform, std::map<Skeleton_Joint*, FbxNode*> node_to_joint, 
		FbxScene* scene, float frameRate)
	{
		//Get number of animations
		int animation_count = scene->GetSrcObjectCount<FbxAnimStack>();
		if (animation_count == 0 || frameRate == 0.f)
		{
			return;
		}
		if (import->skeletons.size() == 0)
		{
			return;
		}

		//Timing information for animation in this scene
		FbxGlobalSettings &settings = scene->GetGlobalSettings();
		FbxTime::EMode time_mode = settings.GetTimeMode();
		double scene_framerate;
		if (time_mode == FbxTime::eCustom)
		{
			scene_framerate = settings.GetCustomFrameRate();
		}
		else
		{
			scene_framerate = FbxTime::GetFrameRate(time_mode);
		}

		uint32_t skeleton_count = import->skeletons.size();
		Skeleton* skeleton = import->skeletons.at(0);
		if (skeleton_count > 1 || skeleton_count < 1)
		{
			return;
		}

		//time between frames
		FbxTime advance;
		advance.SetSecondDouble((double)1.0f / frameRate);

		for (int anim_idex = 0; anim_idex < animation_count; anim_idex++)
		{
			//Import Motions

			FbxAnimStack* anim = scene->GetSrcObject<FbxAnimStack>();
			if (nullptr == anim)
			{
				continue;
			}

			//Get duration of this specific animation.
			FbxTime start_time = anim->LocalStart;
			FbxTime end_time = anim->LocalStop;
			FbxTime duration = end_time - start_time;

			scene->SetCurrentAnimationStack(anim);

			const char* motion_name = anim->GetName();
			double time_span = duration.GetSecondDouble();

			Motion* motion = new Motion(motion_name, (float)time_span, frameRate, skeleton);

			int joint_count = skeleton->GetJointCount();
			for (int joint_idx = 0; joint_idx < joint_count; ++joint_idx)
			{
				Skeleton_Joint* joint = skeleton->GetJointByIndex(joint_idx);
				FbxNode* node = node_to_joint.at(joint);
				if (node != nullptr)
				{
					Matrix44* bone_keyframes = motion->GetJointKeyFrames(joint_idx);

					FbxTime eval_time = FbxTime(0);
					const char* name = node->GetName();
					name;
					for (uint32_t frame_idx = 0; frame_idx < motion->GetFrameCount(); frame_idx++)
					{
						Matrix44 bone_transform = GetNodeWorldTransformAtTime(node, eval_time, import_transform);
						bone_keyframes[frame_idx] = bone_transform;

						eval_time += advance;
					}
				}
			}
			import->motions.push_back(motion);
		}
	}

	//!------------------
	//Import Scene
	static void ImportScene(SceneImport* import, //What we're importing into
		FbxScene *scene, //Object we're importing
		Matrix44Stack& mat_stack,
		const Matrix44& transform,
		const Matrix44& inverseTransform) //Tracks Current Transform
	{
		std::map<Skeleton_Joint*, FbxNode*> node_to_joint_idx;
		std::vector<FbxNode*> nodeAndJointIdx;
		TriangulateScene(scene);
		FbxNode* root = scene->GetRootNode();
		ImportSkeletons(import, root, mat_stack, nullptr, -1, node_to_joint_idx, nodeAndJointIdx);

		ImportSceneMeshes(import, root, mat_stack, nodeAndJointIdx);

		//Top contains just our change of basis and scale matrices at this point.
		float frameRate = 5.f;
		ImportMotions(import, mat_stack.GetTop(), node_to_joint_idx, scene, frameRate);
	}


	//!------------------
	//FbxLoadSceneFromFile
	SceneImport* FbxLoadSceneFromFile(const char* fbx_filename, const Matrix44& engine_basis, bool is_engine_basis_right_handed, const Matrix44& transform)
	{
		FbxManager *fbx_manager = FbxManager::Create();
		if (nullptr == fbx_manager) {
			DebuggerPrintf("Could not create fbx manager.");
			FBX_SAFE_DESTROY(fbx_manager);
			return nullptr;
		}
		FbxIOSettings *io_settings = FbxIOSettings::Create(fbx_manager, IOSROOT);

		fbx_manager->SetIOSettings(io_settings);

		//
		//Create an importer
		FbxImporter *importer = FbxImporter::Create(fbx_manager, "");
		bool load_successful = importer->Initialize(fbx_filename, -1, fbx_manager->GetIOSettings());

		if (load_successful)
		{
			FbxScene *scene = FbxScene::Create(fbx_manager, "");
			Matrix44 scene_basis = GetSceneBasis(scene);
			scene_basis.MakeTranspose();
			bool import_successful = importer->Import(scene);

			Matrix44Stack matrix_stack;
			if (!is_engine_basis_right_handed)
			{
				Vector3 forward = scene_basis.MatrixGetForward();
				scene_basis.MatrixSetForward(forward * -1.f);
			}

			Matrix44 identity = Matrix44::mat44_identity;

			Matrix44 inverseTransform = transform;
			inverseTransform.MatrixInvert();

			matrix_stack.Push(transform);
			matrix_stack.Push(engine_basis);
			matrix_stack.Push(scene_basis);
			Matrix44 topTest = matrix_stack.GetTop();
			Vector3 test = (topTest * Vector4(0.f, 1.f, 0.f)).XYZ();

			SceneImport *import = new SceneImport();
			ImportScene(import, scene, matrix_stack, transform, inverseTransform);

			//Do the normal kind of cleanup
			FBX_SAFE_DESTROY(fbx_manager);
			return import;
		}
		FBX_SAFE_DESTROY(fbx_manager);
		FBX_SAFE_DESTROY(importer);
		FBX_SAFE_DESTROY(io_settings);

		return nullptr;
	}
#else
	//!defined(TOOLS_BUILD)
	class FbxScene;
	void FbxListScene(char const *)
	{
	}
	SceneImport* FbxLoadSceneFromFile(const char* fbx_filename, const Matrix44& engine_basis, bool is_engine_basis_right_handed, const Matrix44& transform)
	{
		fbx_filename;
		engine_basis;
		is_engine_basis_right_handed;
		transform;
		return nullptr;
	}
#endif
