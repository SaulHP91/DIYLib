#ifndef SELECTABLESPHERE_HPP
#define SELECTABLESPHERE_HPP

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

#include <diy/selectable.hpp>

namespace diy
{

	class SelectableSphere : public Selectable
	{
	public:
		DIYLIB_API SelectableSphere(void);
		DIYLIB_API ~SelectableSphere(void);

		DIYLIB_API void SetRadius(float radius);
		DIYLIB_API float GetRadius(void);

		DIYLIB_API void SetCenter(glm::vec3 center);
		DIYLIB_API glm::vec3 GetCenter(void);

		DIYLIB_API bool Pick(glm::vec3 rayOrigin, glm::vec3 rayDirection);

	private:
		float mRadius;
		glm::vec3 mCenter;
	};

}

#endif
