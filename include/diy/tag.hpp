#ifndef TAG_HPP
#define TAG_HPP

#include <string>

#include <glm/glm.hpp>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	class Camera;
	class Font;
	class TagCloud;

	class Tag
	{
	public:
		DIYLIB_API Tag(const char* text = "NewTag");
		DIYLIB_API ~Tag(void);

		DIYLIB_API void SetText(const char* text);
		DIYLIB_API const char* GetText(void);

		DIYLIB_API void SetCenter(glm::vec3 center);
		DIYLIB_API glm::vec3 GetCenter(void);

		DIYLIB_API void SetTextColor(glm::vec3 textColor);
		DIYLIB_API glm::vec3 GetTextColor(void);

		DIYLIB_API void SetLineColor(glm::vec3 lineColor);
		DIYLIB_API glm::vec3 GetLineColor(void);

		DIYLIB_API void SetData(void* data);
		DIYLIB_API void* GetData(void);

		DIYLIB_API void Render(Camera* camera, Font* font, glm::vec3 center = glm::vec3(0.0f));

		DIYLIB_API void Clear(void);

	private:
		friend TagCloud;

		TagCloud* mTagCloud;
		char* mText;
		glm::vec3 mCenter;
		glm::vec3 mTextColor;
		glm::vec3 mLineColor;
		void* mData;
	};

}

#endif
