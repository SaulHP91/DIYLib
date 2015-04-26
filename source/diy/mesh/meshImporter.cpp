#include <diy/diy.hpp>

namespace diy
{
	namespace mesh
	{
		MeshFile* MeshImporter::Import(obj::ObjFile& objFile)
		{
			MeshFile* mesh_file = new MeshFile(objFile.Name);

			if (objFile.MaterialLibrary.Materials.size())
			{
				for (std::vector<obj::MtlMaterial>::iterator mtl_material = objFile.MaterialLibrary.Materials.begin(); mtl_material != objFile.MaterialLibrary.Materials.end(); ++mtl_material)
				{
					MeshMaterial* mesh_material = mesh_file->mMaterials->New(mtl_material->Name);

					mesh_material->mAmbient = glm::vec4(glm::vec3(mtl_material->Ambient), 1.0f);
					mesh_material->mDiffuse = glm::vec4(glm::vec3(mtl_material->Diffuse), mtl_material->Dissolve);
					mesh_material->mSpecular = glm::vec4(glm::vec3(mtl_material->Specular), 1.0f);
					mesh_material->mShininess = mtl_material->SpecularExponent;

					mesh_material->SetAmbientTexture(mesh_file->mTextures->Find(mtl_material->AmbientMap, true));
					mesh_material->SetDiffuseTexture(mesh_file->mTextures->Find(mtl_material->DiffuseMap, true));
					mesh_material->SetAlphaTexture(mesh_file->mTextures->Find(mtl_material->DissolveMap, true));
					mesh_material->SetSpecularTexture(mesh_file->mTextures->Find(mtl_material->SpecularMap, true));
				}
			}

			bool calculateNormals = true;

			MeshObject* mesh_object = mesh_file->mObjects->New(objFile.Name);
			for (std::vector<obj::ObjFace>::iterator obj_face = objFile.Faces.begin(); obj_face != objFile.Faces.end(); ++obj_face)
			{
				std::vector<MeshAnimatedVertex*> mesh_animated_vertices;
				for (std::vector<obj::ObjIndexedVertex>::iterator obj_indexed_vertex = obj_face->IndexedVertices.begin(); obj_indexed_vertex != obj_face->IndexedVertices.end(); ++obj_indexed_vertex)
				{
					MeshVertex meshVertex(objFile.VertexPositions[obj_indexed_vertex->PositionIndex - 1]);
					if (obj_indexed_vertex->TexCoordIndex > 0)
					{
						meshVertex.TexCoord = objFile.VertexTexCoords[obj_indexed_vertex->TexCoordIndex - 1];
					}
					if (obj_indexed_vertex->NormalIndex > 0)
					{
						meshVertex.Normal = objFile.VertexNormals[obj_indexed_vertex->NormalIndex - 1];
						calculateNormals = false;
					}
					mesh_animated_vertices.push_back(mesh_object->mAnimatedVertices->New(meshVertex));
				}
				mesh_object->mFaces->New(obj_face->MaterialIndex, mesh_animated_vertices);
			}

			mesh_object->mFaces->Triangulate();

			if (calculateNormals)
			{
				mesh_object->mAnimatedVertices->CalculateNormals();
			}

			mesh_object->UpdateArrays();

			return mesh_file;
		}

