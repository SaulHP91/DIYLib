#ifndef COLORMAP_HPP
#define COLORMAP_HPP

#include <glm/glm.hpp>
#include <string>
#include <vector>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	class Camera;
	class Font;

	class ColorMap
	{
	public:
		DIYLIB_API ColorMap(void);
		DIYLIB_API ~ColorMap(void);

		DIYLIB_API void SetMinimumValue(float minimumValue);
		DIYLIB_API float GetMinimumValue(void);

		DIYLIB_API void SetMaximumValue(float maximumValue);
		DIYLIB_API float GetMaximumValue(void);

		enum Gradient
		{
			GrayScale,
			Rainbow,
			Altitude,
			PH,
			Heat,
			ColdHot,
			IceFire,
			Zebra
		};

		DIYLIB_API void SetGradient(Gradient gradient);
		DIYLIB_API void SetGradient(std::vector<std::pair<float, glm::vec3> > gradient);
		DIYLIB_API std::vector<std::pair<float, glm::vec3> > GetGradient(void);

		DIYLIB_API glm::vec3 GetColorAt(float value);

		DIYLIB_API void SetStepCount(int stepCount);
		DIYLIB_API int GetStepCount(void);

		DIYLIB_API void SetTextColor(glm::vec3 textColor);
		DIYLIB_API glm::vec3 GetTextColor(void);

		DIYLIB_API void SetTitle(std::string title);
		DIYLIB_API std::string GetTitle(void);

		DIYLIB_API void SetUnits(std::string units);
		DIYLIB_API std::string GetUnits(void);

		DIYLIB_API void Render(Camera* camera, Font* font);

		DIYLIB_API unsigned int GetTextureHandle(void);

	private:
		float mMinimumValue, mMaximumValue;

		std::vector<std::pair<float, glm::vec3> > mGradient;

		int mStepCount;

		glm::vec3 mTextColor;

		std::string mTitle, mUnits;

		void Clear(void);

		unsigned int mTextureHandle;
		bool mDirty;
	};

}

#endif
