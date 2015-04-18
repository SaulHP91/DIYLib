#ifndef SELECTABLEMESH_HPP
#define SELECTABLEMESH_HPP

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

#include <diy/selectable.hpp>

namespace diy
{

	class SelectableMesh : public Selectable
	{
	public:
		DIYLIB_API SelectableMesh(void);
		DIYLIB_API ~SelectableMesh(void);

		DIYLIB_API void SetVertexCount(int vertexCount);
		DIYLIB_API int GetVertexCount(void);

		DIYLIB_API void SetVerticesPointer(const float* verticesPointer);
		DIYLIB_API const float* GetVerticesPointer(void);

		DIYLIB_API void SetFaceCount(int faceCount);
		DIYLIB_API int GetFaceCount(void);

		DIYLIB_API void SetFacesPointer(const int* facesPointer);
		DIYLIB_API const int* GetFacesPointer(void);

		DIYLIB_API bool Pick(glm::vec3 rayOrigin, glm::vec3 rayDirection);

		DIYLIB_API int GetSelectedFaceIndex(void);

		DIYLIB_API void SetMatrix(glm::mat4 matrix);
		DIYLIB_API glm::mat4 GetMatrix(void);
		DIYLIB_API glm::mat4 GetInverseMatrix(void);

	private:
		int mVertexCount;
		const float* mVerticesPointer;

		int mFaceCount;
		const int* mFacesPointer;

		int mSelectedFaceIndex;

		bool mInitializedBoundingSphere;
		glm::vec3 mBoundingSphereCenter;
		float mBoundingSphereRadius;
		void InitializeBoundingSphere(void);

		glm::mat4 mMatrix;
		glm::mat4 mInverseMatrix;
	};

}

#endif
