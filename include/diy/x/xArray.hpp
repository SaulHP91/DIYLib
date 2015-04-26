#ifndef XARRAY_HPP
#define XARRAY_HPP

#include <vector>
#include <fstream>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace x
	{
		template<class T> class XArray
		{
		public:
			XArray(void)
			{
				;
			}

			~XArray(void)
			{
				Clear();
			}

			std::vector<T> Values;

			void Clear(void)
			{
				Values.clear();
				Values.swap(std::vector<T>(Values));
			}

			bool ParseFromFStream(std::ifstream& in, int n)
			{
				std::streampos pos;
				char separator;

				if (!in.good())
				{
					return false;
				}

				in.clear();
				pos = in.tellg();

				Clear();
				Values.reserve(n);

				for (int i = 0; i < n - 1; ++i)
				{
					Values.push_back(T());
					if (!Values.back().ParseFromFStream(in))
					{
						in.clear();
						in.seekg(pos);
						return false;
					}

					in >> separator;
					if (!in.good() || !(separator == ',' || separator == ';'))
					{
						in.clear();
						in.seekg(pos);
						return false;
					}
				}

				Values.push_back(T());
				if (!Values.back().ParseFromFStream(in))
				{
					in.clear();
					in.seekg(pos);
					return false;
				}

				return true;
			}
		};
	}
}

#endif