		MeshFile* MeshImporter::Import(x::XFile& xFile)
		{
			MeshFile* mesh_file = new MeshFile(xFile.Name);

			for (std::vector<x::XNode>::iterator x_node = xFile.Nodes.begin(); x_node != xFile.Nodes.end(); ++x_node)
			{
				if (x_node->Type == x::XNode::Frame && !x_node->Instance)
				{
					x::XNode xNodeFrame = *x_node;
					x::XNode xNodeFrameTransformMatrix = XGetFirstChildNodeByType(xFile, xNodeFrame, x::XNode::FrameTransformMatrix);
					std::vector<x::XFrameTransformMatrix>::iterator x_frame_transform_matrix = xFile.FrameTransformMatrices.begin() + xNodeFrameTransformMatrix.Index;

					MeshNode* mesh_node = mesh_file->mNodes->New(xNodeFrame.Name, x_frame_transform_matrix->FrameMatrix);

					x::XNode xNodeParentFrame = XGetParentNode(xFile, xNodeFrame);
					if (xNodeParentFrame.Type == x::XNode::Frame)
					{
						mesh_node->SetParent(xNodeParentFrame.Name);
					}
				}
			}

			for (std::vector<x::XNode>::iterator x_node = xFile.Nodes.begin(); x_node != xFile.Nodes.end(); ++x_node)
			{
				if (x_node->Type == x::XNode::Material && !x_node->Instance)
				{
					x::XNode xNodeMaterial = *x_node;
					std::vector<x::XMaterial>::iterator x_material = xFile.Materials.begin() + xNodeMaterial.Index;

					MeshMaterial* mesh_material = mesh_file->mMaterials->Find(xNodeMaterial.Name, true);

					mesh_material->mDiffuse = x_material->FaceColor;
					mesh_material->mSpecular = glm::vec4(glm::vec3(x_material->SpecularColor), 1.0f);
					mesh_material->mEmissive = glm::vec4(glm::vec3(x_material->EmissiveColor), 1.0f);
					mesh_material->mShininess = x_material->Power;

					x::XNode xNodeTextureFileName = XGetFirstChildNodeByType(xFile, xNodeMaterial, x::XNode::TextureFileName);
					if (xNodeTextureFileName.Type == x::XNode::TextureFileName)
					{
						std::vector<x::XTextureFileName>::iterator x_texture_file_name = xFile.TextureFileNames.begin() + xNodeTextureFileName.Index;

						mesh_material->SetDiffuseTexture(mesh_file->mTextures->Find(x_texture_file_name->FileName, true));
					}
				}
			}

			for (std::vector<x::XNode>::iterator x_node = xFile.Nodes.begin(); x_node != xFile.Nodes.end(); ++x_node)
			{
				if (x_node->Type == x::XNode::Mesh && !x_node->Instance)
				{
					x::XNode xNodeMesh = *x_node;
					std::vector<x::XMesh>::iterator x_mesh = xFile.Meshes.begin() + xNodeMesh.Index;

					MeshObject* mesh_object = mesh_file->mObjects->New(xNodeMesh.Name);

					x::XNode xNodeParentFrame = XGetParentNode(xFile, xNodeMesh);
					if (xNodeParentFrame.Type == x::XNode::Frame)
					{
						mesh_object->SetNode(xNodeParentFrame.Name);
					}

					for (std::vector<x::XVector>::iterator x_vector = x_mesh->Vertices.Values.begin(); x_vector != x_mesh->Vertices.Values.end(); ++x_vector)
					{
						mesh_object->mAnimatedVertices->New(MeshVertex(*x_vector));
					}

					for (std::vector<x::XMeshFace>::iterator x_mesh_face = x_mesh->Faces.Values.begin(); x_mesh_face != x_mesh->Faces.Values.end(); ++x_mesh_face)
					{
						std::vector<int> meshFaceVertexIndices;
						for (std::vector<x::XDWord>::iterator x_mesh_face_vertex_index = x_mesh_face->FaceVertexIndices.Values.begin(); x_mesh_face_vertex_index != x_mesh_face->FaceVertexIndices.Values.end(); ++x_mesh_face_vertex_index)
						{
							meshFaceVertexIndices.insert(meshFaceVertexIndices.begin(), *x_mesh_face_vertex_index);
						}
						mesh_object->mFaces->New(meshFaceVertexIndices);
					}

					x::XNode xNodeMeshTextureCoords = XGetFirstChildNodeByType(xFile, *x_node, x::XNode::MeshTextureCoords);
					if (xNodeMeshTextureCoords.Type == x::XNode::MeshTextureCoords)
					{
						std::vector<x::XMeshTextureCoords>::iterator x_mesh_texture_coords = xFile.MeshTextureCoords.begin() + xNodeMeshTextureCoords.Index;

						mesh_object->mAnimatedVertices->First();
						for (std::vector<x::XCoords2D>::iterator x_coords_2d = x_mesh_texture_coords->TextureCoords.Values.begin(); x_coords_2d != x_mesh_texture_coords->TextureCoords.Values.end(); ++x_coords_2d)
						{
							MeshAnimatedVertex* mesh_animated_vertex = mesh_object->mAnimatedVertices->operator()();
							MeshVertex meshVertex = mesh_animated_vertex->GetVertex();
							meshVertex.TexCoord = *x_coords_2d;
							mesh_animated_vertex->SetVertex(meshVertex);
							mesh_object->mAnimatedVertices->Next();
						}
					}

					x::XNode xNodeMeshMaterialList = XGetFirstChildNodeByType(xFile, *x_node, x::XNode::MeshMaterialList);
					if (xNodeMeshMaterialList.Type == x::XNode::MeshMaterialList)
					{
						std::vector<x::XMeshMaterialList>::iterator x_mesh_material_list = xFile.MeshMaterialList.begin() + xNodeMeshMaterialList.Index;

						std::vector<x::XNode> xNodeMaterialList;
						XGetChildrenNodeListByType(xNodeMaterialList, xFile, xNodeMeshMaterialList, x::XNode::Material);
						for (std::vector<x::XNode>::iterator x_node_material = xNodeMaterialList.begin(); x_node_material != xNodeMaterialList.end(); ++x_node_material)
						{
							mesh_object->mGroups->New(x_node_material->Name);
						}

						mesh_object->mFaces->First();
						std::vector<x::XDWord>::iterator face_index = x_mesh_material_list->FaceIndexes.Values.begin();
						while (mesh_object->mFaces->operator()())
						{
							mesh_object->mFaces->operator()()->SetGroup(*face_index);
							mesh_object->mFaces->Next();

							if (face_index != x_mesh_material_list->FaceIndexes.Values.end() - 1)
							{
								++face_index;
							}
						}
					}
					else
					{
						MeshGroup* mesh_group = mesh_object->mGroups->New(-1);

						mesh_object->mFaces->First();
						while (mesh_object->mFaces->operator()())
						{
							mesh_object->mFaces->operator()()->SetGroup(mesh_group);
							mesh_object->mFaces->Next();
						}
					}

					x::XNode xNodeMeshNormals = XGetFirstChildNodeByType(xFile, *x_node, x::XNode::MeshNormals);
					if (xNodeMeshNormals.Type == x::XNode::MeshNormals)
					{
						std::vector<x::XMeshNormals>::iterator x_mesh_normals = xFile.MeshNormals.begin() + xNodeMeshNormals.Index;
						mesh_object->mFaces->First();
						for (std::vector<x::XMeshFace>::iterator x_mesh_face = x_mesh_normals->FaceNormals.Values.begin(); x_mesh_face != x_mesh_normals->FaceNormals.Values.end(); ++x_mesh_face)
						{
							std::vector<MeshAnimatedVertex*>::iterator mesh_animated_vertex = mesh_object->GetFaces()->operator()()->mAnimatedVertices.end() - 1;
							for (std::vector<x::XDWord>::iterator x_mesh_face_vertex_index = x_mesh_face->FaceVertexIndices.Values.begin(); x_mesh_face_vertex_index != x_mesh_face->FaceVertexIndices.Values.end(); ++x_mesh_face_vertex_index, --mesh_animated_vertex)
							{
								MeshVertex meshVertex = (*mesh_animated_vertex)->GetVertex();
								meshVertex.Normal += glm::vec3(x_mesh_normals->Normals.Values[*x_mesh_face_vertex_index]);
								(*mesh_animated_vertex)->SetVertex(meshVertex);
							}
							mesh_object->mFaces->Next();
						}
						mesh_object->mFaces->Triangulate();
					}
					else
					{
						mesh_object->mFaces->Triangulate();
						mesh_object->mAnimatedVertices->CalculateNormals();
					}

					x::XNode xNodeSkinMeshHeader = XGetFirstChildNodeByType(xFile, *x_node, x::XNode::SkinMeshHeader);
					if (xNodeSkinMeshHeader.Type == x::XNode::SkinMeshHeader)
					{
						std::vector<x::XNode> xNodeSkinWeightsList;
						XGetSiblingsNodeListByType(xNodeSkinWeightsList, xFile, xNodeSkinMeshHeader, x::XNode::SkinWeights);
						for (std::vector<x::XNode>::iterator x_node_skin_weights = xNodeSkinWeightsList.begin(); x_node_skin_weights != xNodeSkinWeightsList.end(); ++x_node_skin_weights)
						{
							std::vector<x::XSkinWeights>::iterator x_skin_weights = xFile.SkinWeights.begin() + x_node_skin_weights->Index;

							MeshSkin* mesh_skin = mesh_object->mSkins->New(x_skin_weights->TransformNodeName);
							mesh_skin->OffsetMatrix = x_skin_weights->MatrixOffset;

							std::vector<x::XDWord>::iterator vertex_index;
							std::vector<x::XFloat>::iterator weight;
							for
								(
								vertex_index = x_skin_weights->VertexIndices.Values.begin(), weight = x_skin_weights->Weights.Values.begin();
							vertex_index != x_skin_weights->VertexIndices.Values.end();
							++vertex_index, ++weight
								)
							{
								mesh_skin->AppendAnimatedVertex(*vertex_index, *weight);
							}
						}
					}
				}
			}

			for (std::vector<MeshObject*>::iterator mesh_object = mesh_file->mObjects->mObjects.begin(); mesh_object != mesh_file->mObjects->mObjects.end(); ++mesh_object)
			{
				(*mesh_object)->UpdateArrays();
				(*mesh_object)->mAnimatedVertices->LimitSkinWeights(2);
			}

			return mesh_file;
		}

