#ifndef GRID_HPP
#define GRID_HPP

#include <glm/glm.hpp>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	class Camera;

	class Grid
	{
	public:
		DIYLIB_API Grid(void);
		DIYLIB_API ~Grid(void);

		DIYLIB_API void SetVisible(bool visible);
		DIYLIB_API bool GetVisible(void);

		DIYLIB_API void SetMajorStep(float majorStep);
		DIYLIB_API float GetMajorStep(void);

		DIYLIB_API void SetMinorSteps(int minorSteps);
		DIYLIB_API int GetMinorSteps(void);

		DIYLIB_API void SetShowMinor(bool showMinor);
		DIYLIB_API bool GetShowMinor(void);

		DIYLIB_API void SetSize(glm::ivec4 size);
		DIYLIB_API glm::ivec4 GetSize(void);

		DIYLIB_API void SetMajorColor(glm::vec3 majorColor);
		DIYLIB_API glm::vec3 GetMajorColor(void);

		DIYLIB_API void SetMinorColor(glm::vec3 minorColor);
		DIYLIB_API glm::vec3 GetMinorColor(void);

		DIYLIB_API void SetMajorThickness(int majorThickness);
		DIYLIB_API int GetMajorThickness(void);

		DIYLIB_API void SetMinorThickness(int minorThickness);
		DIYLIB_API int GetMinorThickness(void);

		DIYLIB_API void Render(Camera* camera, glm::mat4 localMatrix = glm::mat4(1.0f));

	private:
		bool mVisible;

		float mMajorStep;
		int mMinorSteps;
		float mMinorStep;

		bool mShowMinor;

		glm::ivec4 mSize;

		glm::vec3 mMajorColor;
		glm::vec3 mMinorColor;

		int mMajorThickness;
		int mMinorThickness;
	};

}

#endif
