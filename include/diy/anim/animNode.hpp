#ifndef ANIMNODE_HPP
#define ANIMNODE_HPP

#include <diy/diyClasses.hpp>
#include <diy/anim/animDefines.hpp>

#include <string>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace anim
	{
		class AnimNode
		{
		public:
			DIYLIB_API ~AnimNode(void);

			DIYLIB_API AnimNodes* GetNodes(void);

			DIYLIB_API std::string GetName(void);

			DIYLIB_API void SetIdentity(void);

			DIYLIB_API glm::mat4 GetMatrix(void);
			DIYLIB_API void SetMatrix(glm::mat4 matrix);

			DIYLIB_API glm::vec3 GetTranslation(void);
			DIYLIB_API void SetTranslation(glm::vec3 translation);

			DIYLIB_API glm::quat GetRotation(void);
			DIYLIB_API void SetRotation(glm::quat rotation);
			DIYLIB_API glm::vec3 GetEulerAngles(void);
			DIYLIB_API void SetEulerAngles(glm::vec3 eulerAngles);
			DIYLIB_API float GetAngle(void);
			DIYLIB_API glm::vec3 GetAxis(void);
			DIYLIB_API void SetAngleAxis(float angle, glm::vec3 axis);

			DIYLIB_API glm::vec3 GetScaling(void);
			DIYLIB_API void SetScaling(glm::vec3 scaling);

		private:
			ANIM_FRIENDS

			AnimNode(AnimNodes* nodes, std::string name);

			AnimNodes* mNodes;

			std::string mName;
			glm::mat4 mMatrix;
			glm::vec3 mTranslation;
			glm::quat mRotation;
			glm::vec3 mScaling;

			void Update(void);

			void Interpolate(AnimNode* node, float factor);
		};
	}
}

#endif
 