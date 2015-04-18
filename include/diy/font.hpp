#ifndef FONT_HPP
#define FONT_HPP

#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <map>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{

	class Camera;
	class Shader;
	class MatrixStack;

	class Font
	{
	public:
		DIYLIB_API Font(void);
		DIYLIB_API ~Font(void);

		DIYLIB_API void LoadFromFile(std::string file);

		DIYLIB_API void RenderText(Camera* camera, glm::vec2 position, const char* format, ...);
		DIYLIB_API void RenderText(Camera* camera, glm::vec3 position, const char* format, ...);
		DIYLIB_API glm::vec4 GetTextLimits(const char* format, ...);

		DIYLIB_API void Clear(void);

		DIYLIB_API void SetSize(float size);
		DIYLIB_API float GetSize(void);

		DIYLIB_API void SetColor(glm::vec3 color);
		DIYLIB_API void SetColor(glm::vec4 color);
		DIYLIB_API glm::vec4 GetColor(void);

		DIYLIB_API void SetCursor(glm::vec2 cursor);
		DIYLIB_API glm::vec2 GetCursor(void);

	private:
		struct Info
		{
			std::string face;
			int size;
		};
		Info mInfo;

		struct Common
		{
			int lineHeight;
			int base;
			int scaleW, scaleH;
		};
		Common mCommon;

		struct Page
		{
			int id;
			std::string file;
			unsigned int handle;
		};
		std::vector<Page> mPages;

		struct Char
		{
			int x, y;
			int width, height;
			int xoffset, yoffset;
			int xadvance;
			int page;
			int index;
			glm::vec2 size;
			glm::vec2 offset;
			float advance;
		};
		std::map<int, Char> mChars;

		float mSize;

		glm::vec4 mColor;

		glm::vec2 mCursor, mCurrentCursor;

		float mLineHeight;
		float mSpaceAdvance;
		int mSpaceAdvancesPerTab;

		unsigned int mPositionTexCoordBuffer;
		unsigned int mFaceBuffer;

		bool mInitialized;
		void Initialize(void);

		bool mInitializedShader;
		void InitializeShader(void);

		Shader* mShader;

		MatrixStack* mMatrixStack;
	};

}

#endif
