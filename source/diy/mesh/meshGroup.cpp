#include <diy/diy.hpp>

#include <algorithm>
#include <sstream>

#include <gl/glew.h>

namespace diy
{
	namespace mesh
	{
		MeshGroup::MeshGroup(MeshGroups* groups, MeshMaterial* material, int index) :
			mGroups(groups),
			mMaterial(0),
			mIndex(index)
		{
			mShader = new Shader();

			SetMaterial(material);
		}

		MeshGroup::~MeshGroup(void)
		{
			while (mFaces.size())
			{
				mFaces.back()->mGroup = 0;
				mFaces.pop_back();
			}
			mFaces.swap(std::vector<MeshFace*>(mFaces));

			SetMaterial((MeshMaterial*)0);

			delete mShader;
		}

		MeshGroups* MeshGroup::GetGroups(void)
		{
			return mGroups;
		}

		MeshMaterial* MeshGroup::GetMaterial(void)
		{
			return mMaterial;
		}

		int MeshGroup::GetIndex(void)
		{
			return mIndex;
		}

		std::vector<class MeshFace*>& MeshGroup::GetFaces(void)
		{
			return mFaces;
		}

		void MeshGroup::AppendFace(MeshFace* face)
		{
			if (face)
			{
				if (face->mGroup)
				{
					face->mGroup->RemoveFace(face);
				}

				mFaces.push_back(face);
				face->mGroup = this;
			}
		}

		void MeshGroup::AppendFace(int faceIndex)
		{
			AppendFace(mGroups->mObject->mFaces->operator[](faceIndex));
		}

		void MeshGroup::RemoveFace(MeshFace* face)
		{
			std::vector<MeshFace*>::iterator iterator = find(mFaces.begin(), mFaces.end(), face);

			if (iterator == mFaces.end())
			{
				return;
			}

			mFaces.erase(iterator);
			face->mGroup = 0;

			if (mFaces.empty())
			{
				mGroups->Delete(this);
			}
		}

		void MeshGroup::RemoveFace(int faceIndex)
		{
			RemoveFace(mGroups->mObject->mFaces->operator[](faceIndex));
		}

		void MeshGroup::SetMaterial(MeshMaterial* material)
		{
			if (mMaterial)
			{
				std::vector<MeshGroup*>::iterator group = find(mMaterial->mGroups.begin(), mMaterial->mGroups.end(), this);
				if (group != mMaterial->mGroups.end())
				{
					mMaterial->mGroups.erase(group);
				}
			}

			if (material)
			{
				material->mGroups.push_back(this);
			}

			mMaterial = material;

			Update();
		}

		void MeshGroup::SetMaterial(int index)
		{
			SetMaterial(mGroups->mObject->mObjects->mFile->mMaterials->operator [](index));
		}

		void MeshGroup::SetMaterial(std::string name)
		{
			SetMaterial(mGroups->mObject->mObjects->mFile->mMaterials->Find(name));
		}

		void MeshGroup::RenderUV(void)
		{
			glDisable(GL_LIGHTING);
			glDisable(GL_COLOR_MATERIAL);
			glDisable(GL_CULL_FACE);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDisable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);

			glEnableClientState(GL_VERTEX_ARRAY);

			glColor3f(1.0f, 1.0f, 1.0f);
			glVertexPointer(2, GL_FLOAT, 0, mGroups->mObject->mAnimatedVertices->mTexCoordArray);
			glDrawElements(GL_TRIANGLES, 3 * mFaces.size(), GL_UNSIGNED_INT, mGroups->mObject->mFaces->mIndexArray + 3 * mFirstFace);

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glEnable(GL_CULL_FACE);

			glDisableClientState(GL_VERTEX_ARRAY);
		}

		int MeshGroup::GetIndexCount(void)
		{
			return 3 * mFaces.size();
		}

		const void* MeshGroup::GetIndexPointer(void)
		{
			return mGroups->mObject->mFaces->mIndexArray + 3 * mFirstFace;
		}