		MeshFile* MeshImporter::Import(smd::SmdFile& smdFile)
		{
			MeshFile* mesh_file = new MeshFile(smdFile.Name);

			std::vector<smd::SmdBone>::iterator smd_bone;
			std::vector<smd::SmdFrame>::iterator smd_frame;
			for (smd_bone = smdFile.Bones.begin(), smd_frame = smdFile.Frames.begin(); smd_bone != smdFile.Bones.end(); ++smd_bone, ++smd_frame)
			{
				mesh_file->mNodes->New(smd_bone->Name, smd_frame->GetMatrix())->SetParent(smd_bone->ParentIndex);
			}

			for (std::vector<smd::SmdFace>::iterator smd_face = smdFile.Faces.begin(); smd_face != smdFile.Faces.end(); ++smd_face)
			{
				MeshObject* mesh_object;
				if (smd_face->Vertices[0].BoneIndex < 0)
				{
					mesh_object = mesh_file->mObjects->Find(smdFile.Name, true);
				}
				else
				{
					mesh_object = mesh_file->mObjects->Find(mesh_file->mNodes->mNodes[smd_face->Vertices[0].BoneIndex]->mName, true);
				}

				MeshMaterial* mesh_material = mesh_file->mMaterials->Find(smd_face->Texture);
				if (!mesh_material)
				{
					mesh_material = mesh_file->mMaterials->New(smd_face->Texture);
					mesh_material->SetDiffuseTexture(mesh_file->mTextures->New(smd_face->Texture));
				}

				std::vector<MeshAnimatedVertex*> mesh_animated_vertices(3);
				for (int i = 0; i < 3; ++i)
				{
					smd::SmdVertex* smdVertex = &smd_face->Vertices[i];
					MeshAnimatedVertex* mesh_animated_vertex = mesh_object->mAnimatedVertices->New(MeshVertex(smdVertex->Position, smdVertex->TexCoord, smdVertex->Normal));
					mesh_animated_vertices[i] = mesh_animated_vertex;

					for (std::vector<smd::SmdSkinWeight>::iterator smd_skin_weight = smdVertex->SkinWeights.begin(); smd_skin_weight != smdVertex->SkinWeights.end(); ++smd_skin_weight)
					{
						mesh_animated_vertex->AppendSkinWeight(smd_skin_weight->Index, smd_skin_weight->Weight);
					}
					mesh_animated_vertex->LimitSkinWeights(2);
				}
				mesh_object->mFaces->New(mesh_material, mesh_animated_vertices);
			}

			for (std::vector<MeshObject*>::iterator mesh_object = mesh_file->mObjects->mObjects.begin(); mesh_object != mesh_file->mObjects->mObjects.end(); ++mesh_object)
			{
				(*mesh_object)->UpdateArrays();
			}

			return mesh_file;
		}

