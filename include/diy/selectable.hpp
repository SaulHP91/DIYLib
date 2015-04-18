#ifndef SELECTABLE_HPP
#define SELECTABLE_HPP

#include <glm/glm.hpp>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{

	typedef void(*SELECTABLE_ONCLICK_CALLBACK)(class Selectable* selectable);
	typedef void(*SELECTABLE_ONDOUBLECLICK_CALLBACK)(class Selectable* selectable);
	typedef void(*SELECTABLE_ONENTER_CALLBACK)(class Selectable* selectable);
	typedef void(*SELECTABLE_ONMOUSEDOWN_CALLBACK)(class Selectable* selectable, glm::vec2 position);
	typedef void(*SELECTABLE_ONMOUSEMOVE_CALLBACK)(class Selectable* selectable, glm::vec2 position);
	typedef void(*SELECTABLE_ONMOUSEUP_CALLBACK)(class Selectable* selectable, glm::vec2 position);
	typedef void(*SELECTABLE_ONEXIT_CALLBACK)(class Selectable* selectable);

	class Selector;

	class Selectable
	{
	public:
		DIYLIB_API Selectable(void);
		DIYLIB_API virtual ~Selectable(void);

		DIYLIB_API void SetEnabled(bool enabled);
		DIYLIB_API bool GetEnabled(void);

		DIYLIB_API virtual bool Pick(glm::vec3 rayOrigin, glm::vec3 rayDirection) = 0;

		DIYLIB_API glm::vec3 GetIntersection(void);

		DIYLIB_API bool GetSelected(void);

		DIYLIB_API void SetData(void* data);
		DIYLIB_API void* GetData(void);

		DIYLIB_API void SetOnClick(SELECTABLE_ONCLICK_CALLBACK on_click);
		DIYLIB_API SELECTABLE_ONCLICK_CALLBACK GetOnClick(void);

		DIYLIB_API void SetOnDoubleClick(SELECTABLE_ONDOUBLECLICK_CALLBACK on_double_click);
		DIYLIB_API SELECTABLE_ONDOUBLECLICK_CALLBACK GetOnDoubleClick(void);

		DIYLIB_API void SetOnEnter(SELECTABLE_ONENTER_CALLBACK on_enter);
		DIYLIB_API SELECTABLE_ONENTER_CALLBACK GetOnEnter(void);

		DIYLIB_API void SetOnExit(SELECTABLE_ONEXIT_CALLBACK on_exit);
		DIYLIB_API SELECTABLE_ONEXIT_CALLBACK GetOnExit(void);

		DIYLIB_API void SetOnMouseDown(SELECTABLE_ONMOUSEDOWN_CALLBACK on_mouse_down);
		DIYLIB_API SELECTABLE_ONMOUSEDOWN_CALLBACK GetOnMouseDown(void);

		DIYLIB_API void SetOnMouseMove(SELECTABLE_ONMOUSEMOVE_CALLBACK on_mouse_move);
		DIYLIB_API SELECTABLE_ONMOUSEMOVE_CALLBACK GetOnMouseMove(void);

		DIYLIB_API void SetOnMouseUp(SELECTABLE_ONMOUSEUP_CALLBACK on_mouse_up);
		DIYLIB_API SELECTABLE_ONMOUSEUP_CALLBACK GetOnMouseUp(void);

	protected:
		friend Selector;

		Selector* mSelector;

		bool mEnabled;

		glm::vec3 mIntersection;

		bool mSelected;

		void* mData;

		SELECTABLE_ONCLICK_CALLBACK mOnClick;
		SELECTABLE_ONDOUBLECLICK_CALLBACK mOnDoubleClick;
		SELECTABLE_ONENTER_CALLBACK mOnEnter;
		SELECTABLE_ONEXIT_CALLBACK mOnExit;
		SELECTABLE_ONMOUSEDOWN_CALLBACK mOnMouseDown;
		SELECTABLE_ONMOUSEMOVE_CALLBACK mOnMouseMove;
		SELECTABLE_ONMOUSEUP_CALLBACK mOnMouseUp;
	};

}

#endif