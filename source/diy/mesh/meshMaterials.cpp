#include <diy/diy.hpp>

#include <glm/ext.hpp>

#include <algorithm>

namespace diy
{
	namespace mesh
	{
		MeshMaterials::MeshMaterials(MeshFile* scene) :
			mFile(scene)
		{
			mMaterial = mMaterials.end();
		}

		MeshMaterials::~MeshMaterials(void)
		{
			Clear();
		}

		MeshFile* MeshMaterials::GetFile(void)
		{
			return mFile;
		}

		int MeshMaterials::GetCount(void)
		{
			return mMaterials.size();
		}

		MeshMaterial* MeshMaterials::New(void)
		{
			int index = 0;
			char name[256];

			MeshMaterial* new_material;
			do
			{
				sprintf(name, "Material%i", index++);
				new_material = New(name);
			} while (!new_material);

			return new_material;
		}

		MeshMaterial* MeshMaterials::New(std::string name)
		{
			if (name.empty())
			{
				return 0;
			}

			if (Find(name))
			{
				return 0;
			}

			mMaterials.push_back(new MeshMaterial(this, mMaterials.size(), name));

			return mMaterials.back();
		}

		void MeshMaterials::Clear(void)
		{
			while (mMaterials.size())
			{
				delete mMaterials.back();
				mMaterials.pop_back();
			}
			mMaterials.swap(std::vector<MeshMaterial*>(mMaterials));

			mMaterial = mMaterials.end();
		}

		bool MeshMaterials::Delete(MeshMaterial* material)
		{
			std::vector<MeshMaterial*>::iterator iterator = find(mMaterials.begin(), mMaterials.end(), material);

			if (iterator == mMaterials.end())
			{
				return false;
			}

			if ((*iterator)->IsInUse())
			{
				return false;
			}

			delete *iterator;
			iterator = mMaterials.erase(iterator);

			for (iterator = iterator; iterator != mMaterials.end(); ++iterator)
			{
				--(*iterator)->mIndex;
			}

			return true;
		}

		void MeshMaterials::DeleteUnused(void)
		{
			for (std::vector<MeshMaterial*>::iterator material = mMaterials.begin(); material != mMaterials.end();)
			{
				if (!(*material)->IsInUse())
				{
					delete *material;
					mMaterials.erase(material);
					material = mMaterials.begin();
					continue;
				}
				++material;
			}

			int index = 0;
			for (std::vector<MeshMaterial*>::iterator material = mMaterials.begin(); material != mMaterials.end(); ++material)
			{
				(*material)->mIndex = index++;
			}
		}

		void MeshMaterials::MergeDuplicated(void)
		{
			for (std::vector<MeshMaterial*>::iterator material_a = mMaterials.begin(); material_a != mMaterials.end(); ++material_a)
			{
				for (std::vector<MeshMaterial*>::iterator material_b = material_a + 1; material_b != mMaterials.end(); ++material_b)
				{
					if ((*material_a)->mDiffuseTexture == (*material_b)->mDiffuseTexture && glm::length((*material_a)->mDiffuse - (*material_b)->mDiffuse) < 0.05f)
					{
						while ((*material_b)->mGroups.size())
						{
							(*material_b)->mGroups.front()->SetMaterial(*material_a);
						}
						delete *material_b;
						material_b = mMaterials.erase(material_b);
						--material_b;
					}
				}
			}

			int index = 0;
			for (std::vector<MeshMaterial*>::iterator material = mMaterials.begin(); material != mMaterials.end(); ++material)
			{
				(*material)->mIndex = index++;
			}
		}

