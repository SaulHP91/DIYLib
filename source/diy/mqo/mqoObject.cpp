#include <diy/mqo/mqo.hpp>

#include <glm/ext.hpp>

namespace diy
{
	namespace mqo
	{
		MqoObject::MqoObject(void) :
			Depth(0),
			Folding(0),
			Scale(1.0f),
			Patch(0),
			Segment(0),
			Visible(0),
			Locking(0),
			Shading(0),
			Facet(0),
			Color(0),
			ColorType(0),
			Mirror(0),
			MirrorAxis(0),
			MirrorDistance(1.0f)
		{
			;
		}

		MqoObject::~MqoObject(void)
		{
			Vertices.clear();
			Vertices.swap(std::vector<MqoVector>(Vertices));

			Faces.clear();
			Faces.swap(std::vector<MqoFace>(Faces));
		}

		bool MqoObject::ParseFromFStream(std::ifstream& in)
		{
			static char buffer[256];

			std::streampos pos = in.tellg();

			if (!Name.ParseFromFStream(in))
			{
				in.clear();
				in.seekg(pos);
				return false;
			}

			char token;
			in >> token;
			if (token != '{' || !in.good())
			{
				in.clear();
				in.seekg(pos);
				return false;
			}

			while (true)
			{
				std::streampos alter_p = in.tellg();

				in >> token;
				if (!in.good())
				{
					in.clear();
					in.seekg(pos);
					return false;
				}
				if (token == '}')
				{
					break;
				}

				in.clear();
				in.seekg(alter_p);

				in >> buffer;
				if (!in.good())
				{
					in.clear();
					in.seekg(pos);
					return false;
				}
				if (!_stricmp(buffer, "depth"))
				{
					in >> Depth;
					if (!in.good())
					{
						in.clear();
						in.seekg(pos);
						return false;
					}
				}
				else if (!_stricmp(buffer, "folding"))
				{
					in >> Folding;
					if (!in.good())
					{
						in.clear();
						in.seekg(pos);
						return false;
					}
				}
				else if (!_stricmp(buffer, "scale"))
				{
					if (!Scale.ParseFromFStream(in))
					{
						in.clear();
						in.seekg(pos);
						return false;
					}
				}
				else if (!_stricmp(buffer, "rotation"))
				{
					if (!Rotation.ParseFromFStream(in))
					{
						in.clear();
						in.seekg(pos);
						return false;
					}
				}
				else if (!_stricmp(buffer, "translation"))
				{
					if (!Translation.ParseFromFStream(in))
					{
						in.clear();
						in.seekg(pos);
						return false;
					}
				}
				else if (!_stricmp(buffer, "patch"))
				{
					in >> Patch;
					if (!in.good())
					{
						in.clear();
						in.seekg(pos);
						return false;
					}
				}
				else if (!_stricmp(buffer, "segment"))
				{
					in >> Segment;
					if (!in.good())
					{
						in.clear();
						in.seekg(pos);
						return false;
					}
				}
				else if (!_stricmp(buffer, "visible"))
				{
					in >> Visible;
					if (!in.good())
					{
						in.clear();
						in.seekg(pos);
						return false;
					}
				}
				else if (!_stricmp(buffer, "locking"))
				{
					in >> Locking;
					if (!in.good())
					{
						in.clear();
						in.seekg(pos);
						return false;
					}
				}
				else if (!_stricmp(buffer, "shading"))
				{
					in >> Shading;
					if (!in.good())
					{
						in.clear();
						in.seekg(pos);
						return false;
					}
				}
				else if (!_stricmp(buffer, "facet"))
				{
					in >> Facet;
					if (!in.good())
					{
						in.clear();
						in.seekg(pos);
						return false;
					}
				}
				else if (!_stricmp(buffer, "color"))
				{
					if (!Color.ParseFromFStream(in))
					{
						in.clear();
						in.seekg(pos);
						return false;
					}
				}
				else if (!_stricmp(buffer, "color_type"))
				{
					in >> ColorType;
					if (!in.good())
					{
						in.clear();
						in.seekg(pos);
						return false;
					}
				}
				else if (!_stricmp(buffer, "mirror"))
				{
					in >> Mirror;
					if (!in.good())
					{
						in.clear();
						in.seekg(pos);
						return false;
					}
				}
				else if (!_stricmp(buffer, "mirror_axis"))
				{
					in >> MirrorAxis;
					if (!in.good())
					{
						in.clear();
						in.seekg(pos);
						return false;
					}
				}
				else if (!_stricmp(buffer, "mirror_dis"))
				{
					in >> MirrorDistance;
					if (!in.good())
					{
						in.clear();
						in.seekg(pos);
						return false;
					}
				}
				else if (!_stricmp(buffer, "vertex"))
				{
					int vertexCount;
					in >> vertexCount;
					if (!in.good())
					{
						in.clear();
						in.seekg(pos);
						return false;
					}

					in >> token;
					if (token != '{' || !in.good())
					{
						in.clear();
						in.seekg(pos);
						return false;
					}

					Vertices.resize(vertexCount);
					for (std::vector<MqoVector>::iterator vertex = Vertices.begin(); vertex != Vertices.end(); ++vertex)
					{
						if (!vertex->ParseFromFStream(in))
						{
							in.clear();
							in.seekg(pos);
							return false;
						}
					}

					in >> token;
					if (token != '}' || !in.good())
					{
						in.clear();
						in.seekg(pos);
						return false;
					}
				}
				else if (!_stricmp(buffer, "BVertex"))
				{
					return false;
				}
				else if (!_stricmp(buffer, "face"))
				{
					int faceCount;
					in >> faceCount;
					if (!in.good())
					{
						in.clear();
						in.seekg(pos);
						return false;
					}

					in >> token;
					if (token != '{' || !in.good())
					{
						in.clear();
						in.seekg(pos);
						return false;
					}

					Faces.resize(faceCount);
					for (std::vector<MqoFace>::iterator mqo_face = Faces.begin(); mqo_face != Faces.end(); ++mqo_face)
					{
						if (!mqo_face->ParseFromFStream(in))
						{
							in.clear();
							in.seekg(pos);
							return false;
						}
					}

					in >> token;
					if (token != '}' || !in.good())
					{
						in.clear();
						in.seekg(pos);
						return false;
					}
				}
				else
				{
					in.getline(buffer, 255);
				}
			}

			return true;
		}

		glm::mat4 MqoObject::GetMatrix(void)
		{
			return glm::translate(glm::vec3(Translation)) * glm::yawPitchRoll(Rotation.Y, Rotation.X, Rotation.Z) * glm::scale(glm::vec3(Scale));
		}
	}
}
