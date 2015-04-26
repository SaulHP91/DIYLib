#include <diy/diy.hpp>

#include <glm/own.hpp>

namespace diy
{
	namespace pose
	{
		PoseNode::PoseNode(class PoseNodes* nodes, std::string name) :
			mNodes(nodes),
			mName(name),
			mTranslation(0.0f),
			mRotation(1.0f, 0.0f, 0.0f, 0.0f),
			mScaling(1.0f),
			mMatrix(1.0f)
		{
			;
		}

		PoseNode::~PoseNode(void)
		{
			;
		}

		PoseNodes* PoseNode::Nodes(void)
		{
			return mNodes;
		}

		std::string PoseNode::Name(void)
		{
			return mName;
		}

		void PoseNode::SetIdentity(void)
		{
			mTranslation = glm::vec3(0.0f);
			mRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
			mScaling = glm::vec3(1.0f);

			mMatrix = glm::mat4(1.0f);
		}

		glm::mat4 PoseNode::GetMatrix(void)
		{
			return mMatrix;
		}

		void PoseNode::SetMatrix(glm::mat4 matrix)
		{
			mScaling = glm::vec3(glm::length(glm::vec3(matrix[0])), glm::length(glm::vec3(matrix[1])), glm::length(glm::vec3(matrix[2])));
			mRotation = glm::toQuat(glm::mat3(glm::normalize(glm::vec3(matrix[0])), glm::normalize(glm::vec3(matrix[1])), glm::normalize(glm::vec3(matrix[2]))));
			mTranslation = glm::vec3(matrix[3]);

			Update();
		}

		glm::vec3 PoseNode::GetTranslation(void)
		{
			return mTranslation;
		}

		void PoseNode::SetTranslation(glm::vec3 translation)
		{
			mTranslation = translation;

			Update();
		}

		glm::quat PoseNode::GetRotation(void)
		{
			return mRotation;
		}

		void PoseNode::SetRotation(glm::quat rotation)
		{
			mRotation = rotation;

			Update();
		}

		glm::vec3 PoseNode::GetEulerAngles(void)
		{
			return glm::matrixToRotation(mMatrix);
		}

		void PoseNode::SetEulerAngles(glm::vec3 eulerAngles)
		{
			mRotation = glm::quat(glm::radians(eulerAngles));

			Update();
		}

		float PoseNode::GetAngle(void)
		{
			return glm::angle(mRotation);
		}

		glm::vec3 PoseNode::GetAxis(void)
		{
			return glm::axis(mRotation);
		}

		void PoseNode::SetAngleAxis(float angle, glm::vec3 axis)
		{
			mRotation = glm::angleAxis(angle, axis);

			Update();
		}

		glm::vec3 PoseNode::GetScaling(void)
		{
			return mScaling;
		}

		void PoseNode::SetScaling(glm::vec3 scaling)
		{
			mScaling = scaling;

			Update();
		}

		void PoseNode::Update(void)
		{
			mMatrix = glm::translate(mTranslation) * glm::toMat4(mRotation) * glm::scale(mScaling);
		}
	}
}
