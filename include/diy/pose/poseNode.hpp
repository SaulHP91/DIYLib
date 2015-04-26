#ifndef POSENODE_HPP
#define POSENODE_HPP

#include <diy/diyClasses.hpp>
#include <diy/pose/poseDefines.hpp>

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
	namespace pose
	{
		class PoseNode
		{
		public:
			DIYLIB_API ~PoseNode(void);

			DIYLIB_API PoseNodes* Nodes(void);

			DIYLIB_API std::string Name(void);

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
			POSE_FRIENDS

			PoseNode(PoseNodes* nodes, std::string name);

			PoseNodes* mNodes;

			std::string mName;
			glm::mat4 mMatrix;
			glm::vec3 mTranslation;
			glm::quat mRotation;
			glm::vec3 mScaling;

			void Update(void);
		};
	}
}

#endif

 