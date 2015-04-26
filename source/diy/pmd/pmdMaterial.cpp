#include <diy/pmd/pmd.hpp>

#include <glm/ext.hpp>

namespace diy
{
	namespace pmd
	{
		PmdMaterial::PmdMaterial(void) :
			Diffuse(1.0f),
			Alpha(1.0f),
			Shininess(0),
			Specular(0.0f),
			Ambient(0.0f),
			ToonNumber(0),
			EdgeFlag(0),
			IndexCount(0)
		{
			Texture[0] = 0;
		}

		PmdMaterial::~PmdMaterial(void)
		{
			;
		}

		void PmdMaterial::LoadFromFStream(std::ifstream& in)
		{
			in.read(reinterpret_cast<char*>(glm::value_ptr(Diffuse)), 12);
			in.read(reinterpret_cast<char*>(&Alpha), 4);
			in.read(reinterpret_cast<char*>(&Shininess), 4);
			in.read(reinterpret_cast<char*>(glm::value_ptr(Specular)), 12);
			in.read(reinterpret_cast<char*>(glm::value_ptr(Ambient)), 12);
			in.read(reinterpret_cast<char*>(&ToonNumber), 1);
			in.read(reinterpret_cast<char*>(&EdgeFlag), 1);
			in.read(reinterpret_cast<char*>(&IndexCount), 4);
			in.read(reinterpret_cast<char*>(Texture), 20);
			for (int i = 0; i < 20; ++i)
			{
				if (Texture[i] == '\0')
				{
					break;
				}
				else if (Texture[i] == '*')
				{
					Texture[i] = '\0';
					break;
				}
			}
		}
	}
}
