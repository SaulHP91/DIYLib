#ifndef MESHVERTEX_HPP
#define MESHVERTEX_HPP

#include <diy/diyClasses.hpp>
#include <diy/mesh/meshDefines.hpp>

#include <fstream>

#include <glm/glm.hpp>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace mesh
	{
		class MeshVertex
		{
		public:
			DIYLIB_API MeshVertex(void);
			DIYLIB_API MeshVertex(glm::vec3 position);
			DIYLIB_API MeshVertex(glm::vec3 position, glm::vec2 texCoord);
			DIYLIB_API MeshVertex(glm::vec3 position, glm::vec3 normal);
			DIYLIB_API MeshVertex(glm::vec3 position, glm::vec2 texCoord, glm::vec3 normal);
			DIYLIB_API MeshVertex(glm::vec3 position, glm::vec2 texCoord, glm::vec3 tangent, glm::vec3 bitangent, glm::vec3 normal);
			DIYLIB_API ~MeshVertex(void);

			glm::vec3 Position;
			glm::vec2 TexCoord;
			glm::vec3 Tangent;
			glm::vec3 Bitangent;
			glm::vec3 Normal;

			DIYLIB_API void Normalize(void);

			DIYLIB_API bool LoadFromFStream(std::ifstream& in);
			DIYLIB_API void SaveToFStream(std::ofstream& out);

			DIYLIB_API MeshVertex operator+(MeshVertex vertex);
			DIYLIB_API MeshVertex operator-(MeshVertex vertex);
			DIYLIB_API MeshVertex operator+=(MeshVertex vertex);
			DIYLIB_API MeshVertex operator-=(MeshVertex vertex);

		private:
			MESH_FRIENDS
		};

		bool operator==(glm::vec3& a, glm::vec3& b);
		bool operator<(glm::vec3& a, glm::vec3& b);

		bool operator==(glm::vec2& a, glm::vec2& b);
		bool operator<(glm::vec2& a, glm::vec2& b);

		bool operator==(MeshVertex vertexA, MeshVertex vertexB);
		bool operator<(MeshVertex vertexA, MeshVertex vertexB);
		bool operator!=(MeshVertex vertexA, MeshVertex vertexB);
	}
}

#endif
