#ifndef MESHTEXTURE_HPP
#define MESHTEXTURE_HPP

#include <diy/diyClasses.hpp>
#include <diy/mesh/meshDefines.hpp>

#include <string>
#include <vector>
#include <fstream>

#ifdef DIYLIB_EXPORTS
#define DIYLIB_API __declspec(dllexport)
#else
#define DIYLIB_API __declspec(dllimport)
#endif

namespace diy
{
	namespace mesh
	{
		class MeshTexture
		{
		public:
			DIYLIB_API MeshTexture(const MeshTexture& mesh_texture);
			DIYLIB_API ~MeshTexture(void);

			DIYLIB_API void LoadPixels(void);
			DIYLIB_API void LoadPixels(std::string name);

			DIYLIB_API void SetPixels(int width, int height, int depth, const unsigned char* pixels, int offset = 0, int stride = 0);

			DIYLIB_API int GetIndex(void);
			DIYLIB_API std::string GetName(void);
			DIYLIB_API int GetWidth(void);
			DIYLIB_API int GetHeight(void);
			DIYLIB_API int GetDepth(void);
			DIYLIB_API const unsigned char* GetPixels(void);
			DIYLIB_API std::vector<MeshMaterial*> GetMaterials(void);

			DIYLIB_API void SetWrapS(int wrapS);
			DIYLIB_API int GetWrapS(void);
			DIYLIB_API void SetWrapT(int wrapT);
			DIYLIB_API int GetWrapT(void);

			DIYLIB_API void SetMinFilter(int minFilter);
			DIYLIB_API int GetMinFilter(void);
			DIYLIB_API void SetMagFilter(int magFilter);
			DIYLIB_API int GetMagFilter(void);

			DIYLIB_API bool IsInUse(void);

			DIYLIB_API bool SetName(std::string name);

			DIYLIB_API bool LoadPixelsFromFStream(std::ifstream& in);
			DIYLIB_API void SavePixelsToFStream(std::ofstream& out);

			DIYLIB_API void Apply(void);

			DIYLIB_API void ExportDataToFStream(std::ofstream& out);
			DIYLIB_API void ExportAndroidDataToFStream(std::ofstream& out);

			DIYLIB_API bool CopyPixels(unsigned char* pixels, int format, int offset = 0, int stride = 0);
			DIYLIB_API void ToGrayScale(void);
			DIYLIB_API void InvertColor(void);
			DIYLIB_API void InvertAlpha(void);
			DIYLIB_API void DeleteAlpha(void);
			DIYLIB_API void FlipH(void);
			DIYLIB_API void FlipV(void);

			DIYLIB_API void Resize(int width, int height);
			DIYLIB_API bool IsGrayScale(void);
			DIYLIB_API bool IsMonochrome(void);

		private:
			MESH_FRIENDS

			MeshTexture(MeshTextures* textures, int index, std::string path);

			MeshTextures* mTextures;

			int mIndex;
			std::string mName;
			std::string mComment;
			int mWidth, mHeight, mDepth;
			unsigned char* mPixels;
			int mWrapS, mWrapT, mMinFilter, mMagFilter;
			std::vector<MeshMaterial*> mMaterials;

			void Clear(void);

			unsigned int mHandle;

			void SetPixel(int x, int y, const unsigned char* pixel);
			void GetPixel(int x, int y, unsigned char* pixel);

			friend bool SortTexturesIncreasingRespectName(MeshTexture* texture_a, MeshTexture* texture_b);
		};

		bool SortTexturesIncreasingRespectName(MeshTexture* texture_a, MeshTexture* texture_b);
	}
}

#endif

