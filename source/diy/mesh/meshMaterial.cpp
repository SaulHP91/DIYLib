#include <diy/diy.hpp>

#include <glm/ext.hpp>

#include <gl/glew.h>

namespace diy
{
	namespace mesh
	{
		MeshMaterial::MeshMaterial(MeshMaterials* materials, int index, std::string name) :
			mMaterials(materials),
			mIndex(index),
			mName(name),
			mComment(),
			mShader(SHADER_LAMBERT),
			mType(TYPE_ONE_SIDE),
			mMode(MODE_NORMAL),
			mAmbient(glm::vec3(0.5f), 1.0f),
			mDiffuse(glm::vec3(0.5f), 1.0f),
			mSpecular(glm::vec3(0.25f), 1.0f),
			mEmissive(glm::vec3(0.125f), 1.0f),
			mReflective(1.0f),
			mShininess(100.0f),
			mReflection(0.5f),
			mRefraction(0.0f),
			mAmbientTexture(0),
			mDiffuseTexture(0),
			mAlphaTexture(0),
			mSpecularTexture(0),
			mShininessTexture(0),
			mEmissiveTexture(0),
			mBumpTexture(0),
			mDepthTexture(0),
			mAmbientOcclusionTexture(0),
			mDisplacementTexture(0),
			mStepConeTexture(0),
			mEnvironmentTexture(0),
			mReflectiveTexture(0),
			mReflectionTexture(0),
			mRefractionTexture(0),
			mToonTexture(0),
			Contour(false),
			ContourColor(glm::vec3(0.0f), 1.0f),
			ContourThickness(1.0f)
		{
			;
		}

		MeshMaterial::~MeshMaterial(void)
		{
			SetAmbientTexture(0);
			SetDiffuseTexture(0);
			SetAlphaTexture(0);
			SetSpecularTexture(0);
			SetShininessTexture(0);
			SetEmissiveTexture(0);
			SetBumpTexture(0);
			SetDepthTexture(0);
			SetAmbientOcclusionTexture(0);
			SetDisplacementTexture(0);
			SetStepConeTexture(0);
			SetEnvironmentTexture(0);
			SetReflectiveTexture(0);
			SetReflectionTexture(0);
			SetRefractionTexture(0);
			SetToonTexture(0);

			while (mGroups.size())
			{
				mGroups.back()->mMaterial = 0;
				mGroups.pop_back();
			}
			mGroups.swap(std::vector<MeshGroup*>(mGroups));
		}

		MeshMaterials* MeshMaterial::GetMaterials(void)
		{
			return mMaterials;
		}

		int MeshMaterial::GetIndex(void)
		{
			return mIndex;
		}

		bool MeshMaterial::SetName(std::string name)
		{
			if (mMaterials->Find(name))
			{
				return false;
			}

			mName = name;

			return true;
		}

		std::string MeshMaterial::GetName(void)
		{
			return mName;
		}

		void MeshMaterial::SetShader(Shader shader)
		{
			mShader = shader;

			Update();
		}

		MeshMaterial::Shader MeshMaterial::GetShader(void)
		{
			return mShader;
		}

		void MeshMaterial::SetType(MeshMaterial::Type type)
		{
			mType = type;
		}

		MeshMaterial::Type MeshMaterial::GetType(void)
		{
			return mType;
		}

		void MeshMaterial::SetMode(MeshMaterial::Mode mode)
		{
			mMode = mode;
		}

		MeshMaterial::Mode MeshMaterial::GetMode(void)
		{
			return mMode;
		}

		void MeshMaterial::SetAmbient(glm::vec3 ambient)
		{
			mAmbient = glm::vec4(ambient, 1.0f);
		}

		glm::vec3 MeshMaterial::GetAmbient(void)
		{
			return glm::vec3(mAmbient);
		}

		void MeshMaterial::SetDiffuse(glm::vec3 diffuse)
		{
			mDiffuse = glm::vec4(diffuse, mDiffuse[3]);

			Update();
		}

		glm::vec3 MeshMaterial::GetDiffuse(void)
		{
			return glm::vec3(mDiffuse);
		}

		void MeshMaterial::SetAlpha(float alpha)
		{
			mDiffuse[3] = alpha;

			Update();
		}

		float MeshMaterial::GetAlpha(void)
		{
			return mDiffuse[3];
		}

		void MeshMaterial::SetSpecular(glm::vec3 specular)
		{
			mSpecular = glm::vec4(specular, 1.0f);

			Update();
		}

		glm::vec3 MeshMaterial::GetSpecular(void)
		{
			return glm::vec3(mSpecular);
		}

		void MeshMaterial::SetEmissive(glm::vec3 emissive)
		{
			mEmissive = glm::vec4(emissive, 1.0f);

			Update();
		}

