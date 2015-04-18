#ifndef MANIPULATOR_HPP
#define MANIPULATOR_HPP

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{

	typedef void(*MANIPULATOR_ONCHANGE_CALLBACK)(class Manipulator* manipulator, void* data);

	class Camera;
	class Selector;
	class SelectableMesh;
	class MatrixStack;
	class Shader;

	class Manipulator
	{
	public:
		DIYLIB_API Manipulator(void);
		DIYLIB_API ~Manipulator(void);

		DIYLIB_API void MouseDown(int x, int y);
		DIYLIB_API void MouseMove(int x, int y);
		DIYLIB_API void MouseUp(void);

		DIYLIB_API void SetParentMatrix(glm::mat4 parentMatrix);
		DIYLIB_API glm::mat4 GetParentMatrix(void);
		DIYLIB_API void ApplyParentMatrix(void);

		DIYLIB_API void SetMatrix(glm::mat4 matrix);
		DIYLIB_API glm::mat4 GetMatrix(void);
		DIYLIB_API void ApplyMatrix(void);

		DIYLIB_API glm::mat4 GetFinalMatrix(void);
		DIYLIB_API void ApplyFinalMatrix(void);

		DIYLIB_API void Render(void);

		DIYLIB_API void Show(void);
		DIYLIB_API void Hide(void);
		DIYLIB_API bool IsVisible(void);
		DIYLIB_API bool IsActing(void);

		DIYLIB_API void SetSize(float size);
		DIYLIB_API float GetSize(void);

		enum ManipulatorSpace
		{
			Eye, 
			World, 
			Object 
		};

		DIYLIB_API void SetSpace(ManipulatorSpace space);

		enum ManipulatorStyle
		{
			Translation,
			Rotation,
			Scaling
		};

		DIYLIB_API void SetStyle(ManipulatorStyle style);

		DIYLIB_API void SetCamera(Camera* camera);
		DIYLIB_API Camera* GetCamera(void);

		DIYLIB_API void SetTranslation(glm::vec3 translation);
		DIYLIB_API glm::vec3 GetTranslation(void);

		DIYLIB_API void SetRotation(glm::quat rotation);
		DIYLIB_API glm::quat GetRotation(void);
		DIYLIB_API void SetEulerAngles(glm::vec3 eulerAngles);
		DIYLIB_API glm::vec3 GetEulerAngles(void);
		DIYLIB_API void SetAngleAxis(float angle, glm::vec3 axis);
		DIYLIB_API float GetAngle(void);
		DIYLIB_API glm::vec3 GetAxis(void);

		DIYLIB_API void SetScaling(glm::vec3 scaling);
		DIYLIB_API glm::vec3 GetScaling(void);

		DIYLIB_API bool SnapTranslation(void);
		DIYLIB_API void SnapTranslationOn(void);
		DIYLIB_API void SnapTranslationOff(void);
		DIYLIB_API void SetTranslationStep(float translationStep);
		DIYLIB_API float GetTranslationStep(void);

		DIYLIB_API bool SnapRotation(void);
		DIYLIB_API void SnapRotationOn(void);
		DIYLIB_API void SnapRotationOff(void);
		DIYLIB_API void SetRotationStep(float rotationStep);
		DIYLIB_API float GetRotationStep(void);

		DIYLIB_API void SetData(void* data);
		DIYLIB_API void* GetData(void);

		DIYLIB_API void SetOnChange(MANIPULATOR_ONCHANGE_CALLBACK on_change);

	private:
		Camera* mCamera;

		void Update(void);

		enum SelectionName{ None, XArrow, YArrow, ZArrow, XGimbal, YGimbal, ZGimbal, XScaler, YScaler, ZScaler, XYPlane, YZPlane, ZXPlane, OriginPivot };

		SelectionName Pick(int x, int y);

		void RenderXArrow(void);
		void RenderYArrow(void);
		void RenderZArrow(void);
		void RenderXGimbal(void);
		void RenderYGimbal(void);
		void RenderZGimbal(void);
		void RenderXScaler(void);
		void RenderYScaler(void);
		void RenderZScaler(void);
		void RenderXYPlane(void);
		void RenderYZPlane(void);
		void RenderZXPlane(void);
		void RenderOriginPivot(void);

		glm::vec3 mParentTranslation;
		glm::quat mParentRotation;
		glm::vec3 mParentScaling;
		glm::mat4 mParentMatrix;
		glm::vec3 mTranslation;
		glm::quat mRotation;
		glm::vec3 mScaling;
		glm::mat4 mMatrix;
		glm::vec3 mFinalTranslation;
		glm::quat mFinalRotation;
		glm::vec3 mFinalScaling;
		glm::mat4 mFinalMatrix;

		bool mVisible;
		float mSize;
		SelectionName mFocused;
		SelectionName mSelected;
		bool mActing;

		ManipulatorSpace mSpace;
		ManipulatorStyle mStyle;

		glm::vec3 mMouseContactPoint, mMousePosition, mOldMousePosition, mMouse;
		glm::vec3 mRadius, mNewRadius;
		glm::vec3 mAxis, mAxis_, mAxis1, mAxis1_, mAxis2, mAxis2_, mDT;
		float mDAngle;
		glm::vec3 mBinormal;

		int mOldX, mOldY;
		float mDepth;

		glm::vec3 mWindowPosition;

		bool mSnapTranslation;
		float mTranslationStep;

		bool mSnapRotation;
		float mRotationStep;

		void* mData;

		MANIPULATOR_ONCHANGE_CALLBACK mOnChange;

		Selector* mSelector;
		SelectableMesh* mSelectableXArrow;
		SelectableMesh* mSelectableYArrow;
		SelectableMesh* mSelectableZArrow;
		SelectableMesh* mSelectableXGimbal;
		SelectableMesh* mSelectableYGimbal;
		SelectableMesh* mSelectableZGimbal;
		SelectableMesh* mSelectableXScaler;
		SelectableMesh* mSelectableYScaler;
		SelectableMesh* mSelectableZScaler;
		SelectableMesh* mSelectableXYPlane;
		SelectableMesh* mSelectableYZPlane;
		SelectableMesh* mSelectableZXPlane;
		SelectableMesh* mSelectableOriginPivot;

		MatrixStack* mMatrixStack;

		SelectionName mXArrowName;
		SelectionName mYArrowName;
		SelectionName mZArrowName;
		SelectionName mXGimbalName;
		SelectionName mYGimbalName;
		SelectionName mZGimbalName;
		SelectionName mXScalerName;
		SelectionName mYScalerName;
		SelectionName mZScalerName;
		SelectionName mXYPlaneName;
		SelectionName mYZPlaneName;
		SelectionName mZXPlaneName;
		SelectionName mOriginPivotName;

		static unsigned int mArrowPositionBuffer;
		static unsigned int mArrowNormalBuffer;
		static unsigned int mArrowFaceBuffer;

		static unsigned int mGimbalPositionBuffer;
		static unsigned int mGimbalNormalBuffer;
		static unsigned int mGimbalFaceBuffer;

		static unsigned int mScalerPositionBuffer;
		static unsigned int mScalerNormalBuffer;
		static unsigned int mScalerFaceBuffer;

		static unsigned int mPlanePositionBuffer;
		static unsigned int mPlaneNormalBuffer;
		static unsigned int mPlaneFaceBuffer;

		static unsigned int mPivotPositionBuffer;
		static unsigned int mPivotNormalBuffer;
		static unsigned int mPivotFaceBuffer;

		static Shader mShader;

		static bool mInitializedBuffersAndShaders;
		void InitializeBuffersAndShaders(void);
	};

}

#endif
