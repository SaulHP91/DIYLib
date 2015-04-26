#include <diy/diy.hpp>

#include <glm/own.hpp>

namespace diy
{
	namespace anim
	{
		AnimNode::AnimNode(class AnimNodes* nodes, std::string name) :
			mNodes(nodes),
			mName(name),
			mTranslation(0.0f),
			mRotation(1.0f, 0.0f, 0.0f, 0.0f),
			mScaling(1.0f),
			mMatrix(1.0f)
		{
			;
		}

		AnimNode::~AnimNode(void)
		{
			;
		}

		AnimNodes* AnimNode::GetNodes(void)
		{
			return mNodes;
		}

		std::string AnimNode::GetName(void)
		{
			return mName;
		}

		void AnimNode::SetIdentity(void)
		{
			mTranslation = glm::vec3(0.0f);
			mRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
			mScaling = glm::vec3(1.0f);

			mMatrix = glm::mat4(1.0f);
		}

		glm::mat4 AnimNode::GetMatrix(void)
		{
			return mMatrix;
		}

		void AnimNode::SetMatrix(glm::mat4 matrix)
		{
			mScaling = glm::vec3(glm::length(glm::vec3(matrix[0])), glm::length(glm::vec3(matrix[1])), glm::length(glm::vec3(matrix[2])));
			mRotation = glm::toQuat(glm::mat3(glm::normalize(glm::vec3(matrix[0])), glm::normalize(glm::vec3(matrix[1])), glm::normalize(glm::vec3(matrix[2]))));
			mTranslation = glm::vec3(matrix[3]);

			Update();
		}

		glm::vec3 AnimNode::GetTranslation(void)
		{
			return mTranslation;
		}

		void AnimNode::SetTranslation(glm::vec3 translation)
		{
			mTranslation = translation;

			Update();
		}

		glm::quat AnimNode::GetRotation(void)
		{
			return mRotation;
		}

		void AnimNode::SetRotation(glm::quat rotation)
		{
			mRotation = rotation;

			Update();
		}

		glm::vec3 AnimNode::GetEulerAngles(void)
		{
			return glm::matrixToRotation(mMatrix);
		}

		void AnimNode::SetEulerAngles(glm::vec3 eulerAngles)
		{
			mRotation = glm::quat(glm::radians(eulerAngles));

			Update();
		}

		float AnimNode::GetAngle(void)
		{
			return glm::angle(mRotation);
		}

		glm::vec3 AnimNode::GetAxis(void)
		{
			return glm::axis(mRotation);
		}

		void AnimNode::SetAngleAxis(float angle, glm::vec3 axis)
		{
			mRotation = glm::angleAxis(angle, axis);

			Update();
		}

		glm::vec3 AnimNode::GetScaling(void)
		{
			return mScaling;
		}

		void AnimNode::SetScaling(glm::vec3 scaling)
		{
			mScaling = scaling;

			Update();
		}

		void AnimNode::Update(void)
		{
			mMatrix = glm::translate(mTranslation) * glm::toMat4(mRotation) * glm::scale(mScaling);
		}

		void AnimNode::Interpolate(AnimNode* node, float factor)
		{
			mTranslation = glm::lerp(mTranslation, node->mTranslation, factor);

			if (glm::dot(mRotation, node->mRotation) < 0.0f)
			{
				mRotation = glm::mix(mRotation, -node->mRotation, factor);
			}
			else
			{
				mRotation = glm::mix(mRotation, node->mRotation, factor);
			}

			mScaling = glm::lerp(mScaling, node->mScaling, factor);

			Update();
		}
	}
}
