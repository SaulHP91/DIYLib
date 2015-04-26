#include <diy/diy.hpp>

#include <glm/ext.hpp>

namespace diy
{
	namespace mesh
	{
		MeshVertex::MeshVertex(void) :
			Position(0.0f),
			TexCoord(0.0f),
			Tangent(0.0f),
			Bitangent(0.0f),
			Normal(0.0f)
		{
			;
		}

		MeshVertex::MeshVertex(glm::vec3 position) :
			Position(position),
			TexCoord(0.0f),
			Tangent(0.0f),
			Bitangent(0.0f),
			Normal(0.0f)
		{
			;
		}

		MeshVertex::MeshVertex(glm::vec3 position, glm::vec2 texCoord) :
			Position(position),
			TexCoord(texCoord),
			Tangent(0.0f),
			Bitangent(0.0f),
			Normal(0.0f)
		{
			;
		}

		MeshVertex::MeshVertex(glm::vec3 position, glm::vec3 normal) :
			Position(position),
			TexCoord(0.0f),
			Tangent(0.0f),
			Bitangent(0.0f),
			Normal(normal)
		{
			Normalize();
		}

		MeshVertex::MeshVertex(glm::vec3 position, glm::vec2 texCoord, glm::vec3 normal) :
			Position(position),
			TexCoord(texCoord),
			Tangent(0.0f),
			Bitangent(0.0f),
			Normal(normal)
		{
			Normalize();
		}

		MeshVertex::MeshVertex(glm::vec3 position, glm::vec2 texCoord, glm::vec3 tangent, glm::vec3 bitangent, glm::vec3 normal) :
			Position(position),
			TexCoord(texCoord),
			Tangent(tangent),
			Bitangent(bitangent),
			Normal(normal)
		{
		}

		MeshVertex::~MeshVertex(void)
		{
			;
		}

		void MeshVertex::Normalize(void)
		{
			float tangentLength = glm::length(Tangent);
			if (tangentLength > 0.0f)
			{
				Tangent /= tangentLength;
			}

			float bitangentLength = glm::length(Bitangent);
			if (bitangentLength > 0.0f)
			{
				Bitangent /= bitangentLength;
			}

			float normalLength = glm::length(Normal);
			if (normalLength > 0.0f)
			{
				Normal /= normalLength;
			}
		}

		bool MeshVertex::LoadFromFStream(std::ifstream& in)
		{
			in.read(reinterpret_cast<char*>(glm::value_ptr(Position)), 12);
			in.read(reinterpret_cast<char*>(glm::value_ptr(TexCoord)), 8);
			in.read(reinterpret_cast<char*>(glm::value_ptr(Tangent)), 12);
			in.read(reinterpret_cast<char*>(glm::value_ptr(Bitangent)), 12);
			in.read(reinterpret_cast<char*>(glm::value_ptr(Normal)), 12);

			return true;
		}

		void MeshVertex::SaveToFStream(std::ofstream& out)
		{
			out.write(reinterpret_cast<char*>(glm::value_ptr(Position)), 12);
			out.write(reinterpret_cast<char*>(glm::value_ptr(TexCoord)), 8);
			out.write(reinterpret_cast<char*>(glm::value_ptr(Tangent)), 12);
			out.write(reinterpret_cast<char*>(glm::value_ptr(Bitangent)), 12);
			out.write(reinterpret_cast<char*>(glm::value_ptr(Normal)), 12);
		}

		MeshVertex MeshVertex::operator+(MeshVertex vertex)
		{
			MeshVertex result;

			result.Position = Position + vertex.Position;
			result.TexCoord = TexCoord + vertex.TexCoord;
			result.Tangent = Tangent + vertex.Tangent;
			result.Bitangent = Bitangent + vertex.Bitangent;
			result.Normal = Normal + vertex.Normal;

			return result;
		}

		MeshVertex MeshVertex::operator-(MeshVertex vertex)
		{
			MeshVertex result;

			result.Position = Position - vertex.Position;
			result.TexCoord = TexCoord - vertex.TexCoord;
			result.Tangent = Tangent - vertex.Tangent;
			result.Bitangent = Bitangent - vertex.Bitangent;
			result.Normal = Normal - vertex.Normal;

			return result;
		}

		MeshVertex MeshVertex::operator+=(MeshVertex vertex)
		{
			Position += vertex.Position;
			TexCoord += vertex.TexCoord;
			Tangent += vertex.Tangent;
			Bitangent += vertex.Bitangent;
			Normal += vertex.Normal;

			return *this;
		}

		MeshVertex MeshVertex::operator-=(MeshVertex vertex)
		{
			Position -= vertex.Position;
			TexCoord -= vertex.TexCoord;
			Tangent -= vertex.Tangent;
			Bitangent -= vertex.Bitangent;
			Normal -= vertex.Normal;

			return *this;
		}

		bool operator==(glm::vec3& a, glm::vec3& b)
		{
			return glm::length(a - b) < 1.0e-4f;
		}

		bool operator<(glm::vec3& a, glm::vec3& b)
		{
			if (a.x == b.x)
			{
				if (a.y == b.y)
				{
					return a.z < b.z;
				}
				return a.y < b.y;
			}
			return a.x < b.x;
		}

		bool operator==(glm::vec2& a, glm::vec2& b)
		{
			return glm::length(a - b) < 1.0e-6f;
		}

		bool operator<(glm::vec2& a, glm::vec2& b)
		{
			if (a.x == b.x)
			{
				return a.y < b.y;
			}
			return a.x < b.x;
		}

		bool operator==(MeshVertex vertexA, MeshVertex vertexB)
		{
			return vertexA.Position == vertexB.Position && vertexA.TexCoord == vertexB.TexCoord && vertexA.Normal == vertexB.Normal;
		}

		bool operator<(MeshVertex vertexA, MeshVertex vertexB)
		{
			if (vertexA.Position == vertexB.Position)
			{
				if (vertexA.TexCoord == vertexB.TexCoord)
				{
					return vertexA.Normal < vertexB.Normal;
				}
				return vertexA.TexCoord < vertexB.TexCoord;
			}
			return vertexA.Position < vertexB.Position;
		}

		bool operator!=(MeshVertex vertexA, MeshVertex vertexB)
		{
			return !(vertexA == vertexB);
		}
	}
}
