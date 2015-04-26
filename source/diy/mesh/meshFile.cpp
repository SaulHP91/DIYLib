#include <diy/diy.hpp>

#include <sstream>

#include <zlib.h>

namespace diy
{
	namespace mesh
	{
		MeshFile::MeshFile(std::string name) :
			mName(name),
			mComment()
		{
			mNodes = new MeshNodes(this);
			mObjects = new MeshObjects(this);
			mMaterials = new MeshMaterials(this);
			mTextures = new MeshTextures(this);
			mPhysics = new MeshPhysics(this);
		}

		MeshFile::~MeshFile(void)
		{
			delete mNodes;
			delete mObjects;
			delete mMaterials;
			delete mTextures;
			delete mPhysics;
		}

		void MeshFile::SetName(std::string name)
		{
			mName = name;
		}

		std::string MeshFile::GetName(void)
		{
			return mName;
		}

		void MeshFile::SetComment(std::string comment)
		{
			mComment = comment;
		}

		std::string MeshFile::GetComment(void)
		{
			return mComment;
		}

		MeshNodes* MeshFile::GetNodes(void)
		{
			return mNodes;
		}

		MeshObjects* MeshFile::GetObjects(void)
		{
			return mObjects;
		}

		MeshMaterials* MeshFile::GetMaterials(void)
		{
			return mMaterials;
		}

		MeshTextures* MeshFile::GetTextures(void)
		{
			return mTextures;
		}

		MeshPhysics* MeshFile::GetPhysics(void)
		{
			return mPhysics;
		}

		void MeshFile::Clear(void)
		{
			mName.clear();
			mComment.clear();

			mNodes->Clear();
			mObjects->Clear();
			mMaterials->Clear();
			mTextures->Clear();
			mPhysics->Clear();
		}

		bool MeshFile::LoadFromFile(std::string path)
		{
			std::ifstream in(path.c_str(), std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				char header[5];
				in.read(header, 4);
				header[4] = 0;
				if (_stricmp(header, MESH_HEADER))
				{
					return false;
				}

				float version;
				in.read(reinterpret_cast<char*>(&version), 4);
				if (version != MESH_VERSION)
				{
					return false;
				}

				int length;
				in.read(reinterpret_cast<char*>(&length), 4);

				char* name = new char[length];
				in.read(name, length);
				mName = name;
				delete[] name;

				int commentLength;
				in.read(reinterpret_cast<char*>(&commentLength), 4);

				char* comment = new char[commentLength];
				in.read(comment, commentLength);
				mComment = comment;
				delete[] comment;

				mNodes->LoadFromFStream(in);
				mObjects->LoadFromFStream(in);
				mMaterials->LoadFromFStream(in);
				mTextures->LoadFromFStream(in);
				mPhysics->LoadFromFStream(in);

				int endMesh;
				in.read(reinterpret_cast<char*>(&endMesh), 4);

				in.close();

				return true;
			}

			return false;
		}

		void MeshFile::SaveToFile(std::string path)
		{
			std::ofstream out(path.c_str(), std::ios::out | std::ios::binary);
			if (out.is_open())
			{
				char header[] = MESH_HEADER;
				out.write(header, 4);

				float version = MESH_VERSION;
				out.write(reinterpret_cast<char*>(&version), 4);

				int length = strlen(mName.c_str()) + 1;
				out.write(reinterpret_cast<char*>(&length), 4);
				out.write(mName.c_str(), length);

				int commentLength = strlen(mComment.c_str()) + 1;
				out.write(reinterpret_cast<char*>(&commentLength), 4);
				out.write(mComment.c_str(), commentLength);

				mNodes->SaveToFStream(out);
				mObjects->SaveToFStream(out);
				mMaterials->SaveToFStream(out);
				mTextures->SaveToFStream(out);
				mPhysics->SaveToFStream(out);

				int endMesh = 0;
				out.write(reinterpret_cast<char*>(&endMesh), 4);

				out.close();
			}
		}