		MeshFile* MeshImporter::Import(pmd::PmdFile& pmdFile)
		{
			MeshFile* mesh_file = new MeshFile(pmdFile.Name);

			mesh_file->mComment = pmdFile.Comment;

			for (std::vector<pmd::PmdBone>::iterator pmd_bone = pmdFile.Bones.begin(); pmd_bone != pmdFile.Bones.end(); ++pmd_bone)
			{
				MeshNode* mesh_node = mesh_file->mNodes->New(pmd_bone->Name);
				if (pmd_bone->ParentIndex == -1)
				{
					mesh_node->SetMatrix(glm::translate(glm::vec3(pmd_bone->Translation)));
				}
				else
				{
					MeshNode* mesh_node_parent = mesh_file->mNodes->mNodes[pmd_bone->ParentIndex];
					mesh_node->SetParent(mesh_node_parent);
					mesh_node->SetMatrix(glm::translate(glm::vec3(pmd_bone->Translation)) * mesh_node_parent->mOffsetMatrix);
				}
			}

			MeshObject* mesh_object = mesh_file->mObjects->New(pmdFile.Name);

			for (std::vector<pmd::PmdVertex>::iterator pmd_vertex = pmdFile.Vertices.begin(); pmd_vertex != pmdFile.Vertices.end(); ++pmd_vertex)
			{
				MeshAnimatedVertex* mesh_animated_vertex = mesh_object->mAnimatedVertices->New(MeshVertex(pmd_vertex->Position, pmd_vertex->TexCoord, pmd_vertex->Normal));

				float bone0Weight = float(pmd_vertex->Bone0Weight) / 100.0f;
				mesh_animated_vertex->AppendSkinWeight(pmd_vertex->BoneIndices[0], bone0Weight);

				float bone1Weight = 1.0f - bone0Weight;
				if (bone1Weight > 0.0f)
				{
					mesh_animated_vertex->AppendSkinWeight(pmd_vertex->BoneIndices[1], bone1Weight);
				}
			}

			if (pmdFile.MorphTargets.size() > 1)
			{
				for (std::vector<pmd::PmdMorphTarget>::iterator pmd_morph_target = pmdFile.MorphTargets.begin() + 1; pmd_morph_target != pmdFile.MorphTargets.end(); ++pmd_morph_target)
				{
					MeshMorphTarget* mesh_morph_target = mesh_object->mMorphTargets->New(pmd_morph_target->Name);
					for (std::vector<pmd::PmdMorphTargetVertex>::iterator pmd_morph_target_vertex = pmd_morph_target->Vertices.begin(); pmd_morph_target_vertex != pmd_morph_target->Vertices.end(); ++pmd_morph_target_vertex)
					{
						MeshAnimatedVertex* mesh_animated_vertex = mesh_object->mAnimatedVertices->mAnimatedVertices[pmdFile.MorphTargets.front().Vertices[pmd_morph_target_vertex->Index].Index];
						mesh_morph_target->AppendAnimatedVertex(mesh_animated_vertex, mesh_animated_vertex->mVertex + MeshVertex(glm::vec3(pmd_morph_target_vertex->Position)));
					}
				}
			}

			std::vector<pmd::PmdFace>::iterator pmd_face = pmdFile.Faces.begin();
			for (std::vector<pmd::PmdMaterial>::iterator pmd_material = pmdFile.Materials.begin(); pmd_material != pmdFile.Materials.end(); ++pmd_material)
			{
				MeshMaterial* mesh_material = mesh_file->mMaterials->New();
				mesh_material->mDiffuse = glm::vec4(pmd_material->Diffuse, pmd_material->Alpha);
				mesh_material->mShininess = pmd_material->Shininess;
				mesh_material->mSpecular = glm::vec4(pmd_material->Specular, 1.0f);
				mesh_material->mAmbient = glm::vec4(pmd_material->Ambient, 1.0f);
				mesh_material->Contour = pmd_material->EdgeFlag;
				mesh_material->SetDiffuseTexture(mesh_file->mTextures->Find(pmd_material->Texture, true));

				std::vector<int> vertexIndices(3);
				for (int n = 0; n < pmd_material->IndexCount / 3; ++n, ++pmd_face)
				{
					vertexIndices[0] = static_cast<int>(pmd_face->VertexIndices[2]);
					vertexIndices[1] = static_cast<int>(pmd_face->VertexIndices[1]);
					vertexIndices[2] = static_cast<int>(pmd_face->VertexIndices[0]);
					mesh_object->mFaces->New(mesh_material, vertexIndices);
				}
			}

			mesh_object->UpdateArrays();

			return mesh_file;
		}