		bool MeshMaterials::LoadFromFStream(std::ifstream& in)
		{
			int count;
			in.read(reinterpret_cast<char*>(&count), 4);

			mMaterials.reserve(count);

			for (int i = 0; i < count; ++i)
			{
				int length;

				in.read(reinterpret_cast<char*>(&length), 4);
				char* name = new char[length];
				in.read(name, length);
				MeshMaterial* material = Find(name, true);
				delete[] name;

				in.read(reinterpret_cast<char*>(&length), 4);
				char* comment = new char[length];
				in.read(comment, length);
				material->mComment = std::string(comment);
				delete[] comment;

				in.read(reinterpret_cast<char*>(&material->mShader), 4);
				in.read(reinterpret_cast<char*>(&material->mType), 4);
				in.read(reinterpret_cast<char*>(&material->mMode), 4);

				in.read(reinterpret_cast<char*>(glm::value_ptr(material->mAmbient)), 16);
				in.read(reinterpret_cast<char*>(glm::value_ptr(material->mDiffuse)), 16);
				in.read(reinterpret_cast<char*>(glm::value_ptr(material->mSpecular)), 16);
				in.read(reinterpret_cast<char*>(glm::value_ptr(material->mEmissive)), 16);
				in.read(reinterpret_cast<char*>(glm::value_ptr(material->mReflective)), 16);

				in.read(reinterpret_cast<char*>(&material->mShininess), 4);
				in.read(reinterpret_cast<char*>(&material->mReflection), 4);
				in.read(reinterpret_cast<char*>(&material->mRefraction), 4);

				int channel;

				int ambientLength;
				in.read(reinterpret_cast<char*>(&ambientLength), 4);
				if (ambientLength)
				{
					char* ambientName = new char[ambientLength];
					in.read(ambientName, ambientLength);
					material->SetAmbientTexture(mFile->mTextures->Find(ambientName, true));
					delete[] ambientName;
				}
				in.read(reinterpret_cast<char*>(&channel), 4);

				int diffuseLength;
				in.read(reinterpret_cast<char*>(&diffuseLength), 4);
				if (diffuseLength)
				{
					char* diffuseName = new char[diffuseLength];
					in.read(diffuseName, diffuseLength);
					material->SetDiffuseTexture(mFile->mTextures->Find(diffuseName, true));
					delete[] diffuseName;
				}
				in.read(reinterpret_cast<char*>(&channel), 4);

				int alphaLength;
				in.read(reinterpret_cast<char*>(&alphaLength), 4);
				if (alphaLength)
				{
					char* alphaName = new char[alphaLength];
					in.read(alphaName, alphaLength);
					material->SetAlphaTexture(mFile->mTextures->Find(alphaName, true));
					delete[] alphaName;
				}
				in.read(reinterpret_cast<char*>(&channel), 4);

				int specularLength;
				in.read(reinterpret_cast<char*>(&specularLength), 4);
				if (specularLength)
				{
					char* specularName = new char[specularLength];
					in.read(specularName, specularLength);
					material->SetSpecularTexture(mFile->mTextures->Find(specularName, true));
					delete[] specularName;
				}
				in.read(reinterpret_cast<char*>(&channel), 4);

				int shininessLength;
				in.read(reinterpret_cast<char*>(&shininessLength), 4);
				if (shininessLength)
				{
					char* shininessName = new char[shininessLength];
					in.read(shininessName, shininessLength);
					material->SetShininessTexture(mFile->mTextures->Find(shininessName, true));
					delete[] shininessName;
				}
				in.read(reinterpret_cast<char*>(&channel), 4);

				int emissiveLength;
				in.read(reinterpret_cast<char*>(&emissiveLength), 4);
				if (emissiveLength)
				{
					char* emissiveName = new char[emissiveLength];
					in.read(emissiveName, emissiveLength);
					material->SetEmissiveTexture(mFile->mTextures->Find(emissiveName, true));
					delete[] emissiveName;
				}
				in.read(reinterpret_cast<char*>(&channel), 4);

				int bumpLength;
				in.read(reinterpret_cast<char*>(&bumpLength), 4);
				if (bumpLength)
				{
					char* bumpName = new char[bumpLength];
					in.read(bumpName, bumpLength);
					material->SetBumpTexture(mFile->mTextures->Find(bumpName, true));
					delete[] bumpName;
				}
				in.read(reinterpret_cast<char*>(&channel), 4);

				int depthLength;
				in.read(reinterpret_cast<char*>(&depthLength), 4);
				if (depthLength)
				{
					char* depthName = new char[depthLength];
					in.read(depthName, depthLength);
					material->SetDepthTexture(mFile->mTextures->Find(depthName, true));
					delete[] depthName;
				}
				in.read(reinterpret_cast<char*>(&channel), 4);

				int ambientOcclusionLength;
				in.read(reinterpret_cast<char*>(&ambientOcclusionLength), 4);
				if (ambientOcclusionLength)
				{
					char* ambientOcclusionName = new char[ambientOcclusionLength];
					in.read(ambientOcclusionName, ambientOcclusionLength);
					material->SetAmbientOcclusionTexture(mFile->mTextures->Find(ambientOcclusionName, true));
					delete[] ambientOcclusionName;
				}
				in.read(reinterpret_cast<char*>(&channel), 4);

				int displacementLength;
				in.read(reinterpret_cast<char*>(&displacementLength), 4);
				if (displacementLength)
				{
					char* displacementName = new char[displacementLength];
					in.read(displacementName, displacementLength);
					material->SetDisplacementTexture(mFile->mTextures->Find(displacementName, true));
					delete[] displacementName;
				}
				in.read(reinterpret_cast<char*>(&channel), 4);

				int stepConeLength;
				in.read(reinterpret_cast<char*>(&stepConeLength), 4);
				if (stepConeLength)
				{
					char* stepConeName = new char[stepConeLength];
					in.read(stepConeName, stepConeLength);
					material->SetStepConeTexture(mFile->mTextures->Find(stepConeName, true));
					delete[] stepConeName;
				}
				in.read(reinterpret_cast<char*>(&channel), 4);

				int environmentLength;
				in.read(reinterpret_cast<char*>(&environmentLength), 4);
				if (environmentLength)
				{
					char* environmentName = new char[environmentLength];
					in.read(environmentName, environmentLength);
					material->SetEnvironmentTexture(mFile->mTextures->Find(environmentName, true));
					delete[] environmentName;
				}
				in.read(reinterpret_cast<char*>(&channel), 4);

				int reflectiveLength;
				in.read(reinterpret_cast<char*>(&reflectiveLength), 4);
				if (reflectiveLength)
				{
					char* reflectiveName = new char[reflectiveLength];
					in.read(reflectiveName, reflectiveLength);
					material->SetReflectiveTexture(mFile->mTextures->Find(reflectiveName, true));
					delete[] reflectiveName;
				}
				in.read(reinterpret_cast<char*>(&channel), 4);

				int reflectionLength;
				in.read(reinterpret_cast<char*>(&reflectionLength), 4);
				if (reflectionLength)
				{
					char* reflectionName = new char[reflectionLength];
					in.read(reflectionName, reflectionLength);
					material->SetReflectionTexture(mFile->mTextures->Find(reflectionName, true));
					delete[] reflectionName;
				}
				in.read(reinterpret_cast<char*>(&channel), 4);

				int refractionLength;
				in.read(reinterpret_cast<char*>(&refractionLength), 4);
				if (refractionLength)
				{
					char* refractionName = new char[refractionLength];
					in.read(refractionName, refractionLength);
					material->SetRefractionTexture(mFile->mTextures->Find(refractionName, true));
					delete[] refractionName;
				}
				in.read(reinterpret_cast<char*>(&channel), 4);

				int toonLength;
				in.read(reinterpret_cast<char*>(&toonLength), 4);
				if (toonLength)
				{
					char* toonName = new char[toonLength];
					in.read(toonName, toonLength);
					material->SetToonTexture(mFile->mTextures->Find(toonName, true));
					delete[] toonName;
				}
				in.read(reinterpret_cast<char*>(&channel), 4);

				in.read(reinterpret_cast<char*>(&material->Contour), 4);
				in.read(reinterpret_cast<char*>(glm::value_ptr(material->ContourColor)), 16);
				in.read(reinterpret_cast<char*>(&material->ContourThickness), 4);

				int endMaterial;
				in.read(reinterpret_cast<char*>(&endMaterial), 4);

				material->Update();
			}

			int endMaterials;
			in.read(reinterpret_cast<char*>(&endMaterials), 4);

			return true;
		}

