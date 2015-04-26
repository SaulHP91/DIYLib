#ifndef MQOFACE_HPP
#define MQOFACE_HPP

#include <diy/mqo/mqoTexcoord.hpp>

#include <vector>
#include <fstream>

#include <glm/glm.hpp>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace mqo
	{
		class MqoFace
		{
		public:
			DIYLIB_API MqoFace(void);
			DIYLIB_API ~MqoFace(void);

			int VertexCount;
			glm::ivec4 VertexIndices;
			int MaterialIndex;
			MqoTexCoord VertexTexCoords[4];

			DIYLIB_API bool ParseFromFStream(std::ifstream& in);
		};

	}
}

#endif