		void MeshFile::ExportAndroidData(std::string path)
		{
			std::ofstream out(path.c_str(), std::ios::out | std::ios::binary);
			if (out.is_open())
			{
				int length = mName.length();
				out.write(reinterpret_cast<char*>(&length), 4);
				out.write(mName.c_str(), length);

				length = mComment.length();
				out.write(reinterpret_cast<char*>(&length), 4);
				out.write(mComment.c_str(), length);

				int nNodes = mNodes->mNodes.size();
				out.write(reinterpret_cast<char*>(&nNodes), 4);
				for (std::vector<MeshNode*>::iterator node = mNodes->mNodes.begin(); node != mNodes->mNodes.end(); ++node)
				{
					length = (*node)->mName.length();
					out.write(reinterpret_cast<char*>(&length), 4);
					out.write((*node)->mName.c_str(), length);
					out.write(reinterpret_cast<char*>(glm::value_ptr((*node)->mMatrix)), 64);
					int parent = -1;
					if ((*node)->mParent)
					{
						parent = (*node)->mParent->mIndex;
					}
					out.write(reinterpret_cast<char*>(&parent), 4);
				}

				int nObjects = mObjects->mObjects.size();
				out.write(reinterpret_cast<char*>(&nObjects), 4);
				for (std::vector<MeshObject*>::iterator object = mObjects->mObjects.begin(); object != mObjects->mObjects.end(); ++object)
				{
					length = (*object)->mName.length();
					out.write(reinterpret_cast<char*>(&length), 4);
					out.write((*object)->mName.c_str(), length);
					int node = -1;
					if ((*object)->mNode)
					{
						node = (*object)->mNode->mIndex;
					}
					out.write(reinterpret_cast<char*>(&node), 4);
					out.write(reinterpret_cast<char*>(&(*object)->mAnimatedVertices->mSize), 4);
					out.write(reinterpret_cast<char*>((*object)->mAnimatedVertices->mFinalPositionArray), 12 * (*object)->mAnimatedVertices->mSize);
					HALF texCoord[2];
					for (unsigned int i = 0; i < (*object)->mAnimatedVertices->mSize; ++i)
					{
						texCoord[0] = floatToHALF((*object)->mAnimatedVertices->mTexCoordArray[2 * i]);
						texCoord[1] = floatToHALF((*object)->mAnimatedVertices->mTexCoordArray[2 * i + 1]);
						out.write(reinterpret_cast<char*>(texCoord), 4);
					}
					HALF tangent[3];
					for (unsigned int i = 0; i < (*object)->mAnimatedVertices->mSize; ++i)
					{
						tangent[0] = floatToHALF((*object)->mAnimatedVertices->mFinalTangentArray[3 * i]);
						tangent[1] = floatToHALF((*object)->mAnimatedVertices->mFinalTangentArray[3 * i + 1]);
						tangent[2] = floatToHALF((*object)->mAnimatedVertices->mFinalTangentArray[3 * i + 2]);
						out.write(reinterpret_cast<char*>(tangent), 6);
					}
					HALF bitangent[3];
					for (unsigned int i = 0; i < (*object)->mAnimatedVertices->mSize; ++i)
					{
						bitangent[0] = floatToHALF((*object)->mAnimatedVertices->mFinalBitangentArray[3 * i]);
						bitangent[1] = floatToHALF((*object)->mAnimatedVertices->mFinalBitangentArray[3 * i + 1]);
						bitangent[2] = floatToHALF((*object)->mAnimatedVertices->mFinalBitangentArray[3 * i + 2]);
						out.write(reinterpret_cast<char*>(bitangent), 6);
					}
					HALF normal[3];
					for (unsigned int i = 0; i < (*object)->mAnimatedVertices->mSize; ++i)
					{
						normal[0] = floatToHALF((*object)->mAnimatedVertices->mFinalNormalArray[3 * i]);
						normal[1] = floatToHALF((*object)->mAnimatedVertices->mFinalNormalArray[3 * i + 1]);
						normal[2] = floatToHALF((*object)->mAnimatedVertices->mFinalNormalArray[3 * i + 2]);
						out.write(reinterpret_cast<char*>(normal), 6);
					}
					out.write(reinterpret_cast<char*>(&(*object)->mFaces->mSize), 4);
					if ((*object)->mAnimatedVertices->mSize < 256)
					{
						unsigned char face[3];
						for (int i = 0; i < (*object)->mFaces->mSize; ++i)
						{
							face[0] = static_cast<unsigned char>((*object)->mFaces->mIndexArray[3 * i]);
							face[1] = static_cast<unsigned char>((*object)->mFaces->mIndexArray[3 * i + 1]);
							face[2] = static_cast<unsigned char>((*object)->mFaces->mIndexArray[3 * i + 2]);
							out.write(reinterpret_cast<char*>(face), 3);
						}
					}
					else if ((*object)->mAnimatedVertices->mSize < 65536)
					{
						unsigned short face[3];
						for (int i = 0; i < (*object)->mFaces->mSize; ++i)
						{
							face[0] = static_cast<unsigned short>((*object)->mFaces->mIndexArray[3 * i]);
							face[1] = static_cast<unsigned short>((*object)->mFaces->mIndexArray[3 * i + 1]);
							face[2] = static_cast<unsigned short>((*object)->mFaces->mIndexArray[3 * i + 2]);
							out.write(reinterpret_cast<char*>(face), 6);
						}
					}
					else
					{
						out.write(reinterpret_cast<char*>((*object)->mFaces->mIndexArray), 12 * (*object)->mFaces->mSize);
					}
					int nGroups = (*object)->mGroups->mGroups.size();
					out.write(reinterpret_cast<char*>(&nGroups), 4);
					int firstFace = 0;
					for (std::vector<MeshGroup*>::iterator group = (*object)->mGroups->mGroups.begin(); group != (*object)->mGroups->mGroups.end(); ++group)
					{
						int materialIndex = -1;
						if ((*group)->mMaterial)
						{
							materialIndex = (*group)->mMaterial->mIndex;
						}
						out.write(reinterpret_cast<char*>(&materialIndex), 4);
						out.write(reinterpret_cast<char*>(&firstFace), 4);
						int nFaces = (*group)->mFaces.size();
						out.write(reinterpret_cast<char*>(&nFaces), 4);
						firstFace += nFaces;
					}

					if ((*object)->mSkins->mSkins.empty())
					{
						int nSkins = 0;
						out.write(reinterpret_cast<const char*>(&nSkins), 4);
					}
					else
					{
						int nSkins = (*object)->mAnimatedVertices->mAnimatedVertices.size();
						out.write(reinterpret_cast<char*>(&nSkins), 4);
						for (std::vector<MeshAnimatedVertex*>::iterator animated_vertex = (*object)->mAnimatedVertices->mAnimatedVertices.begin(); animated_vertex != (*object)->mAnimatedVertices->mAnimatedVertices.end(); ++animated_vertex)
						{
							switch ((*animated_vertex)->mSkinWeights.size())
							{
							case 0:
							{
								HALF nodeIndex0 = floatToHALF(-1.0f);
								out.write(reinterpret_cast<char*>(&nodeIndex0), 2);
								HALF skinWeight0 = floatToHALF(0.0f);
								out.write(reinterpret_cast<char*>(&skinWeight0), 2);
								HALF nodeIndex1 = floatToHALF(-1.0f);
								out.write(reinterpret_cast<char*>(&nodeIndex1), 2);
								HALF skinWeight1 = floatToHALF(0.0f);
								out.write(reinterpret_cast<char*>(&skinWeight1), 2);
								break;
							}
							case 1:
							{
								HALF nodeIndex0 = floatToHALF(static_cast<float>((*animated_vertex)->mSkinWeights.front()->mSkinWeights->mSkin->mNode->mIndex));
								out.write(reinterpret_cast<char*>(&nodeIndex0), 2);
								HALF skinWeight0 = floatToHALF((*animated_vertex)->mSkinWeights.front()->mWeight);
								out.write(reinterpret_cast<char*>(&skinWeight0), 2);
								HALF nodeIndex1 = floatToHALF(-1.0f);
								out.write(reinterpret_cast<char*>(&nodeIndex1), 2);
								HALF skinWeight1 = floatToHALF(0.0f);
								out.write(reinterpret_cast<char*>(&skinWeight1), 2);
								break;
							}
							case 2: default:
							{
								HALF nodeIndex0 = floatToHALF(static_cast<float>((*animated_vertex)->mSkinWeights.front()->mSkinWeights->mSkin->mNode->mIndex));
								out.write(reinterpret_cast<char*>(&nodeIndex0), 2);
								HALF skinWeight0 = floatToHALF((*animated_vertex)->mSkinWeights.front()->mWeight);
								out.write(reinterpret_cast<char*>(&skinWeight0), 2);
								HALF nodeIndex1 = floatToHALF(static_cast<float>((*animated_vertex)->mSkinWeights.back()->mSkinWeights->mSkin->mNode->mIndex));
								out.write(reinterpret_cast<char*>(&nodeIndex1), 2);
								HALF skinWeight1 = floatToHALF((*animated_vertex)->mSkinWeights.back()->mWeight);
								out.write(reinterpret_cast<char*>(&skinWeight1), 2);
							}
							}
						}
					}
					if ((*object)->mMorphTargets->mMorphTargets.empty())
					{
						int nMorphTargets = 0;
						out.write(reinterpret_cast<char*>(&nMorphTargets), 4);
					}
					else
					{
						int nMorphTargets = (*object)->mMorphTargets->mMorphTargets.size() + 1;
						out.write(reinterpret_cast<char*>(&nMorphTargets), 4);
						length = 0;
						out.write(reinterpret_cast<char*>(&length), 4);
						int nBaseVertices = 0;
						for (std::vector<MeshAnimatedVertex*>::iterator vertex = (*object)->mAnimatedVertices->mAnimatedVertices.begin(); vertex != (*object)->mAnimatedVertices->mAnimatedVertices.end(); ++vertex)
						{
							if ((*vertex)->mMorphTargetVertices.size())
							{
								++nBaseVertices;
							}
						}
						out.write(reinterpret_cast<char*>(&nBaseVertices), 4);
						for (std::vector<MeshAnimatedVertex*>::iterator vertex = (*object)->mAnimatedVertices->mAnimatedVertices.begin(); vertex != (*object)->mAnimatedVertices->mAnimatedVertices.end(); ++vertex)
						{
							if ((*vertex)->mMorphTargetVertices.size())
							{
								out.write(reinterpret_cast<char*>(&(*vertex)->mIndex), 4);
							}
						}
						for (std::vector<MeshAnimatedVertex*>::iterator vertex = (*object)->mAnimatedVertices->mAnimatedVertices.begin(); vertex != (*object)->mAnimatedVertices->mAnimatedVertices.end(); ++vertex)
						{
							if ((*vertex)->mMorphTargetVertices.size())
							{
								out.write(reinterpret_cast<char*>(glm::value_ptr((*vertex)->mVertex.Position)), 12);
							}
						}
						for (std::vector<MeshMorphTarget*>::iterator morph_target = (*object)->mMorphTargets->mMorphTargets.begin(); morph_target != (*object)->mMorphTargets->mMorphTargets.end(); ++morph_target)
						{
							length = (*morph_target)->mName.length();
							out.write(reinterpret_cast<char*>(&length), 4);
							out.write((*morph_target)->mName.c_str(), length);
							int nVertices = (*morph_target)->mMorphTargetVertices->mMorphTargetVertices.size();
							out.write(reinterpret_cast<char*>(&nVertices), 4);
							for (std::vector<MeshMorphTargetVertex*>::iterator vertex = (*morph_target)->mMorphTargetVertices->mMorphTargetVertices.begin(); vertex != (*morph_target)->mMorphTargetVertices->mMorphTargetVertices.end(); ++vertex)
							{
								out.write(reinterpret_cast<char*>(&(*vertex)->mAnimatedVertex->mIndex), 4);
							}
							for (std::vector<MeshMorphTargetVertex*>::iterator vertex = (*morph_target)->mMorphTargetVertices->mMorphTargetVertices.begin(); vertex != (*morph_target)->mMorphTargetVertices->mMorphTargetVertices.end(); ++vertex)
							{
								out.write(reinterpret_cast<char*>(glm::value_ptr((*vertex)->mDeltaVertex.Position)), 12);
							}
						}
					}
				}

				int nMaterials = mMaterials->mMaterials.size();
				out.write(reinterpret_cast<char*>(&nMaterials), 4);
				for (std::vector<MeshMaterial*>::iterator material = mMaterials->mMaterials.begin(); material != mMaterials->mMaterials.end(); ++material)
				{
					length = (*material)->mName.length();
					out.write(reinterpret_cast<char*>(&length), 4);
					out.write((*material)->mName.c_str(), length);
					out.write(reinterpret_cast<char*>(&(*material)->mShader), 4);
					out.write(reinterpret_cast<char*>(&(*material)->mType), 4);
					out.write(reinterpret_cast<char*>(&(*material)->mMode), 4);
					out.write(reinterpret_cast<char*>(glm::value_ptr((*material)->mAmbient)), 16);
					out.write(reinterpret_cast<char*>(glm::value_ptr((*material)->mDiffuse)), 16);
					out.write(reinterpret_cast<char*>(glm::value_ptr((*material)->mSpecular)), 16);
					out.write(reinterpret_cast<char*>(glm::value_ptr((*material)->mEmissive)), 16);
					out.write(reinterpret_cast<char*>(&(*material)->mShininess), 4);
					int ambientTextureIndex = -1;
					if ((*material)->mAmbientTexture)
					{
						ambientTextureIndex = (*material)->mAmbientTexture->mIndex;
					}
					out.write(reinterpret_cast<char*>(&ambientTextureIndex), 4);
					int diffuseTextureIndex = -1;
					if ((*material)->mDiffuseTexture)
					{
						diffuseTextureIndex = (*material)->mDiffuseTexture->mIndex;
					}
					out.write(reinterpret_cast<char*>(&diffuseTextureIndex), 4);
					int alphaTextureIndex = -1;
					if ((*material)->mAlphaTexture)
					{
						alphaTextureIndex = (*material)->mAlphaTexture->mIndex;
					}
					out.write(reinterpret_cast<char*>(&alphaTextureIndex), 4);
					int specularTextureIndex = -1;
					if ((*material)->mSpecularTexture)
					{
						specularTextureIndex = (*material)->mSpecularTexture->mIndex;
					}
					out.write(reinterpret_cast<char*>(&specularTextureIndex), 4);
					int emissiveTextureIndex = -1;
					if ((*material)->mEmissiveTexture)
					{
						emissiveTextureIndex = (*material)->mEmissiveTexture->mIndex;
					}
					out.write(reinterpret_cast<char*>(&emissiveTextureIndex), 4);
					int bumpTextureIndex = -1;
					if ((*material)->mBumpTexture)
					{
						bumpTextureIndex = (*material)->mBumpTexture->mIndex;
					}
					out.write(reinterpret_cast<char*>(&bumpTextureIndex), 4);
					int depthTextureIndex = -1;
					if ((*material)->mDepthTexture)
					{
						depthTextureIndex = (*material)->mDepthTexture->mIndex;
					}
					out.write(reinterpret_cast<char*>(&depthTextureIndex), 4);
					int contour = (*material)->Contour;
					out.write(reinterpret_cast<char*>(&contour), 4);
					out.write(reinterpret_cast<char*>(glm::value_ptr((*material)->ContourColor)), 16);
					out.write(reinterpret_cast<char*>(&(*material)->ContourThickness), 4);
				}

				int nTextures = mTextures->mTextures.size();
				out.write(reinterpret_cast<char*>(&nTextures), 4);
				for (std::vector<MeshTexture*>::iterator texture = mTextures->mTextures.begin(); texture != mTextures->mTextures.end(); ++texture)
				{
					(*texture)->ExportAndroidDataToFStream(out);
				}

				out.close();

				std::ifstream in(path.c_str(), std::ios::in | std::ios::binary);
				std::streampos start = in.tellg();
				in.seekg(0, std::ifstream::end);
				unsigned long sourceLen = in.tellg();
				char* source = new char[sourceLen];
				in.clear();
				in.seekg(start);
				in.read(source, sourceLen);
				in.close();
				unsigned long destLen = sourceLen;
				char* dest = new char[destLen];
				compress(reinterpret_cast<Bytef*>(dest), &destLen, reinterpret_cast<Bytef*>(source), sourceLen);
				delete[] source;

				out.open(path.c_str(), std::ios::out | std::ios::binary);
				char header[] = MESH_HEADER;
				out.write(header, 4);
				HALF version = floatToHALF(1.0f);
				out.write(reinterpret_cast<char*>(&version), 2);
				out.write(reinterpret_cast<char*>(&destLen), 4);
				out.write(reinterpret_cast<char*>(&sourceLen), 4);
				out.write(dest, destLen);
				delete[] dest;
				out.close();
			}
		}

