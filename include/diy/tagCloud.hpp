#ifndef TAGCLOUD_HPP
#define TAGCLOUD_HPP

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
	class Tag;
	class Camera;
	class Font;

	class TagCloud
	{
	public:
		DIYLIB_API TagCloud(void);
		DIYLIB_API ~TagCloud(void);

		DIYLIB_API void SetCenter(glm::vec3 center);
		DIYLIB_API glm::vec3 GetCenter(void);

		DIYLIB_API Tag* New(const char* text = "NewTag");
		DIYLIB_API void Add(Tag* tag);
		DIYLIB_API void Delete(Tag* tag);
		DIYLIB_API void Clear(void);

		DIYLIB_API void Render(Camera* camera, Font* font);

	private:
		glm::vec3 mCenter;
		std::vector<Tag*> mTags;
	};
}

#endif
