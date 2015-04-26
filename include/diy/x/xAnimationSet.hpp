#ifndef XANIMATIONSET_HPP
#define XANIMATIONSET_HPP

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace x
	{
		class XAnimationSet
		{
		public:
			DIYLIB_API XAnimationSet(void);
			DIYLIB_API ~XAnimationSet(void);
		};
	}
}

#endif
 