		MeshFile* MeshImporter::Import(mqo::MqoFile& mqoFile)
		{
			MeshFile* mesh_file = new MeshFile(mqoFile.Name);

			for (std::vector<mqo::MqoMaterial>::iterator mqo_material = mqoFile.Materials.begin(); mqo_material != mqoFile.Materials.end(); ++mqo_material)
			{
				MeshMaterial* mesh_material = mesh_file->mMaterials->New(mqo_material->Name);

				mesh_material->mDiffuse = glm::vec4(mqo_material->Diffuse * glm::vec3(mqo_material->Color), mqo_material->Alpha);
				mesh_material->mAmbient = glm::vec4(glm::vec3(mqo_material->Ambient), 1.0f);
				mesh_material->mSpecular = glm::vec4(glm::vec3(mqo_material->Specular), 1.0f);
				mesh_material->mEmissive = glm::vec4(glm::vec3(mqo_material->Emission), 1.0f);
				mesh_material->mShininess = mqo_material->Power;

				mesh_material->SetDiffuseTexture(mesh_file->mTextures->Find(mqo_material->Texture, true));
				mesh_material->SetAlphaTexture(mesh_file->mTextures->Find(mqo_material->AlphaPlane, true));
				mesh_material->SetBumpTexture(mesh_file->mTextures->Find(mqo_material->Bump, true));
			}

			for (std::vector<mqo::MqoObject>::iterator mqo_object = mqoFile.Objects.begin(); mqo_object != mqoFile.Objects.end(); ++mqo_object)
			{
				MeshNode* mesh_node = mesh_file->mNodes->New(mqo_object->Name);
				mesh_node->SetMatrix(mqo_object->GetMatrix());

				if (mqo_object->Depth > 0)
				{
					std::vector<mqo::MqoObject>::iterator mqo_object_parent = mqo_object;
					do
					{
						--mqo_object_parent;
					} while (mqo_object_parent->Depth >= mqo_object->Depth);
					mesh_node->SetParent(mesh_file->mNodes->Find(mqo_object_parent->Name));
				}

				MeshObject* mesh_object = mesh_file->mObjects->New(mqo_object->Name);
				mesh_object->SetNode(mesh_node);
				mesh_object->mOffsetMatrix = mesh_node->mOffsetMatrix;

				for (std::vector<mqo::MqoVector>::iterator mqo_vertex = mqo_object->Vertices.begin(); mqo_vertex != mqo_object->Vertices.end(); ++mqo_vertex)
				{
					mesh_object->mAnimatedVertices->New(MeshVertex(*mqo_vertex));
				}

				for (std::vector<mqo::MqoFace>::iterator mqo_face = mqo_object->Faces.begin(); mqo_face != mqo_object->Faces.end(); ++mqo_face)
				{
					if (mqo_face->VertexCount == 3 || mqo_face->VertexCount == 4)
					{
						std::vector<MeshAnimatedVertex*> animatedVertices;
						for (int i = 0; i < mqo_face->VertexCount; ++i)
						{
							MeshAnimatedVertex* mesh_animated_vertex = mesh_object->mAnimatedVertices->mAnimatedVertices[mqo_face->VertexIndices[i]];
							if (mesh_animated_vertex->mReserved)
							{
								mesh_animated_vertex = mesh_animated_vertex->Duplicate();
							}
							mesh_animated_vertex->mReserved = mesh_animated_vertex;
							MeshVertex meshVertex = mesh_animated_vertex->GetVertex();
							meshVertex.TexCoord = mqo_face->VertexTexCoords[i];
							mesh_animated_vertex->SetVertex(meshVertex);
							animatedVertices.insert(animatedVertices.begin(), mesh_animated_vertex);
						}
						mesh_object->mFaces->New(mqo_face->MaterialIndex, animatedVertices);
					}
				}

				if (mqo_object->Mirror)
				{
					mesh_object->Mirror(mqo_object->MirrorAxis);
				}
				mesh_object->mFaces->Triangulate();
				mesh_object->mAnimatedVertices->CalculateNormals();
				mesh_object->UpdateArrays();
			}

			return mesh_file;
		}