		void MeshFile::Combine(MeshFile* file)
		{
			file->mNodes->First();
			while (MeshNode* node = file->mNodes->operator()())
			{
				MeshNode* new_node = mNodes->New(node->mName, node->mMatrix);
				if (new_node && node->mParent)
				{
					new_node->SetParent(node->mParent->mName);
				}

				file->mNodes->Next();
			}

			file->mMaterials->First();
			while (MeshMaterial* material = file->mMaterials->operator ()())
			{
				MeshMaterial* new_material = mMaterials->New(material->mName);
				if (new_material)
				{
					new_material->mShader = material->mShader;
					new_material->mType = material->mType;
					new_material->mMode = material->mMode;

					new_material->mAmbient = material->mAmbient;
					new_material->mDiffuse = material->mDiffuse;
					new_material->mSpecular = material->mSpecular;
					new_material->mEmissive = material->mEmissive;
					new_material->mShininess = material->mShininess;

					if (material->mAmbientTexture)
					{
						new_material->SetAmbientTexture(mTextures->Find(material->mAmbientTexture->mName, true));
					}

					if (material->mDiffuseTexture)
					{
						new_material->SetDiffuseTexture(mTextures->Find(material->mDiffuseTexture->mName, true));
					}

					if (material->mAlphaTexture)
					{
						new_material->SetAlphaTexture(mTextures->Find(material->mAlphaTexture->mName, true));
					}

					if (material->mSpecularTexture)
					{
						new_material->SetSpecularTexture(mTextures->Find(material->mSpecularTexture->mName, true));
					}

					if (material->mEmissiveTexture)
					{
						new_material->SetEmissiveTexture(mTextures->Find(material->mEmissiveTexture->mName, true));
					}

					if (material->mBumpTexture)
					{
						new_material->SetBumpTexture(mTextures->Find(material->mBumpTexture->mName, true));
					}

					if (material->mDepthTexture)
					{
						new_material->SetDepthTexture(mTextures->Find(material->mDepthTexture->mName, true));
					}

					new_material->Contour = material->Contour;
					new_material->ContourColor = material->ContourColor;
					new_material->ContourThickness = material->ContourThickness;
				}

				file->mMaterials->Next();
			}

			file->mTextures->First();
			while (MeshTexture* texture = file->mTextures->operator ()())
			{
				MeshTexture* new_texture = mTextures->Find(texture->mName, true);

				if (!new_texture->mPixels)
				{
					new_texture->SetPixels(texture->mWidth, texture->mHeight, texture->mDepth, texture->mPixels);
				}

				file->mTextures->Next();
			}

			file->mObjects->First();
			while (MeshObject* object = file->mObjects->operator ()())
			{
				MeshObject* new_object = mObjects->Find(object->mName, true);

				if (object->mNode)
				{
					new_object->SetNode(object->mNode->mName);
					new_object->mOffsetMatrix = object->mOffsetMatrix;
				}

				for (std::vector<MeshSkin*>::iterator skin = object->mSkins->mSkins.begin(); skin != object->mSkins->mSkins.end(); ++skin)
				{
					MeshSkin* new_skin = new_object->mSkins->Find((*skin)->mNode->mName, true);
					new_skin->OffsetMatrix = (*skin)->OffsetMatrix;
				}

				int vertexOffset = new_object->mAnimatedVertices->mAnimatedVertices.size();
				object->mAnimatedVertices->First();
				MeshAnimatedVertex* animated_vertex;
				while (animated_vertex = object->mAnimatedVertices->operator ()())
				{
					MeshAnimatedVertex* new_animated_vertex = new_object->mAnimatedVertices->New(animated_vertex->mVertex);

					for (std::vector<MeshSkinWeight*>::iterator skin_weight = animated_vertex->mSkinWeights.begin(); skin_weight != animated_vertex->mSkinWeights.end(); ++skin_weight)
					{
						new_animated_vertex->AppendSkinWeight((*skin_weight)->mSkinWeights->mSkin->mNode->mName, (*skin_weight)->mWeight);
					}

					for (std::vector<MeshMorphTargetVertex*>::iterator morph_target_vertex = animated_vertex->mMorphTargetVertices.begin(); morph_target_vertex != animated_vertex->mMorphTargetVertices.end(); ++morph_target_vertex)
					{
						new_animated_vertex->AppendMorphTargetVertex((*morph_target_vertex)->mMorphTargetVertices->mMorphTarget->mName, (*morph_target_vertex)->mVertex);
					}

					object->mAnimatedVertices->Next();
				}

				MeshFace* face;
				object->mFaces->First();
				while (face = object->mFaces->operator ()())
				{
					std::vector<int> vertexIndices;
					for (std::vector<MeshAnimatedVertex*>::iterator animated_vertex = face->mAnimatedVertices.begin(); animated_vertex != face->mAnimatedVertices.end(); ++animated_vertex)
					{
						vertexIndices.push_back((*animated_vertex)->mIndex + vertexOffset);
					}
					if (!face->mGroup)
					{
						new_object->mFaces->New((MeshGroup*)0, vertexIndices);
					}
					else if (!face->mGroup->mMaterial)
					{
						new_object->mFaces->New(-1, vertexIndices);
					}
					else
					{
						new_object->mFaces->New(face->mGroup->mMaterial->mName, vertexIndices);
					}

					object->mFaces->Next();
				}

				new_object->UpdateArrays();

				file->mObjects->Next();
			}

			file->mPhysics->mRigidBodies->First();
			while (MeshRigidBody* rigid_body = file->mPhysics->mRigidBodies->operator ()())
			{
				MeshRigidBody* new_rigid_body = mPhysics->mRigidBodies->Find(rigid_body->mName, true);

				if (rigid_body->mNode)
				{
					new_rigid_body->SetNode(mNodes->Find(rigid_body->mNode->mName, true));
				}

				new_rigid_body->mCollisionGroup = rigid_body->mCollisionGroup;
				new_rigid_body->mShape = rigid_body->mShape;
				new_rigid_body->mSize = rigid_body->mSize;
				new_rigid_body->mInitialTranslation = rigid_body->mInitialTranslation;
				new_rigid_body->mInitialRotation = rigid_body->mInitialRotation;
				new_rigid_body->mMass = rigid_body->mMass;
				new_rigid_body->mLinearDamping = rigid_body->mLinearDamping;
				new_rigid_body->mAngularDamping = rigid_body->mAngularDamping;
				new_rigid_body->mBounce = rigid_body->mBounce;
				new_rigid_body->mFriction = rigid_body->mFriction;
				new_rigid_body->mType = rigid_body->mType;

				new_rigid_body->LoadInitialConditions();

				file->mPhysics->mRigidBodies->Next();
			}

			file->mPhysics->mJoints->First();
			while (MeshJoint* joint = file->mPhysics->mJoints->operator ()())
			{
				MeshJoint* new_joint = mPhysics->mJoints->Find(joint->mName, true);
				if (joint->mNode)
				{
					new_joint->SetNode(mNodes->Find(joint->mNode->mName, true));
				}

				new_joint->mType = joint->mType;

				if (joint->mFirstBody)
				{
					new_joint->SetFirstBody(mPhysics->mRigidBodies->Find(joint->mFirstBody->mName, true));
				}

				if (joint->mSecondBody)
				{
					new_joint->SetSecondBody(mPhysics->mRigidBodies->Find(joint->mSecondBody->mName, true));
				}

				new_joint->mTranslation = joint->mTranslation;
				new_joint->mRotation = joint->mRotation;
				new_joint->mLoStop = joint->mLoStop;
				new_joint->mHiStop = joint->mHiStop;
				new_joint->mBounce = joint->mBounce;
				new_joint->mCFM = joint->mCFM;
				new_joint->mStopERP = joint->mStopERP;
				new_joint->mStopCFM = joint->mStopCFM;

				new_joint->Update();

				file->mPhysics->mJoints->Next();
			}
		}

