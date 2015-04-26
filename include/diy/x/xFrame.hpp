#ifndef XFRAME_HPP
#define XFRAME_HPP

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace x
	{
		class XFrame
		{
		public:
			XFrame(void);
			~XFrame(void);
		};
	}
}

#endif
