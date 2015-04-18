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

		DIYLIB_API void SetCamera(Camera* camera);
		DIYLIB_API Camera* GetCamera(void);

		DIYLIB_API void Click(void);
		DIYLIB_API void DoubleClick(void);
		DIYLIB_API void MouseDown(glm::vec2 mousePos);
		DIYLIB_API void MouseMove(glm::vec2 mousePos);
		DIYLIB_API void MouseUp(glm::vec2 mousePos);

		template<class T>T* NewSelectable(void)
		{
			mSelectables.push_back(new T());
			mSelectables.back()->mSelector = this;

			return dynamic_cast<T*>(mSelectables.back());
		}
		DIYLIB_API void AddSelectable(Selectable* selectable);
		DIYLIB_API void DeleteSelectable(Selectable* selectable);
		DIYLIB_API void ClearSelectables(void);

		DIYLIB_API Selectable* GetSelected(void);

	private:
		Camera* mCamera;

		std::vector<Selectable*> mSelectables;
		Selectable* mSelected;
		Selectable* mOldSelected;
	};

}

#endif