		void MeshFile::Update(anim::AnimKeyFrame* anim_key_frame)
		{
			anim_key_frame->GetNodes()->First();
			anim::AnimNode* anim_node;
			while (anim_node = anim_key_frame->GetNodes()->operator()())
			{
				MeshNode* node = mNodes->Find(anim_node->GetName());
				if (node)
				{
					node->mAdditionalMatrix = anim_node->GetMatrix();
				}
				anim_key_frame->GetNodes()->Next();
			}
			mNodes->Update();

			anim_key_frame->GetObjects()->First();
			anim::AnimObject* anim_object;
			while (anim_object = anim_key_frame->GetObjects()->operator()())
			{
				MeshObject* object = mObjects->Find(anim_object->GetName());
				if (object)
				{
					object->mVisible = anim_object->Visible();

					anim_object->GetMorphTargets()->First();
					anim::AnimMorphTarget* anim_morph_target;
					while (anim_morph_target = anim_object->GetMorphTargets()->operator()())
					{
						MeshMorphTarget* morph_target = object->mMorphTargets->Find(anim_morph_target->GetName());
						if (morph_target)
						{
							morph_target->mBlendFactor = anim_morph_target->GetBlendFactor();
						}
						anim_object->GetMorphTargets()->Next();
					}
				}
				anim_key_frame->GetObjects()->Next();
			}
			mObjects->Update();
		}