		void MeshMaterials::SaveToFStream(std::ofstream& out)
		{
			int count = mMaterials.size();
			out.write(reinterpret_cast<char*>(&count), 4);

			for (std::vector<MeshMaterial*>::iterator material = mMaterials.begin(); material != mMaterials.end(); ++material)
			{
				int length = strlen((*material)->mName.c_str()) + 1;
				out.write(reinterpret_cast<char*>(&length), 4);
				out.write((*material)->mName.c_str(), length);
				length = strlen((*material)->mComment.c_str()) + 1;
				out.write(reinterpret_cast<char*>(&length), 4);
				out.write((*material)->mComment.c_str(), length);

				out.write(reinterpret_cast<char*>(&(*material)->mShader), 4);
				out.write(reinterpret_cast<char*>(&(*material)->mType), 4);
				out.write(reinterpret_cast<char*>(&(*material)->mMode), 4);

				out.write(reinterpret_cast<char*>(glm::value_ptr((*material)->mAmbient)), 16);
				out.write(reinterpret_cast<char*>(glm::value_ptr((*material)->mDiffuse)), 16);
				out.write(reinterpret_cast<char*>(glm::value_ptr((*material)->mSpecular)), 16);
				out.write(reinterpret_cast<char*>(glm::value_ptr((*material)->mEmissive)), 16);
				out.write(reinterpret_cast<char*>(glm::value_ptr((*material)->mReflective)), 16);

				out.write(reinterpret_cast<char*>(&(*material)->mShininess), 4);
				out.write(reinterpret_cast<char*>(&(*material)->mReflection), 4);
				out.write(reinterpret_cast<char*>(&(*material)->mRefraction), 4);

				int channel = 0;

				if ((*material)->mAmbientTexture)
				{
					int ambientLength = strlen((*material)->mAmbientTexture->mName.c_str()) + 1;
					out.write(reinterpret_cast<char*>(&ambientLength), 4);
					out.write((*material)->mAmbientTexture->mName.c_str(), ambientLength);
				}
				else
				{
					int ambientLength = 0;
					out.write(reinterpret_cast<char*>(&ambientLength), 4);
				}
				out.write(reinterpret_cast<char*>(&channel), 4);

				if ((*material)->mDiffuseTexture)
				{
					int diffuseLength = strlen((*material)->mDiffuseTexture->mName.c_str()) + 1;
					out.write(reinterpret_cast<char*>(&diffuseLength), 4);
					out.write((*material)->mDiffuseTexture->mName.c_str(), diffuseLength);
				}
				else
				{
					int diffuseLength = 0;
					out.write(reinterpret_cast<char*>(&diffuseLength), 4);
				}
				out.write(reinterpret_cast<char*>(&channel), 4);

				if ((*material)->mAlphaTexture)
				{
					int alphaLength = strlen((*material)->mAlphaTexture->mName.c_str()) + 1;
					out.write(reinterpret_cast<char*>(&alphaLength), 4);
					out.write((*material)->mAlphaTexture->mName.c_str(), alphaLength);
				}
				else
				{
					int alphaLength = 0;
					out.write(reinterpret_cast<char*>(&alphaLength), 4);
				}
				out.write(reinterpret_cast<char*>(&channel), 4);

				if ((*material)->mSpecularTexture)
				{
					int specularLength = strlen((*material)->mSpecularTexture->mName.c_str()) + 1;
					out.write(reinterpret_cast<char*>(&specularLength), 4);
					out.write((*material)->mSpecularTexture->mName.c_str(), specularLength);
				}
				else
				{
					int specularLength = 0;
					out.write(reinterpret_cast<char*>(&specularLength), 4);
				}
				out.write(reinterpret_cast<char*>(&channel), 4);

				if ((*material)->mShininessTexture)
				{
					int specularLength = strlen((*material)->mShininessTexture->mName.c_str()) + 1;
					out.write(reinterpret_cast<char*>(&specularLength), 4);
					out.write((*material)->mShininessTexture->mName.c_str(), specularLength);
				}
				else
				{
					int specularLength = 0;
					out.write(reinterpret_cast<char*>(&specularLength), 4);
				}
				out.write(reinterpret_cast<char*>(&channel), 4);

				if ((*material)->mEmissiveTexture)
				{
					int emissiveLength = strlen((*material)->mEmissiveTexture->mName.c_str()) + 1;
					out.write(reinterpret_cast<char*>(&emissiveLength), 4);
					out.write((*material)->mEmissiveTexture->mName.c_str(), emissiveLength);
				}
				else
				{
					int emissiveLength = 0;
					out.write(reinterpret_cast<char*>(&emissiveLength), 4);
				}
				out.write(reinterpret_cast<char*>(&channel), 4);

				if ((*material)->mBumpTexture)
				{
					int bumpLength = strlen((*material)->mBumpTexture->mName.c_str()) + 1;
					out.write(reinterpret_cast<char*>(&bumpLength), 4);
					out.write((*material)->mBumpTexture->mName.c_str(), bumpLength);
				}
				else
				{
					int bumpLength = 0;
					out.write(reinterpret_cast<char*>(&bumpLength), 4);
				}
				out.write(reinterpret_cast<char*>(&channel), 4);

				if ((*material)->mDepthTexture)
				{
					int depthLength = strlen((*material)->mDepthTexture->mName.c_str()) + 1;
					out.write(reinterpret_cast<char*>(&depthLength), 4);
					out.write((*material)->mDepthTexture->mName.c_str(), depthLength);
				}
				else
				{
					int depthLength = 0;
					out.write(reinterpret_cast<char*>(&depthLength), 4);
				}
				out.write(reinterpret_cast<char*>(&channel), 4);

				if ((*material)->mAmbientOcclusionTexture)
				{
					int depthLength = strlen((*material)->mAmbientOcclusionTexture->mName.c_str()) + 1;
					out.write(reinterpret_cast<char*>(&depthLength), 4);
					out.write((*material)->mAmbientOcclusionTexture->mName.c_str(), depthLength);
				}
				else
				{
					int depthLength = 0;
					out.write(reinterpret_cast<char*>(&depthLength), 4);
				}
				out.write(reinterpret_cast<char*>(&channel), 4);

				if ((*material)->mDisplacementTexture)
				{
					int depthLength = strlen((*material)->mDisplacementTexture->mName.c_str()) + 1;
					out.write(reinterpret_cast<char*>(&depthLength), 4);
					out.write((*material)->mDisplacementTexture->mName.c_str(), depthLength);
				}
				else
				{
					int depthLength = 0;
					out.write(reinterpret_cast<char*>(&depthLength), 4);
				}
				out.write(reinterpret_cast<char*>(&channel), 4);

				if ((*material)->mStepConeTexture)
				{
					int depthLength = strlen((*material)->mStepConeTexture->mName.c_str()) + 1;
					out.write(reinterpret_cast<char*>(&depthLength), 4);
					out.write((*material)->mStepConeTexture->mName.c_str(), depthLength);
				}
				else
				{
					int depthLength = 0;
					out.write(reinterpret_cast<char*>(&depthLength), 4);
				}
				out.write(reinterpret_cast<char*>(&channel), 4);

				if ((*material)->mEnvironmentTexture)
				{
					int depthLength = strlen((*material)->mEnvironmentTexture->mName.c_str()) + 1;
					out.write(reinterpret_cast<char*>(&depthLength), 4);
					out.write((*material)->mEnvironmentTexture->mName.c_str(), depthLength);
				}
				else
				{
					int depthLength = 0;
					out.write(reinterpret_cast<char*>(&depthLength), 4);
				}
				out.write(reinterpret_cast<char*>(&channel), 4);

				if ((*material)->mReflectiveTexture)
				{
					int depthLength = strlen((*material)->mReflectiveTexture->mName.c_str()) + 1;
					out.write(reinterpret_cast<char*>(&depthLength), 4);
					out.write((*material)->mReflectiveTexture->mName.c_str(), depthLength);
				}
				else
				{
					int depthLength = 0;
					out.write(reinterpret_cast<char*>(&depthLength), 4);
				}
				out.write(reinterpret_cast<char*>(&channel), 4);

				if ((*material)->mReflectionTexture)
				{
					int depthLength = strlen((*material)->mReflectionTexture->mName.c_str()) + 1;
					out.write(reinterpret_cast<char*>(&depthLength), 4);
					out.write((*material)->mReflectionTexture->mName.c_str(), depthLength);
				}
				else
				{
					int depthLength = 0;
					out.write(reinterpret_cast<char*>(&depthLength), 4);
				}
				out.write(reinterpret_cast<char*>(&channel), 4);

				if ((*material)->mRefractionTexture)
				{
					int depthLength = strlen((*material)->mRefractionTexture->mName.c_str()) + 1;
					out.write(reinterpret_cast<char*>(&depthLength), 4);
					out.write((*material)->mRefractionTexture->mName.c_str(), depthLength);
				}
				else
				{
					int depthLength = 0;
					out.write(reinterpret_cast<char*>(&depthLength), 4);
				}
				out.write(reinterpret_cast<char*>(&channel), 4);

				if ((*material)->mToonTexture)
				{
					int depthLength = strlen((*material)->mToonTexture->mName.c_str()) + 1;
					out.write(reinterpret_cast<char*>(&depthLength), 4);
					out.write((*material)->mToonTexture->mName.c_str(), depthLength);
				}
				else
				{
					int depthLength = 0;
					out.write(reinterpret_cast<char*>(&depthLength), 4);
				}
				out.write(reinterpret_cast<char*>(&channel), 4);

				out.write(reinterpret_cast<char*>(&(*material)->Contour), 4);
				out.write(reinterpret_cast<char*>(glm::value_ptr((*material)->ContourColor)), 16);
				out.write(reinterpret_cast<char*>(&(*material)->ContourThickness), 4);

				int endMaterial = 0;
				out.write(reinterpret_cast<char*>(&endMaterial), 4);
			}

			int endMaterials = 0;
			out.write(reinterpret_cast<char*>(&endMaterials), 4);
		}