		void MeshGroup::Update(void)
		{
			mShader->Clear();

			std::ostringstream vert;
			std::ostringstream frag;

			if (!mMaterial)
			{
				vert << "uniform mat4 uPMVMatrix;" << std::endl;
				vert << "attribute vec3 aPosition;" << std::endl;
				vert << "void main()" << std::endl;
				vert << "{" << std::endl;
				vert << "   gl_Position = uPMVMatrix * vec4(aPosition, 1.0);" << std::endl;
				vert << "}" << std::endl;

				frag << "void main()" << std::endl;
				frag << "{" << std::endl;
				frag << "	gl_FragColor = vec4(1.0);" << std::endl;
				frag << "}" << std::endl;
			}
			else
			{
				switch (mMaterial->mShader)
				{
				case MeshMaterial::SHADER_CONSTANT:
				{
					vert << "uniform mat4 uPMVMatrix;" << std::endl;
					vert << "attribute vec3 aPosition;" << std::endl;
					if (mMaterial->mDiffuseTexture)
					{
						vert << "attribute vec2 aTexCoord;" << std::endl;
						vert << "varying vec2 vTexCoord;" << std::endl;
					}
					vert << "void main()" << std::endl;
					vert << "{" << std::endl;
					vert << "gl_Position = uPMVMatrix * vec4(aPosition, 1.0);" << std::endl;
					if (mMaterial->mDiffuseTexture)
					{
						vert << "vTexCoord = aTexCoord;" << std::endl;
					}
					vert << "}" << std::endl;

					if (mMaterial->mDiffuseTexture)
					{
						frag << "uniform sampler2D uDiffuseTexture;" << std::endl;
						frag << "varying vec2 vTexCoord;" << std::endl;
					}
					frag << "void main()" << std::endl;
					frag << "{" << std::endl;
					frag << "vec4 color = vec4(" << mMaterial->mDiffuse[0] << ", " << mMaterial->mDiffuse[1] << ", " << mMaterial->mDiffuse[2] << ", " << mMaterial->mDiffuse[3] << ");" << std::endl;
					if (mMaterial->mDiffuseTexture)
					{
						frag << "color *= texture2D(uDiffuseTexture, vTexCoord);" << std::endl;
					}
					frag << "gl_FragColor = color;" << std::endl;
					frag << "}" << std::endl;
				}
				break;
				case MeshMaterial::SHADER_LAMBERT:
				{
					vert << "uniform mat4 uPMVMatrix;" << std::endl;
					vert << "attribute vec3 aPosition;" << std::endl;
					vert << "uniform mat3 uNMatrix;" << std::endl;
					vert << "attribute vec3 aNormal;" << std::endl;
					vert << "varying vec3 vNormal;" << std::endl;
					if (mMaterial->mDiffuseTexture)
					{
						vert << "attribute vec2 aTexCoord;" << std::endl;
						vert << "varying vec2 vTexCoord;" << std::endl;
					}
					vert << "void main()" << std::endl;
					vert << "{" << std::endl;
					vert << "gl_Position = uPMVMatrix * vec4(aPosition, 1.0);" << std::endl;
					vert << "vNormal = normalize(uNMatrix * aNormal);" << std::endl;
					if (mMaterial->mDiffuseTexture)
					{
						vert << "vTexCoord = aTexCoord;" << std::endl;
					}
					vert << "}" << std::endl;

					frag << "varying vec3 vNormal;" << std::endl;
					if (mMaterial->mDiffuseTexture)
					{
						frag << "uniform sampler2D uDiffuseTexture;" << std::endl;
						frag << "varying vec2 vTexCoord;" << std::endl;
					}
					frag << "void main()" << std::endl;
					frag << "{" << std::endl;
					frag << "vec3 normal = normalize(vNormal);" << std::endl;
					frag << "float diffuseFactor = max(0.0, dot(normal, vec3(0.0, 0.0, 1.0)));" << std::endl;
					frag << "vec4 color = vec4(" << mMaterial->mDiffuse[0] << ", " << mMaterial->mDiffuse[1] << ", " << mMaterial->mDiffuse[2] << ", " << mMaterial->mDiffuse[3] << ");" << std::endl;
					if (mMaterial->mDiffuseTexture)
					{
						frag << "color *= texture2D(uDiffuseTexture, vTexCoord);" << std::endl;
					}
					frag << "gl_FragColor = vec4(diffuseFactor * color.rgb, color.a);" << std::endl;
					frag << "}" << std::endl;
				}
				break;
				default:
				{
					vert << "uniform mat4 uPMVMatrix;" << std::endl;
					vert << "attribute vec3 aPosition;" << std::endl;
					vert << "void main()" << std::endl;
					vert << "{" << std::endl;
					vert << "   gl_Position = uPMVMatrix * vec4(aPosition, 1.0);" << std::endl;
					vert << "}" << std::endl;

					frag << "void main()" << std::endl;
					frag << "{" << std::endl;
					frag << "	gl_FragColor = vec4(1.0);" << std::endl;
					frag << "}" << std::endl;
				}
				}
			}

			mShader->CompileSource(vert.str().c_str(), GL_VERTEX_SHADER);
			mShader->CompileSource(frag.str().c_str(), GL_FRAGMENT_SHADER);

			mShader->Link();
		}

