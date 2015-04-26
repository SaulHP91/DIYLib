#ifndef MQOOBJECT_HPP
#define MQOOBJECT_HPP

#include <diy/mqo/mqoString.hpp>
#include <diy/mqo/mqoVector.hpp>

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
		class MqoFace;
		class MqoObject
		{
		public:
			DIYLIB_API MqoObject(void);
			DIYLIB_API ~MqoObject(void);

			MqoString Name;
			int Depth;
			int Folding;
			MqoVector Scale;
			MqoVector Rotation;
			MqoVector Translation;
			int Patch;
			int Segment;
			int Visible;
			int Locking;
			int Shading;
			float Facet;
			MqoVector Color;
			int ColorType;
			int Mirror;
			int MirrorAxis;
			float MirrorDistance;
			std::vector<MqoVector> Vertices;
			std::vector<MqoFace> Faces;

			DIYLIB_API bool ParseFromFStream(std::ifstream& in);
			DIYLIB_API glm::mat4 GetMatrix(void);
		};
	}
}

#endif
 