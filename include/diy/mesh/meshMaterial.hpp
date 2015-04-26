#ifndef MESHMATERIAL_HPP
#define MESHMATERIAL_HPP

#include <diy/diyClasses.hpp>
#include <diy/mesh/meshDefines.hpp>

#include <string>
#include <vector>

#include <glm/glm.hpp>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace mesh
	{
		class MeshMaterial
		{
		public:
			DIYLIB_API ~MeshMaterial(void);

			DIYLIB_API MeshMaterials* GetMaterials(void);

			DIYLIB_API int GetIndex(void);

			DIYLIB_API bool SetName(std::string name);
			DIYLIB_API std::string GetName(void);

			enum Shader
			{
				SHADER_CONSTANT,
				SHADER_LAMBERT,
				SHADER_PHONG
			};
			DIYLIB_API void SetShader(Shader shader);
			DIYLIB_API Shader GetShader(void);

			enum Type
			{
				TYPE_ONE_SIDE,
				TYPE_TWO_SIDES
			};
			DIYLIB_API void SetType(Type type);
			DIYLIB_API Type GetType(void);

			enum Mode
			{
				MODE_NORMAL,
				MODE_ADD,
				MODE_SUBTRACT,
				MODE_MULTIPLY
			};
			DIYLIB_API void SetMode(Mode mode);
			DIYLIB_API Mode GetMode(void);

			DIYLIB_API void SetAmbient(glm::vec3 ambient);
			DIYLIB_API glm::vec3 GetAmbient(void);

			DIYLIB_API void SetDiffuse(glm::vec3 diffuse);
			DIYLIB_API glm::vec3 GetDiffuse(void);

			DIYLIB_API void SetAlpha(float alpha);
			DIYLIB_API float GetAlpha(void);

			DIYLIB_API void SetSpecular(glm::vec3 specular);
			DIYLIB_API glm::vec3 GetSpecular(void);

			DIYLIB_API void SetEmissive(glm::vec3 emissive);
			DIYLIB_API glm::vec3 GetEmissive(void);

			DIYLIB_API void SetReflective(glm::vec3 emissive);
			DIYLIB_API glm::vec3 GetReflective(void);

			DIYLIB_API void SetShininess(float shininess);
			DIYLIB_API float GetShininess(void);

			DIYLIB_API void SetReflection(float reflection);
			DIYLIB_API float GetReflection(void);

			DIYLIB_API void SetRefraction(float refraction);
			DIYLIB_API float GetRefraction(void);

			DIYLIB_API void SetAmbientTexture(MeshTexture* ambient_texture);
			DIYLIB_API MeshTexture* GetAmbientTexture(void);

			DIYLIB_API void SetDiffuseTexture(MeshTexture* diffuse_texture);
			DIYLIB_API MeshTexture* GetDiffuseTexture(void);

			DIYLIB_API void SetAlphaTexture(MeshTexture* alpha_texture);
			DIYLIB_API MeshTexture* GetAlphaTexture(void);

			DIYLIB_API void SetSpecularTexture(MeshTexture* specular_texture);
			DIYLIB_API MeshTexture* GetSpecularTexture(void);

			DIYLIB_API void SetShininessTexture(MeshTexture* shininess_texture);
			DIYLIB_API MeshTexture* GetShininessTexture(void);

			DIYLIB_API void SetEmissiveTexture(MeshTexture* emissive_texture);
			DIYLIB_API MeshTexture* GetEmissiveTexture(void);

			DIYLIB_API void SetBumpTexture(MeshTexture* bump_texture);
			DIYLIB_API MeshTexture* GetBumpTexture(void);

			DIYLIB_API void SetDepthTexture(MeshTexture* depth_texture);
			DIYLIB_API MeshTexture* GetDepthTexture(void);

			DIYLIB_API void SetAmbientOcclusionTexture(MeshTexture* ambient_occlusion_texture);
			DIYLIB_API MeshTexture* GetAmbientOcclusionTexture(void);

			DIYLIB_API void SetDisplacementTexture(MeshTexture* displacement_texture);
			DIYLIB_API MeshTexture* GetDisplacementTexture(void);

			DIYLIB_API void SetStepConeTexture(MeshTexture* step_cone_texture);
			DIYLIB_API MeshTexture* GetStepConeTexture(void);

			DIYLIB_API void SetEnvironmentTexture(MeshTexture* environment_texture);
			DIYLIB_API MeshTexture* GetEnvironmentTexture(void);

			DIYLIB_API void SetReflectiveTexture(MeshTexture* reflective_texture);
			DIYLIB_API MeshTexture* GetReflectiveTexture(void);

			DIYLIB_API void SetReflectionTexture(MeshTexture* reflection_texture);
			DIYLIB_API MeshTexture* GetReflectionTexture(void);

			DIYLIB_API void SetRefractionTexture(MeshTexture* refraction_texture);
			DIYLIB_API MeshTexture* GetRefractionTexture(void);

			DIYLIB_API void SetToonTexture(MeshTexture* toon_texture);
			DIYLIB_API MeshTexture* GetToonTexture(void);

			bool Contour;
			glm::vec4 ContourColor;
			float ContourThickness;

			DIYLIB_API bool IsInUse(void);

			DIYLIB_API void Apply(void);

			DIYLIB_API bool IsOpaque(void);
			DIYLIB_API bool IsTransparent(void);

			DIYLIB_API std::vector<MeshGroup*> GetGroups(void);

		private:
			MESH_FRIENDS

			MeshMaterial(MeshMaterials* materials, int index, std::string name);

			MeshMaterials* mMaterials;

			int mIndex;
			std::string mName;
			std::string mComment;

			Shader mShader;
			Type mType;
			Mode mMode;

			glm::vec4 mAmbient;
			glm::vec4 mDiffuse;
			glm::vec4 mSpecular;
			glm::vec4 mEmissive;
			glm::vec4 mReflective;

			float mShininess;
			float mReflection;
			float mRefraction;

			MeshTexture* mAmbientTexture;
			MeshTexture* mDiffuseTexture;
			MeshTexture* mAlphaTexture;
			MeshTexture* mSpecularTexture;
			MeshTexture* mShininessTexture;
			MeshTexture* mEmissiveTexture;
			MeshTexture* mBumpTexture;
			MeshTexture* mDepthTexture;
			MeshTexture* mAmbientOcclusionTexture;
			MeshTexture* mDisplacementTexture;
			MeshTexture* mStepConeTexture;
			MeshTexture* mEnvironmentTexture;
			MeshTexture* mReflectiveTexture;
			MeshTexture* mReflectionTexture;
			MeshTexture* mRefractionTexture;
			MeshTexture* mToonTexture;

			std::vector<MeshGroup*> mGroups;

			friend bool SortMaterialsIncreasingRespectName(MeshMaterial* material_a, MeshMaterial* material_b);

			void Update(void);
		};

		bool SortMaterialsIncreasingRespectName(MeshMaterial* material_a, MeshMaterial* material_b);
	}
}

#endif