		void MeshGroup::Render(Camera* camera)
		{
			MeshObject* object = mGroups->mObject;
			glm::mat4 matrix = camera->GetProjectionMatrix() * camera->GetFinalMatrix();

			mShader->Begin();

			mShader->Uniform(GL_FLOAT_MAT4, mShader->GetUniformLocation("uPMVMatrix"), 1, glm::value_ptr(matrix));

			if (mMaterial)
			{
				switch (mMaterial->mShader)
				{
				case MeshMaterial::SHADER_CONSTANT:
				{
					if (mMaterial->mDiffuseTexture)
					{
						glActiveTexture(GL_TEXTURE1);
						glBindTexture(GL_TEXTURE_2D, mMaterial->mDiffuseTexture->mHandle);
						mShader->Uniform(GL_SAMPLER_2D, mShader->GetUniformLocation("uDiffuseTexture"), 1);
						mShader->EnableVertexAttribArray(mShader->GetAttribLocation("aPosition"));
						mShader->VertexAttribPointer(GL_FLOAT_VEC3, mShader->GetAttribLocation("aPosition"), object->mAnimatedVertices->mPositionArray);
						mShader->EnableVertexAttribArray(mShader->GetAttribLocation("aTexCoord"));
						mShader->VertexAttribPointer(GL_FLOAT_VEC2, mShader->GetAttribLocation("aTexCoord"), object->mAnimatedVertices->mTexCoordArray);
						glDrawElements(GL_TRIANGLES, 3 * mFaces.size(), GL_UNSIGNED_INT, object->mFaces->mIndexArray + 3 * mFirstFace);
						mShader->DisableVertexAttribArray(mShader->GetAttribLocation("aPosition"));
						mShader->DisableVertexAttribArray(mShader->GetAttribLocation("aTexCoord"));
						glActiveTexture(GL_TEXTURE0);
					}
					else
					{
						mShader->EnableVertexAttribArray(mShader->GetAttribLocation("aPosition"));
						mShader->VertexAttribPointer(GL_FLOAT_VEC3, mShader->GetAttribLocation("aPosition"), object->mAnimatedVertices->mPositionArray);
						glDrawElements(GL_TRIANGLES, 3 * mFaces.size(), GL_UNSIGNED_INT, object->mFaces->mIndexArray + 3 * mFirstFace);
						mShader->DisableVertexAttribArray(mShader->GetAttribLocation("aPosition"));
					}
				}
				break;
				case MeshMaterial::SHADER_LAMBERT:
				{
					if (mMaterial->mDiffuseTexture)
					{
						mShader->Uniform(GL_FLOAT_MAT3, mShader->GetUniformLocation("uNMatrix"), 1, glm::value_ptr(camera->GetNormalFinalMatrix()));
						glActiveTexture(GL_TEXTURE1);
						glBindTexture(GL_TEXTURE_2D, mMaterial->mDiffuseTexture->mHandle);
						mShader->Uniform(GL_SAMPLER_2D, mShader->GetUniformLocation("uDiffuseTexture"), 1);
						mShader->EnableVertexAttribArray(mShader->GetAttribLocation("aPosition"));
						mShader->VertexAttribPointer(GL_FLOAT_VEC3, mShader->GetAttribLocation("aPosition"), object->mAnimatedVertices->mPositionArray);
						mShader->EnableVertexAttribArray(mShader->GetAttribLocation("aNormal"));
						mShader->VertexAttribPointer(GL_FLOAT_VEC3, mShader->GetAttribLocation("aNormal"), object->mAnimatedVertices->mNormalArray);
						mShader->EnableVertexAttribArray(mShader->GetAttribLocation("aTexCoord"));
						mShader->VertexAttribPointer(GL_FLOAT_VEC2, mShader->GetAttribLocation("aTexCoord"), object->mAnimatedVertices->mTexCoordArray);
						glDrawElements(GL_TRIANGLES, 3 * mFaces.size(), GL_UNSIGNED_INT, object->mFaces->mIndexArray + 3 * mFirstFace);
						mShader->DisableVertexAttribArray(mShader->GetAttribLocation("aPosition"));
						mShader->DisableVertexAttribArray(mShader->GetAttribLocation("aNormal"));
						mShader->DisableVertexAttribArray(mShader->GetAttribLocation("aTexCoord"));
						glActiveTexture(GL_TEXTURE0);
					}
					else
					{
						mShader->Uniform(GL_FLOAT_MAT3, mShader->GetUniformLocation("uNMatrix"), 1, glm::value_ptr(camera->GetNormalFinalMatrix()));
						mShader->EnableVertexAttribArray(mShader->GetAttribLocation("aPosition"));
						mShader->VertexAttribPointer(GL_FLOAT_VEC3, mShader->GetAttribLocation("aPosition"), object->mAnimatedVertices->mPositionArray);
						mShader->EnableVertexAttribArray(mShader->GetAttribLocation("aNormal"));
						mShader->VertexAttribPointer(GL_FLOAT_VEC3, mShader->GetAttribLocation("aNormal"), object->mAnimatedVertices->mNormalArray);
						glDrawElements(GL_TRIANGLES, 3 * mFaces.size(), GL_UNSIGNED_INT, object->mFaces->mIndexArray + 3 * mFirstFace);
						mShader->DisableVertexAttribArray(mShader->GetAttribLocation("aPosition"));
						mShader->DisableVertexAttribArray(mShader->GetAttribLocation("aNormal"));
					}
				}
				break;
				default:
				{
					mShader->EnableVertexAttribArray(mShader->GetAttribLocation("aPosition"));
					mShader->VertexAttribPointer(GL_FLOAT_VEC3, mShader->GetAttribLocation("aPosition"), object->mAnimatedVertices->mPositionArray);
					glDrawElements(GL_TRIANGLES, 3 * mFaces.size(), GL_UNSIGNED_INT, object->mFaces->mIndexArray + 3 * mFirstFace);
					mShader->DisableVertexAttribArray(mShader->GetAttribLocation("aPosition"));
				}
				}
			}
			else
			{
				mShader->EnableVertexAttribArray(mShader->GetAttribLocation("aPosition"));
				mShader->VertexAttribPointer(GL_FLOAT_VEC3, mShader->GetAttribLocation("aPosition"), object->mAnimatedVertices->mPositionArray);
				glDrawElements(GL_TRIANGLES, 3 * mFaces.size(), GL_UNSIGNED_INT, object->mFaces->mIndexArray + 3 * mFirstFace);
				mShader->DisableVertexAttribArray(mShader->GetAttribLocation("aPosition"));
			}

			mShader->End();
		}
	}
}