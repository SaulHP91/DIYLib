#ifndef MESHTEXTURES_HPP
#define MESHTEXTURES_HPP

#include <diy/diyClasses.hpp>
#include <diy/mesh/meshDefines.hpp>

#include <string>
#include <fstream>
#include <vector>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace mesh
	{
		class MeshTextures
		{
		public:
			DIYLIB_API ~MeshTextures(void);

			DIYLIB_API MeshFile* GetFile(void);

			DIYLIB_API int GetCount(void);

			DIYLIB_API MeshTexture* New(std::string name);
			DIYLIB_API bool Delete(MeshTexture* texture);
			DIYLIB_API void DeleteUnused(void);

			DIYLIB_API void Clear(void);
			DIYLIB_API bool LoadFromFStream(std::ifstream& in);
			DIYLIB_API void SaveToFStream(std::ofstream& out);

			DIYLIB_API MeshTexture* operator[](int index);
			DIYLIB_API MeshTexture* Find(std::string name, bool create = false);

			DIYLIB_API MeshTexture* operator()(void);
			DIYLIB_API MeshTexture* First(void);
			DIYLIB_API MeshTexture* Next(void);
			DIYLIB_API MeshTexture* Last(void);
			DIYLIB_API MeshTexture* Previous(void);

			DIYLIB_API MeshTexture* SeparateAlpha(MeshTexture* texture);
			DIYLIB_API bool MergeAlpha(MeshTexture* destiny, MeshTexture* source);

			DIYLIB_API void Sort(void);
			DIYLIB_API void Reverse(void);

		private:
			MESH_FRIENDS

			MeshTextures(MeshFile* file);

			MeshFile* mFile;

			std::vector<MeshTexture*> mTextures;
			std::vector<MeshTexture*>::iterator mTexture;
		};
	}
}

#endif