		MeshFile* MeshImporter::Import(tds::TdsFile& tdsFile)
		{
			MeshFile* mesh_file = new MeshFile(tdsFile.Name);

			for (std::vector<tds::TdsMaterial>::iterator tds_material = tdsFile.Materials.begin(); tds_material != tdsFile.Materials.end(); ++tds_material)
			{
				MeshMaterial* mesh_material = mesh_file->mMaterials->Find(tds_material->Name, true);

				mesh_material->mAmbient = glm::vec4(glm::vec3(tds_material->Ambient), 1.0f);
				mesh_material->mDiffuse = glm::vec4(glm::vec3(tds_material->Diffuse), 1.0f - tds_material->Transparency / 100.0f);
				mesh_material->mSpecular = glm::vec4(glm::vec3(tds_material->Specular), 1.0f);
				mesh_material->mShininess = 100.0f - tds_material->Shininess;

				mesh_material->SetDiffuseTexture(mesh_file->mTextures->Find(tds_material->TexMap.Name, true));
				mesh_material->SetAlphaTexture(mesh_file->mTextures->Find(tds_material->OpacMap.Name, true));
			}

			for (std::vector<tds::TdsObjectNode>::iterator tds_object_node = tdsFile.ObjectNodes.begin(); tds_object_node != tdsFile.ObjectNodes.end(); ++tds_object_node)
			{
				if (!_stricmp(tds_object_node->Header.Name.c_str(), TDS_DUMMY))
				{
					continue;
				}

				MeshNode* mesh_node = mesh_file->mNodes->Find(tds_object_node->Header.Name, true);
				if (tds_object_node->Header.Hierarchy != -1)
				{
					mesh_node->SetParent(tdsFile.ObjectNodes[tds_object_node->Header.Hierarchy].Header.Name);
				}
				mesh_node->mPivot = tds_object_node->Pivot;
			}

			for (std::vector<tds::TdsObject>::iterator tds_object = tdsFile.Objects.begin(); tds_object != tdsFile.Objects.end(); ++tds_object)
			{
				MeshNode* mesh_node = mesh_file->mNodes->Find(tds_object->Name, true);
				if (mesh_node->mParent)
				{
					mesh_node->SetMatrix(mesh_node->mParent->mOffsetMatrix * glm::mat4(tds_object->Matrix));
				}
				else
				{
					mesh_node->SetMatrix(tds_object->Matrix);
				}

				MeshObject* mesh_object = mesh_file->mObjects->New(tds_object->Name);
				mesh_object->SetNode(mesh_node);
				mesh_object->mOffsetMatrix = glm::translate(-mesh_node->mPivot) * mesh_node->mOffsetMatrix;

				std::vector<tds::TdsPoint>::iterator tds_point;
				std::vector<tds::TdsTexVert>::iterator tds_tex_vert;
				for (tds_point = tds_object->PointArray.begin(), tds_tex_vert = tds_object->TexVerts.begin(); tds_point != tds_object->PointArray.end(); ++tds_point)
				{
					if (tds_tex_vert == tds_object->TexVerts.end())
					{
						mesh_object->mAnimatedVertices->New(MeshVertex(*tds_point));
					}
					else
					{
						mesh_object->mAnimatedVertices->New(MeshVertex(*tds_point, *tds_tex_vert));
						++tds_tex_vert;
					}
				}

				std::vector<int> vertexIndices(3);
				for (std::vector<tds::TdsFace>::iterator tds_face = tds_object->FaceArray.begin(); tds_face != tds_object->FaceArray.end(); ++tds_face)
				{
					vertexIndices[0] = static_cast<int>(tds_face->Vertex1);
					vertexIndices[1] = static_cast<int>(tds_face->Vertex2);
					vertexIndices[2] = static_cast<int>(tds_face->Vertex3);
					mesh_object->mFaces->New(vertexIndices);
				}

				for (std::vector<tds::TdsMshMatGroup>::iterator tds_msh_mat_group = tds_object->MshMatGroups.begin(); tds_msh_mat_group != tds_object->MshMatGroups.end(); ++tds_msh_mat_group)
				{
					MeshGroup* mesh_group = mesh_object->mGroups->Find(tds_msh_mat_group->MaterialName, true);

					for (std::vector<unsigned short>::iterator face = tds_msh_mat_group->FaceNum.begin(); face != tds_msh_mat_group->FaceNum.end(); ++face)
					{
						mesh_group->AppendFace(static_cast<int>(*face));
					}
				}

				mesh_object->mAnimatedVertices->CalculateNormals();
				mesh_object->UpdateArrays();
			}

			return mesh_file;
		}

		MeshFile* MeshImporter::Import(std::string name)
		{
			char ext[256];
			_splitpath(name.c_str(), 0, 0, 0, ext);

			if (!_stricmp(ext, MQO_EXT))
			{
				mqo::MqoFile mqoFile;
				mqoFile.ParseFromFile(name);
				return Import(mqoFile);
			}

			if (!_stricmp(ext, OBJ_EXT))
			{
				obj::ObjFile objFile;
				objFile.ParseFromFile(name);
				return Import(objFile);
			}

			if (!_stricmp(ext, PMD_EXT))
			{
				pmd::PmdFile pmdFile;
				pmdFile.LoadFromFile(name);
				return Import(pmdFile);
			}


			if (!_stricmp(ext, SMD_EXT))
			{
				smd::SmdFile smdFile;
				smdFile.ParseFromFile(name);
				return Import(smdFile);
			}


			if (!_stricmp(ext, TDS_EXT))
			{
				tds::TdsFile tdsFile;
				tdsFile.LoadFromFile(name);
				return Import(tdsFile);
			}

			if (!_stricmp(ext, X_EXT))
			{
				x::XFile xFile;
				xFile.ParseFromFile(name);
				return Import(xFile);
			}

			return 0;
		}
	}
}
