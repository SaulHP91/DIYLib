#ifndef XANIMATION_HPP
#define XANIMATION_HPP

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace x
	{
		class XAnimation
		{
		public:
			DIYLIB_API XAnimation(void);
			DIYLIB_API ~XAnimation(void);
		};
	}
}

#endif

 