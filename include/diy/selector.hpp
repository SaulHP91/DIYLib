#ifndef SELECTOR_HPP
#define SELECTOR_HPP

#include <glm/glm.hpp>
#include <vector>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{

	class Camera;
	class Selectable;

	class Selector
	{
	public:
		DIYLIB_API Selector(void);
		DIYLIB_API ~Selector(void);

		DIYLIB_API void SetEnabled(bool enabled);
		DIYLIB_API bool GetEnabled(void);

		DIYLIB_API void SetCamera(Camera* camera);
		DIYLIB_API Camera* GetCamera(void);

		DIYLIB_API void Click(void);
		DIYLIB_API void DoubleClick(void);
		DIYLIB_API void MouseDown(glm::vec2 mousePosition);
		DIYLIB_API void MouseMove(glm::vec2 mousePosition);
		DIYLIB_API void MouseUp(glm::vec2 mousePosition);

		template<class T>T* New(void)
		{
			mSelectables.push_back(new T());
			mSelectables.back()->mSelector = this;
			mSelectables.back()->mCreator = this;

			return dynamic_cast<T*>(mSelectables.back());
		}
		DIYLIB_API void Add(Selectable* selectable);
		DIYLIB_API void Delete(Selectable* selectable);
		DIYLIB_API void Clear(void);

		DIYLIB_API void SetSelected(Selectable* selectable);
		DIYLIB_API Selectable* GetSelected(void);

	private:
		bool mEnabled;

		Camera* mCamera;

		std::vector<Selectable*> mSelectables;
		Selectable* mSelected;
		Selectable* mOldSelected;

		bool mMouseDown;
		bool mDrag;

		glm::vec2 mOldMousePosition;
	};

}

#endif