		void MeshFile::Update(pose::PosePose* pose_pose)
		{
			pose_pose->Nodes()->First();
			pose::PoseNode* pose_node;
			while (pose_node = pose_pose->Nodes()->operator()())
			{
				MeshNode* node = mNodes->Find(pose_node->Name());
				if (node)
				{
					node->mAdditionalMatrix = pose_node->GetMatrix();
				}
				pose_pose->Nodes()->Next();
			}
			mNodes->Update();

			pose_pose->Objects()->First();
			pose::PoseObject* pose_object;
			while (pose_object = pose_pose->Objects()->operator()())
			{
				MeshObject* object = mObjects->Find(pose_object->Name());
				if (object)
				{
					object->mVisible = pose_object->Visible();

					pose_object->MorphTargets()->First();
					pose::PoseMorphTarget* pose_morph_target;
					while (pose_morph_target = pose_object->MorphTargets()->operator()())
					{
						MeshMorphTarget* morph_target = object->mMorphTargets->Find(pose_morph_target->Name());
						if (morph_target)
						{
							morph_target->mBlendFactor = pose_morph_target->GetBlendFactor();
						}
						pose_object->MorphTargets()->Next();
					}
				}
				pose_pose->Objects()->Next();
			}
			mObjects->Update();
		}
	}
}