		glm::vec3 MeshMaterial::GetEmissive(void)
		{
			return glm::vec3(mEmissive);
		}

		void MeshMaterial::SetShininess(float shininess)
		{
			mShininess = shininess;

			Update();
		}

		float MeshMaterial::GetShininess(void)
		{
			return mShininess;
		}

		void MeshMaterial::SetReflection(float reflection)
		{
			mReflection = reflection;

			Update();
		}

		float MeshMaterial::GetReflection(void)
		{
			return mReflection;
		}

		void MeshMaterial::SetRefraction(float refraction)
		{
			mRefraction = refraction;

			Update();
		}

		float MeshMaterial::GetRefraction(void)
		{
			return mRefraction;
		}

		void MeshMaterial::SetAmbientTexture(class MeshTexture* ambient_texture)
		{
			if (mAmbientTexture)
			{
				mAmbientTexture->mMaterials.erase(find(mAmbientTexture->mMaterials.begin(), mAmbientTexture->mMaterials.end(), this));
				mAmbientTexture = 0;
			}

			if (ambient_texture)
			{
				mAmbientTexture = ambient_texture;
				mAmbientTexture->mMaterials.push_back(this);
			}

			Update();
		}

		MeshTexture* MeshMaterial::GetAmbientTexture(void)
		{
			return mAmbientTexture;
		}

		void MeshMaterial::SetDiffuseTexture(class MeshTexture* diffuse_texture)
		{
			if (mDiffuseTexture)
			{
				mDiffuseTexture->mMaterials.erase(find(mDiffuseTexture->mMaterials.begin(), mDiffuseTexture->mMaterials.end(), this));
				mDiffuseTexture = 0;
			}

			if (diffuse_texture)
			{
				mDiffuseTexture = diffuse_texture;
				mDiffuseTexture->mMaterials.push_back(this);
			}

			Update();
		}

		MeshTexture* MeshMaterial::GetDiffuseTexture(void)
		{
			return mDiffuseTexture;
		}

		void MeshMaterial::SetAlphaTexture(class MeshTexture* texture)
		{
			if (mAlphaTexture)
			{
				mAlphaTexture->mMaterials.erase(find(mAlphaTexture->mMaterials.begin(), mAlphaTexture->mMaterials.end(), this));
				mAlphaTexture = 0;
			}

			if (texture)
			{
				mAlphaTexture = texture;
				mAlphaTexture->mMaterials.push_back(this);
			}

			Update();
		}

		MeshTexture* MeshMaterial::GetAlphaTexture(void)
		{
			return mAlphaTexture;
		}

		void MeshMaterial::SetSpecularTexture(class MeshTexture* specular_texture)
		{
			if (mSpecularTexture)
			{
				mSpecularTexture->mMaterials.erase(find(mSpecularTexture->mMaterials.begin(), mSpecularTexture->mMaterials.end(), this));
				mSpecularTexture = 0;
			}

			if (specular_texture)
			{
				mSpecularTexture = specular_texture;
				mSpecularTexture->mMaterials.push_back(this);
			}

			Update();
		}

		MeshTexture* MeshMaterial::GetSpecularTexture(void)
		{
			return mSpecularTexture;
		}

		void MeshMaterial::SetShininessTexture(class MeshTexture* shininess_texture)
		{
			if (mShininessTexture)
			{
				mShininessTexture->mMaterials.erase(find(mShininessTexture->mMaterials.begin(), mShininessTexture->mMaterials.end(), this));
				mShininessTexture = 0;
			}

			if (shininess_texture)
			{
				mShininessTexture = shininess_texture;
				mShininessTexture->mMaterials.push_back(this);
			}

			Update();
		}

		MeshTexture* MeshMaterial::GetShininessTexture(void)
		{
			return mShininessTexture;
		}

		void MeshMaterial::SetEmissiveTexture(class MeshTexture* emissive_texture)
		{
			if (mEmissiveTexture)
			{
				mEmissiveTexture->mMaterials.erase(find(mEmissiveTexture->mMaterials.begin(), mEmissiveTexture->mMaterials.end(), this));
				mEmissiveTexture = 0;
			}

			if (emissive_texture)
			{
				mEmissiveTexture = emissive_texture;
				mEmissiveTexture->mMaterials.push_back(this);
			}

			Update();
		}

		MeshTexture* MeshMaterial::GetEmissiveTexture(void)
		{
			return mEmissiveTexture;
		}

