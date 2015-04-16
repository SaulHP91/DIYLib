#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	class Shader;

	class Camera
	{
	public:
		DIYLIB_API Camera(void);
		DIYLIB_API ~Camera(void);

		DIYLIB_API void SetEnabled(bool enabled);
		DIYLIB_API bool GetEnabled(void);

		DIYLIB_API void SetVisible(bool visible);
		DIYLIB_API bool GetVisible(void);

		DIYLIB_API void SetViewport(glm::vec4 viewport);
		DIYLIB_API glm::vec4 GetViewport(void);

		DIYLIB_API void SetFovy(float fovy);
		DIYLIB_API float GetFovy(void);

		DIYLIB_API float GetAspect(void);

		DIYLIB_API void SetNear(float _near);
		DIYLIB_API float GetNear(void);

		DIYLIB_API void SetFar(float _far);
		DIYLIB_API float GetFar(void);

		DIYLIB_API void SetPivot(glm::vec3 pivot);
		DIYLIB_API glm::vec3 GetPivot(void);

		DIYLIB_API void SetOrientation(glm::vec3 orientation);
		DIYLIB_API glm::vec3 GetOrientation(void);

		DIYLIB_API void SetZoom(float zoom);
		DIYLIB_API float GetZoom(void);

		DIYLIB_API void LookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));

		enum ProjectionModel
		{
			Ortho,
			Perspective
		};

		DIYLIB_API void SetProjectionModel(ProjectionModel projectionModel);
		DIYLIB_API ProjectionModel GetProjectionModel(void);

		DIYLIB_API glm::mat4 GetProjectionMatrix(void);

		DIYLIB_API void SetParentMatrix(glm::mat4 parentMatrix);
		DIYLIB_API glm::mat4 GetParentMatrix(void);
		DIYLIB_API glm::mat4 GetInverseParentMatrix(void);

		DIYLIB_API glm::mat4 GetMatrix(void);
		DIYLIB_API glm::mat4 GetInverseMatrix(void);

		DIYLIB_API glm::mat4 GetFinalMatrix(void);
		DIYLIB_API glm::mat4 GetInverseFinalMatrix(void);

		DIYLIB_API glm::mat4 GetBillboardMatrix(void);
		DIYLIB_API glm::mat4 GetInverseBillboardMatrix(void);

		DIYLIB_API glm::mat3 GetNormalMatrix(void);
		DIYLIB_API glm::mat3 GetInverseNormalMatrix(void);

		DIYLIB_API glm::mat3 GetNormalFinalMatrix(void);
		DIYLIB_API glm::mat3 GetInverseNormalFinalMatrix(void);

		DIYLIB_API glm::mat3 GetNormalBillboardMatrix(void);
		DIYLIB_API glm::mat3 GetInverseNormalBillboardMatrix(void);

		DIYLIB_API float GetUnitaryDistance(void);
		DIYLIB_API float GetFocalDistance(void);

		DIYLIB_API void SetDistance(float distance);
		DIYLIB_API float GetDistance(void);
		DIYLIB_API float GetDistanceTo(glm::vec3 target);

		DIYLIB_API float GetScaleFactor(glm::vec3 centerOfInterest, float verticalSizeInPixels, float actualVerticalSize = 1.0f);

		DIYLIB_API glm::vec3 GetPosition(void);

		DIYLIB_API void SetPivotConstrained(bool pivotConstrained);
		DIYLIB_API bool GetPivotConstrained(void);

		DIYLIB_API void SetOrientationConstrained(bool orientationConstrained);
		DIYLIB_API bool GetOrientationConstrained(void);

		DIYLIB_API void SetZoomConstrained(bool zoomConstrained);
		DIYLIB_API bool GetZoomConstrained(void);

		DIYLIB_API void SetDistanceConstrained(bool distanceConstrained);
		DIYLIB_API bool GetDistanceConstrained(void);

		DIYLIB_API void SetConstrained(bool constrained);
		DIYLIB_API bool GetConstrained(void);

		DIYLIB_API void SetMinPivot(glm::vec3 minPivot);
		DIYLIB_API glm::vec3 GetMinPivot(void);

		DIYLIB_API void SetMaxPivot(glm::vec3 maxPivot);
		DIYLIB_API glm::vec3 GetMaxPivot(void);

		DIYLIB_API void SetMinOrientation(glm::vec3 minOrientation);
		DIYLIB_API glm::vec3 GetMinOrientation(void);

		DIYLIB_API void SetMaxOrientation(glm::vec3 maxOrientation);
		DIYLIB_API glm::vec3 GetMaxOrientation(void);

		DIYLIB_API void SetMinZoom(float minZoom);
		DIYLIB_API float GetMinZoom(void);

		DIYLIB_API void SetMaxZoom(float maxZoom);
		DIYLIB_API float GetMaxZoom(void);

		DIYLIB_API void SetMinDistance(float minDistance);
		DIYLIB_API float GetMinDistance(void);

		DIYLIB_API void SetMaxDistance(float maxDistance);
		DIYLIB_API float GetMaxDistance(void);

		DIYLIB_API void SetPivotLocked(bool pivotLocked);
		DIYLIB_API bool GetPivotLocked(void);

		DIYLIB_API void SetOrientationLocked(bool orientationLocked);
		DIYLIB_API bool GetOrientationLocked(void);

		DIYLIB_API void SetZoomLocked(bool zoomLocked);
		DIYLIB_API bool GetZoomLocked(void);

		DIYLIB_API void SetDistanceLocked(bool distanceLocked);
		DIYLIB_API bool GetDistanceLocked(void);

		DIYLIB_API void SetLocked(bool locked);
		DIYLIB_API bool GetLocked(void);

		DIYLIB_API void ApplyViewport(void);
		DIYLIB_API void ApplyProjectionMatrix(void);
		DIYLIB_API void ApplyFinalMatrix(void);

		DIYLIB_API void RenderPivot(Camera* camera = 0);
		DIYLIB_API void RenderFrustum(Camera* camera = 0);
		DIYLIB_API void RenderAxes(Camera* camera = 0);
		DIYLIB_API void Render(Camera* camera = 0);

		enum Preset
		{
			Home,
			Front,
			Back,
			SideLeft,
			SideRight,
			Top,
			Bottom
		};

		DIYLIB_API void LoadPreset(Preset);

		DIYLIB_API glm::vec3 Project(glm::vec3 obj);
		DIYLIB_API glm::vec3 UnProject(glm::vec3 win);

		DIYLIB_API void GetRay(glm::vec2 mousePos, glm::vec3& origin, glm::vec3& direction);

		class Status
		{
		public:
			Status(void);
			~Status(void);

			void Clear(void);

			enum KeyboardKey
			{
				Shift,
				Control,
				Alt
			};

			Status& operator <<(KeyboardKey key);
			Status& operator >>(KeyboardKey key);
			void SetPressed(KeyboardKey key, bool pressed);
			bool GetPressed(KeyboardKey key);

			enum MouseButton
			{
				Left,
				Right,
				Middle
			};

			Status& operator <<(MouseButton button);
			Status& operator >>(MouseButton button);
			void SetPressed(MouseButton button, bool pressed);
			bool GetPressed(MouseButton button);

		private:
			bool mKeyboardKeys[3];
			bool mMouseButtons[3];
		};

		enum Style
		{
			Illusion,
			CustomGirl,
			Maya,
			StudioMax,
			Metasequoia,
			Blender,
			MilkShape
		};

		DIYLIB_API void SetStyle(Style style);
		DIYLIB_API Style GetStyle(void);

		DIYLIB_API void MouseMove(Status status, glm::vec2 mousePos);
		DIYLIB_API void MouseWheel(Status status, int wheelDelta, glm::vec2 mousePos);

		DIYLIB_API void FitBoundingSphere(glm::vec3 center, float radius);
		DIYLIB_API void FitBoundingBox(glm::vec3 center, glm::vec3 halfExtends, glm::mat4 matrix = glm::mat4(1.0f));

	private:
		bool mEnabled, mVisible;

		glm::vec4 mViewport;
		float mFovy, mAspect, mOrthoNear, mPerspectiveNear, mFar;

		glm::vec3 mPivot, mOrientation;
		float mZoom;

		float mUnitaryDistance, mFocalDistance, mDistance;

		ProjectionModel mProjectionModel;

		glm::mat4 mProjectionMatrix, mParentMatrix, mInverseParentMatrix, mMatrix, mInverseMatrix, mFinalMatrix, mInverseFinalMatrix, mBillboardMatrix, mInverseBillboardMatrix;
		glm::mat3 mNormalParentMatrix, mInverseNormalParentMatrix, mNormalMatrix, mInverseNormalMatrix, mNormalFinalMatrix, mInverseNormalFinalMatrix, mNormalBillboardMatrix, mInverseNormalBillboardMatrix;

		glm::vec3 mPosition;

		bool mPivotConstrained, mOrientationConstrained, mZoomConstrained, mDistanceConstrained;
		bool mPivotLocked, mOrientationLocked, mZoomLocked, mDistanceLocked;

		glm::vec3 mMinPivot, mMaxPivot, mMinOrientation, mMaxOrientation;
		float mMinZoom, mMaxZoom, mMinDistance, mMaxDistance;

		void Update(void);

		glm::vec2 mMousePos, mOldMousePos;
		Style mStyle;

		void MouseMoveIllusion(Status status, glm::vec2 mouseDelta);
		void MouseMoveCustomGirl(Status status, glm::vec2 mouseDelta);
		void MouseMoveMaya(Status status, glm::vec2 mouseDelta);
		void MouseMoveStudioMax(Status status, glm::vec2 mouseDelta);
		void MouseMoveMetasequoia(Status status, glm::vec2 mouseDelta);
		void MouseMoveBlender(Status status, glm::vec2 mouseDelta);
		void MouseMoveMilkShape(Status status, glm::vec2 mouseDelta);

		static Shader mPivotShader;
		static unsigned int mPivotPositionBuffer;
		static unsigned int mPivotFaceBuffer;

		static Shader mCameraShader;
		static unsigned int mCameraPositionBuffer;
		static unsigned int mCameraNormalBuffer;
		static unsigned int mCameraFaceBuffer;

		static bool mInitialized;
		void Initialize(void);
	};

}

#endif
