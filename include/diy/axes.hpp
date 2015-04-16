#ifndef AXES_HPP
#define AXES_HPP

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{

	class Camera;

	class Axes
	{
	public:
		DIYLIB_API Axes(void);
		DIYLIB_API ~Axes(void);

		DIYLIB_API void SetVisible(bool visible);
		DIYLIB_API bool GetVisible(void);

		DIYLIB_API void SetSize(float size);
		DIYLIB_API float GetSize(void);

		DIYLIB_API void Render(Camera* camera);

	private:
		bool mVisible;

		float mSize;

		static unsigned int mAxisPositionBuffer;
		static unsigned int mAxisNormalBuffer;
		static unsigned int mAxisFaceBuffer;
		static class Shader mXAxisShader;
		static class Shader mYAxisShader;
		static class Shader mZAxisShader;

		static unsigned int mXPositionBuffer;
		static unsigned int mXFaceBuffer;
		static class Shader mXShader;

		static unsigned int mYPositionBuffer;
		static unsigned int mYFaceBuffer;
		static class Shader mYShader;

		static unsigned int mZPositionBuffer;
		static unsigned int mZFaceBuffer;
		static class Shader mZShader;

		static bool mInitialized;
		void Initialize(void);
	};

}

#endif