		void MeshMaterial::SetBumpTexture(class MeshTexture* bump_texture)
		{
			if (mBumpTexture)
			{
				mBumpTexture->mMaterials.erase(find(mBumpTexture->mMaterials.begin(), mBumpTexture->mMaterials.end(), this));
				mBumpTexture = 0;
			}

			if (bump_texture)
			{
				mBumpTexture = bump_texture;
				mBumpTexture->mMaterials.push_back(this);
			}

			Update();
		}

		MeshTexture* MeshMaterial::GetBumpTexture(void)
		{
			return mBumpTexture;
		}

		void MeshMaterial::SetDepthTexture(class MeshTexture* depth_texture)
		{
			if (mDepthTexture)
			{
				mDepthTexture->mMaterials.erase(find(mDepthTexture->mMaterials.begin(), mDepthTexture->mMaterials.end(), this));
				mDepthTexture = 0;
			}

			if (depth_texture)
			{
				mDepthTexture = depth_texture;
				mDepthTexture->mMaterials.push_back(this);
			}

			Update();
		}

		MeshTexture* MeshMaterial::GetDepthTexture(void)
		{
			return mDepthTexture;
		}

		void MeshMaterial::SetAmbientOcclusionTexture(class MeshTexture* ambient_occlusion_texture)
		{
			if (mAmbientOcclusionTexture)
			{
				mAmbientOcclusionTexture->mMaterials.erase(find(mAmbientOcclusionTexture->mMaterials.begin(), mAmbientOcclusionTexture->mMaterials.end(), this));
				mAmbientOcclusionTexture = 0;
			}

			if (ambient_occlusion_texture)
			{
				mAmbientOcclusionTexture = ambient_occlusion_texture;
				mAmbientOcclusionTexture->mMaterials.push_back(this);
			}

			Update();
		}

		MeshTexture* MeshMaterial::GetAmbientOcclusionTexture(void)
		{
			return mAmbientOcclusionTexture;
		}

		void MeshMaterial::SetDisplacementTexture(class MeshTexture* displacement_texture)
		{
			if (mDisplacementTexture)
			{
				mDisplacementTexture->mMaterials.erase(find(mDisplacementTexture->mMaterials.begin(), mDisplacementTexture->mMaterials.end(), this));
				mDisplacementTexture = 0;
			}

			if (displacement_texture)
			{
				mDisplacementTexture = displacement_texture;
				mDisplacementTexture->mMaterials.push_back(this);
			}

			Update();
		}

		MeshTexture* MeshMaterial::GetDisplacementTexture(void)
		{
			return mDisplacementTexture;
		}

		void MeshMaterial::SetStepConeTexture(class MeshTexture* step_cone_texture)
		{
			if (mStepConeTexture)
			{
				mStepConeTexture->mMaterials.erase(find(mStepConeTexture->mMaterials.begin(), mStepConeTexture->mMaterials.end(), this));
				mStepConeTexture = 0;
			}

			if (step_cone_texture)
			{
				mStepConeTexture = step_cone_texture;
				mStepConeTexture->mMaterials.push_back(this);
			}

			Update();
		}

		MeshTexture* MeshMaterial::GetStepConeTexture(void)
		{
			return mStepConeTexture;
		}

		void MeshMaterial::SetEnvironmentTexture(class MeshTexture* environment_texture)
		{
			if (mEnvironmentTexture)
			{
				mEnvironmentTexture->mMaterials.erase(find(mEnvironmentTexture->mMaterials.begin(), mEnvironmentTexture->mMaterials.end(), this));
				mEnvironmentTexture = 0;
			}

			if (environment_texture)
			{
				mEnvironmentTexture = environment_texture;
				mEnvironmentTexture->mMaterials.push_back(this);
			}

			Update();
		}

		MeshTexture* MeshMaterial::GetEnvironmentTexture(void)
		{
			return mEnvironmentTexture;
		}

		void MeshMaterial::SetReflectiveTexture(class MeshTexture* reflective_texture)
		{
			if (mReflectiveTexture)
			{
				mReflectiveTexture->mMaterials.erase(find(mReflectiveTexture->mMaterials.begin(), mReflectiveTexture->mMaterials.end(), this));
				mReflectiveTexture = 0;
			}

			if (reflective_texture)
			{
				mReflectiveTexture = reflective_texture;
				mReflectiveTexture->mMaterials.push_back(this);
			}

			Update();
		}

		MeshTexture* MeshMaterial::GetReflectiveTexture(void)
		{
			return mReflectiveTexture;
		}

		void MeshMaterial::SetReflectionTexture(class MeshTexture* reflection_texture)
		{
			if (mReflectionTexture)
			{
				mReflectionTexture->mMaterials.erase(find(mReflectionTexture->mMaterials.begin(), mReflectionTexture->mMaterials.end(), this));
				mReflectionTexture = 0;
			}

			if (reflection_texture)
			{
				mReflectionTexture = reflection_texture;
				mReflectionTexture->mMaterials.push_back(this);
			}

			Update();
		}