		MeshMaterial* MeshMaterials::operator[](int index)
		{
			if (index < 0 || index >= int(mMaterials.size()))
			{
				return 0;
			}

			return mMaterials[index];
		}

		MeshMaterial* MeshMaterials::Find(std::string name, bool create)
		{
			for (std::vector<MeshMaterial*>::iterator material = mMaterials.begin(); material != mMaterials.end(); ++material)
			{
				if (!_stricmp((*material)->mName.c_str(), name.c_str()))
				{
					return *material;
				}
			}

			if (create)
			{
				return New(name);
			}

			return 0;
		}

		MeshMaterial* MeshMaterials::operator()(void)
		{
			if (mMaterial == mMaterials.end())
			{
				return 0;
			}

			return *mMaterial;
		}

		MeshMaterial* MeshMaterials::First(void)
		{
			if (mMaterials.empty())
			{
				mMaterial = mMaterials.end();
			}
			else
			{
				mMaterial = mMaterials.begin();
			}

			return operator()();
		}

		MeshMaterial* MeshMaterials::Next(void)
		{
			if (mMaterial != mMaterials.end())
			{
				++mMaterial;
			}

			return operator()();
		}

		MeshMaterial* MeshMaterials::Last(void)
		{
			if (mMaterials.empty())
			{
				mMaterial = mMaterials.end();
			}
			else
			{
				mMaterial = mMaterials.end() - 1;
			}

			return operator()();
		}

		MeshMaterial* MeshMaterials::Previous(void)
		{
			if (mMaterial == mMaterials.begin())
			{
				mMaterial = mMaterials.end();
			}
			else if (mMaterial != mMaterials.end())
			{
				--mMaterial;
			}

			return operator()();
		}

		void MeshMaterials::Sort(void)
		{
			sort(mMaterials.begin(), mMaterials.end(), SortMaterialsIncreasingRespectName);

			int index = 0;
			for (std::vector<MeshMaterial*>::iterator material = mMaterials.begin(); material != mMaterials.end(); ++material)
			{
				(*material)->mIndex = index++;
			}
		}

		void MeshMaterials::Reverse(void)
		{
			reverse(mMaterials.begin(), mMaterials.end());

			int index = 0;
			for (std::vector<MeshMaterial*>::iterator material = mMaterials.begin(); material != mMaterials.end(); ++material)
			{
				(*material)->mIndex = index++;
			}
		}
	}
}
