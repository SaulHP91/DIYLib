#ifndef SELECTABLEPOINTCLOUD_HPP
#define SELECTABLEPOINTCLOUD_HPP

#include "diy\selectable.hpp"

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	class Camera;

	class SelectablePointCloud : public Selectable
	{
	public:
		DIYLIB_API SelectablePointCloud(void);
		DIYLIB_API ~SelectablePointCloud(void);

		DIYLIB_API void SetPointCount(int pointCount);
		DIYLIB_API int GetPointCount(void);

		DIYLIB_API void SetPointsPointer(const float* pointsPointer);
		DIYLIB_API const float* GetPointsPointer(void);

		DIYLIB_API void SetPointSize(float pointSize);
		DIYLIB_API float GetPointSize(void);

		DIYLIB_API bool Pick(glm::vec3 rayOrigin, glm::vec3 rayDirection);

		DIYLIB_API int GetSelectedPointIndex(void);

		DIYLIB_API void SetCamera(Camera* camera);
		DIYLIB_API Camera* GetCamera(void);

		DIYLIB_API void SetMatrix(glm::mat4 matrix);
		DIYLIB_API glm::mat4 GetMatrix(void);
		DIYLIB_API glm::mat4 GetInverseMatrix(void);

	private:
		int mPointCount;
		const float* mPointsPointer;
		float mPointSize;

		int mSelectedPointIndex;

		Camera* mCamera;

		bool mInitializedBoundingSphere;
		glm::vec3 mBoundingSphereCenter;
		float mBoundingSphereRadius;
		void InitializeBoundingSphere(void);

		glm::mat4 mMatrix;
		glm::mat4 mInverseMatrix;
	};

}

#endif