		MeshTexture* MeshMaterial::GetReflectionTexture(void)
		{
			return mReflectionTexture;
		}

		void MeshMaterial::SetRefractionTexture(class MeshTexture* refraction_texture)
		{
			if (mRefractionTexture)
			{
				mRefractionTexture->mMaterials.erase(find(mRefractionTexture->mMaterials.begin(), mRefractionTexture->mMaterials.end(), this));
				mRefractionTexture = 0;
			}

			if (refraction_texture)
			{
				mRefractionTexture = refraction_texture;
				mRefractionTexture->mMaterials.push_back(this);
			}

			Update();
		}

		MeshTexture* MeshMaterial::GetRefractionTexture(void)
		{
			return mRefractionTexture;
		}

		void MeshMaterial::SetToonTexture(class MeshTexture* toon_texture)
		{
			if (mToonTexture)
			{
				mToonTexture->mMaterials.erase(find(mToonTexture->mMaterials.begin(), mToonTexture->mMaterials.end(), this));
				mToonTexture = 0;
			}

			if (toon_texture)
			{
				mToonTexture = toon_texture;
				mToonTexture->mMaterials.push_back(this);
			}

			Update();
		}

		MeshTexture* MeshMaterial::GetToonTexture(void)
		{
			return mToonTexture;
		}

		bool MeshMaterial::IsInUse(void)
		{
			return mGroups.size();
		}

		void MeshMaterial::Apply(void)
		{
			if (mDiffuse[3] < 1.0f)
			{
				glEnable(GL_BLEND);
			}
			else
			{
				glDisable(GL_BLEND);
			}

			if (mDiffuseTexture)
			{
				mDiffuseTexture->Apply();
			}
			else
			{
				glDisable(GL_TEXTURE_2D);
			}

			switch (mType)
			{
			case TYPE_ONE_SIDE:
			{
				glEnable(GL_CULL_FACE);
				glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
			}
			break;
			case TYPE_TWO_SIDES:
			{
				glDisable(GL_CULL_FACE);
				glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
			}
			break;
			}

			switch (mMode)
			{
			case MODE_NORMAL:
			{
				glEnable(GL_LIGHTING);
				glEnable(GL_LIGHT0);
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glm::value_ptr(mDiffuse));
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, glm::value_ptr(mAmbient));
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(mSpecular));
				glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, glm::value_ptr(mEmissive));
				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mShininess);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glDisable(GL_POLYGON_OFFSET_FILL);
			}
			break;
			case MODE_ADD:
			{
				glDisable(GL_LIGHTING);
				glColor4fv(glm::value_ptr(mDiffuse));
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE);
				glEnable(GL_POLYGON_OFFSET_FILL);
				glPolygonOffset(-0.1f, 0.0f);
			}
			break;
			case MODE_SUBTRACT:
			{
				glDisable(GL_LIGHTING);
				glColor4fv(glm::value_ptr(mDiffuse));
				glEnable(GL_BLEND);
				glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR);
				glEnable(GL_POLYGON_OFFSET_FILL);
				glPolygonOffset(-0.1f, 0.0f);
			}
			break;
			case MODE_MULTIPLY:
			{
				glDisable(GL_LIGHTING);
				glColor4fv(glm::value_ptr(mDiffuse));
				glEnable(GL_BLEND);
				glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
				glEnable(GL_POLYGON_OFFSET_FILL);
				glPolygonOffset(-0.1f, 0.0f);
			}
			break;
			default:;
			}
		}

		bool MeshMaterial::IsOpaque(void)
		{
			return !IsTransparent();
		}

		bool MeshMaterial::IsTransparent(void)
		{
			if (mDiffuseTexture)
			{
				return mDiffuse[3] < 1.0f || mMode > MATERIAL_NORMAL || mDiffuseTexture->mDepth == 2 || mDiffuseTexture->mDepth == 4;
			}

			return mDiffuse[3] < 1.0f || mMode > MATERIAL_NORMAL;
		}

		std::vector<MeshGroup*> MeshMaterial::GetGroups(void)
		{
			return mGroups;
		}

		bool SortMaterialsIncreasingRespectName(class MeshMaterial* material_a, class MeshMaterial* material_b)
		{
			return strcmp(material_a->mName.c_str(), material_b->mName.c_str()) < 0;
		}

		void MeshMaterial::Update(void)
		{
			for (std::vector<MeshGroup*>::iterator group = mGroups.begin(); group != mGroups.end(); ++group)
			{
				(*group)->Update();
			}
		}
	}
}
