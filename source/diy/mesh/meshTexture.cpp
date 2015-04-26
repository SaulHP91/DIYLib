#include <diy/diy.hpp>

#include <gl\glew.h>
#include <gl\glu.h>
#include <zlib.h>

namespace diy
{
	namespace mesh
	{
		MeshTexture::MeshTexture(MeshTextures* textures, int index, std::string name) :
			mTextures(textures),
			mIndex(index),
			mName(name),
			mComment(),
			mWidth(0),
			mHeight(0),
			mDepth(0),
			mPixels(0),
			mWrapS(TEXTURE_WRAP_REPEAT),
			mWrapT(TEXTURE_WRAP_REPEAT),
			mMinFilter(TEXTURE_FILTER_LINEAR),
			mMagFilter(TEXTURE_FILTER_LINEAR),
			mHandle(0)
		{
			LoadPixels();
		}

		MeshTexture::MeshTexture(const MeshTexture& mesh_texture)
		{
			mName = mesh_texture.mName;
			mWidth = mesh_texture.mWidth;
			mHeight = mesh_texture.mHeight;
			mDepth = mesh_texture.mDepth;
			mPixels = new unsigned char[mWidth * mHeight * mDepth];
			memcpy(mPixels, mesh_texture.mPixels, mWidth * mHeight * mDepth);
		}

		MeshTexture::~MeshTexture(void)
		{
			Clear();

			mMaterials.clear();
			mMaterials.swap(std::vector<MeshMaterial*>(mMaterials));
		}

		void MeshTexture::Clear(void)
		{
			mWidth = mHeight = mDepth = 0;

			delete[] mPixels;
			mPixels = 0;

			if (mHandle)
			{
				glDeleteTextures(1, &mHandle);
				mHandle = 0;
			}
		}

		void MeshTexture::LoadPixels(void)
		{
			LoadPixels(mName);
		}

#include <FreeImage.h>

#pragma link "FreeImage.lib"

		void MeshTexture::LoadPixels(std::string name)
		{
			FREE_IMAGE_FORMAT format = FreeImage_GetFileType(name.c_str());
			if (format == FIF_UNKNOWN)
			{
				return;
			}

			FIBITMAP* bitmap = FreeImage_Load(format, name.c_str(), 0);
			int width = FreeImage_GetWidth(bitmap);
			int height = FreeImage_GetHeight(bitmap);
			int depth = FreeImage_GetBPP(bitmap) / 8;
			unsigned char* pixels = FreeImage_GetBits(bitmap);
			if (depth == 3 || depth == 4)
			{
				for (int i = 0; i < width * height; ++i)
				{
					unsigned char aux = pixels[i * depth];
					pixels[i * depth] = pixels[i * depth + 2];
					pixels[i * depth + 2] = aux;
				}
			}
			SetPixels(width, height, depth, pixels);
			FreeImage_Unload(bitmap);
		}

		void MeshTexture::SetPixels(int width, int height, int depth, const unsigned char* pixels, int offset, int stride)
		{
			Clear();

			mWidth = width;
			mHeight = height;
			mDepth = depth;

			mPixels = new unsigned char[mWidth * mHeight * mDepth];

			for (int i = 0; i < mWidth * mHeight; ++i)
			{
				for (int j = 0; j < mDepth; ++j)
				{
					mPixels[i * mDepth + j] = pixels[offset + i * (mDepth + stride) + j];
				}
			}

			if (!mHandle)
			{
				glGenTextures(1, &mHandle);
			}
			if (mHandle)
			{
				glBindTexture(GL_TEXTURE_2D, mHandle);
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

				gluBuild2DMipmaps
					(
					GL_TEXTURE_2D,
					TEXTURE_DEPTH_TO_GL_COMPONENTS(mDepth),
					mWidth,
					mHeight,
					TEXTURE_DEPTH_TO_GL_FORMAT(mDepth),
					GL_UNSIGNED_BYTE,
					mPixels
					);
			}
		}

		int MeshTexture::GetIndex(void)
		{
			return mIndex;
		}

		std::string MeshTexture::GetName(void)
		{
			return mName;
		}

		int MeshTexture::GetWidth(void)
		{
			return mWidth;
		}

		int MeshTexture::GetHeight(void)
		{
			return mHeight;
		}

		int MeshTexture::GetDepth(void)
		{
			return mDepth;
		}

		const unsigned char* MeshTexture::GetPixels(void)
		{
			return mPixels;
		}

		void MeshTexture::SetWrapS(int wrapS)
		{
			mWrapS = wrapS;
		}

		int MeshTexture::GetWrapS(void)
		{
			return mWrapS;
		}

		void MeshTexture::SetWrapT(int wrapT)
		{
			mWrapT = wrapT;
		}

		int MeshTexture::GetWrapT(void)
		{
			return mWrapT;
		}

		void MeshTexture::SetMinFilter(int minFilter)
		{
			mMinFilter = minFilter;
		}

		int MeshTexture::GetMinFilter(void)
		{
			return mMinFilter;
		}

		void MeshTexture::SetMagFilter(int magFilter)
		{
			mMagFilter = magFilter;
		}

		int MeshTexture::GetMagFilter(void)
		{
			return mMagFilter;
		}

		std::vector<MeshMaterial*> MeshTexture::GetMaterials(void)
		{
			return mMaterials;
		}

		bool MeshTexture::IsInUse(void)
		{
			return mMaterials.size();
		}

		bool MeshTexture::SetName(std::string name)
		{
			if (mTextures->Find(name))
			{
				return false;
			}

			mName = name;

			return true;
		}

		bool MeshTexture::LoadPixelsFromFStream(std::ifstream& in)
		{
			int width;
			int height;
			int depth;
			int format;

			in.read(reinterpret_cast<char*>(&width), 4);
			in.read(reinterpret_cast<char*>(&height), 4);
			in.read(reinterpret_cast<char*>(&depth), 4);
			in.read(reinterpret_cast<char*>(&format), 4);

			unsigned long sourceLength;
			in.read(reinterpret_cast<char*>(&sourceLength), 4);
			unsigned char* source = new unsigned char[sourceLength];
			in.read(reinterpret_cast<char*>(source), sourceLength);
			unsigned long destinyLength = width * height * depth;
			unsigned char* destiny = new unsigned char[destinyLength];
			uncompress(destiny, &destinyLength, source, sourceLength);
			delete[] source;
			SetPixels(width, height, depth, destiny);
			delete[] destiny;

			return true;
		}

		void MeshTexture::SavePixelsToFStream(std::ofstream& out)
		{
			out.write(reinterpret_cast<char*>(&mWidth), 4);
			out.write(reinterpret_cast<char*>(&mHeight), 4);
			out.write(reinterpret_cast<char*>(&mDepth), 4);
			int format = TEXTURE_COMPRESSED;
			out.write(reinterpret_cast<char*>(&format), 4);

			unsigned long sourceLength = mWidth * mHeight * mDepth;
			unsigned char* source = mPixels;
			unsigned long destinyLength = mWidth * mHeight * mDepth;
			unsigned char* destiny = new unsigned char[destinyLength];
			compress(destiny, &destinyLength, reinterpret_cast<Bytef*>(source), sourceLength);
			out.write(reinterpret_cast<char*>(&destinyLength), 4);
			out.write(reinterpret_cast<char*>(destiny), destinyLength);
			delete[] destiny;
		}

		void MeshTexture::Apply(void)
		{
			if (mHandle)
			{
				if (mDepth == 2 || mDepth == 4)
				{
					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				}

				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, mHandle);

				if (mWrapS == TEXTURE_WRAP_CLAMP)
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
				}
				else
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				}

				if (mWrapT == TEXTURE_WRAP_CLAMP)
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
				}
				else
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				}

				if (mMinFilter == TEXTURE_FILTER_NEAREST)
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
				}
				else
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				}

				if (mMagFilter == TEXTURE_FILTER_NEAREST)
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				}
				else
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				}
			}
		}

		void MeshTexture::ExportDataToFStream(std::ofstream& out)
		{
			int length = mName.length();
			out.write(reinterpret_cast<char*>(&length), 4);
			out.write(mName.c_str(), length);
			out.write(reinterpret_cast<char*>(&mWidth), 4);
			out.write(reinterpret_cast<char*>(&mHeight), 4);
			out.write(reinterpret_cast<char*>(&mDepth), 4);
			out.write(reinterpret_cast<char*>(mPixels), mWidth * mHeight * mDepth);
		}

		void MeshTexture::ExportAndroidDataToFStream(std::ofstream& out)
		{
			int length = mName.length();
			out.write(reinterpret_cast<char*>(&length), 4);
			out.write(mName.c_str(), length);

			int width = mWidth;
			int height = mHeight;
			int depth = mDepth;
			unsigned char* pixels = new unsigned char[mWidth * mHeight * mDepth];
			memcpy(pixels, mPixels, width * height * mDepth);

			if (mWidth > 256 || mHeight > 256)
			{
				float ratio = mWidth > mHeight ? 256.0f / mWidth : 256.0f / mHeight;
				Resize(ratio * mWidth, ratio * mHeight);
			}

			out.write(reinterpret_cast<char*>(&mWidth), 4);
			out.write(reinterpret_cast<char*>(&mHeight), 4);
			switch (mDepth)
			{
			case 1: case 2:
			{
				int format = mDepth == 1 ? TEXTURE_GRAY_8 : TEXTURE_GRAY_ALPHA_88;
				out.write(reinterpret_cast<char*>(&format), 4);
				out.write(reinterpret_cast<char*>(mPixels), mWidth * mHeight * mDepth);
			}
			break;
			case 3:
			{
				if (mWidth * mHeight > 128)
				{
					FIBITMAP* src = FreeImage_Allocate(mWidth, mHeight, 24);
					memcpy(FreeImage_GetBits(src), mPixels, mWidth * mHeight * 3);
					FIBITMAP* dst = FreeImage_ColorQuantize(src, FIQ_NNQUANT);
					FreeImage_Unload(src);
					int format = TEXTURE_RGB_PALETTED_8;
					out.write(reinterpret_cast<char*>(&format), 4);
					RGBQUAD* palette = FreeImage_GetPalette(dst);
					for (int i = 0; i < 256; ++i)
					{
						unsigned char r = palette[i].rgbBlue;
						out.write(reinterpret_cast<char*>(&r), 1);
						unsigned char g = palette[i].rgbGreen;
						out.write(reinterpret_cast<char*>(&g), 1);
						unsigned char b = palette[i].rgbRed;
						out.write(reinterpret_cast<char*>(&b), 1);
					}
					out.write(reinterpret_cast<char*>(FreeImage_GetBits(dst)), mWidth * mHeight);
					FreeImage_Unload(dst);
				}
				else
				{
					int format = TEXTURE_RGB_888;
					out.write(reinterpret_cast<char*>(&format), 4);
					out.write(reinterpret_cast<char*>(mPixels), mWidth * mHeight * mDepth);
				}
			}
			break;
			case 4:
			{
				if (mWidth * mHeight > 86)
				{
					FIBITMAP* src = FreeImage_Allocate(mWidth, mHeight, 24);
					CopyPixels(FreeImage_GetBits(src), TEXTURE_RGB);
					FIBITMAP* dst = FreeImage_ColorQuantize(src, FIQ_NNQUANT);
					FreeImage_Unload(src);
					int format = TEXTURE_RGB_PALETTED_8_ALPHA_8;
					out.write(reinterpret_cast<char*>(&format), 4);
					RGBQUAD* palette = FreeImage_GetPalette(dst);
					for (int i = 0; i < 256; ++i)
					{
						unsigned char r = palette[i].rgbBlue;
						out.write(reinterpret_cast<char*>(&r), 1);
						unsigned char g = palette[i].rgbGreen;
						out.write(reinterpret_cast<char*>(&g), 1);
						unsigned char b = palette[i].rgbRed;
						out.write(reinterpret_cast<char*>(&b), 1);
					}
					out.write(reinterpret_cast<char*>(FreeImage_GetBits(dst)), mWidth * mHeight);
					for (int i = 0; i < mWidth * mHeight; ++i)
					{
						out.write(reinterpret_cast<char*>(mPixels + 4 * i + 3), 1);
					}
					FreeImage_Unload(dst);
				}
				else
				{
					int format = TEXTURE_RGBA_8888;
					out.write(reinterpret_cast<char*>(&format), 4);
					out.write(reinterpret_cast<char*>(mPixels), mWidth * mHeight * mDepth);
				}
			}
			break;
			default:
			{
				;
			}
			}

			SetPixels(width, height, depth, pixels);
			delete[] pixels;
		}

		bool MeshTexture::CopyPixels(unsigned char* pixels, int format, int offset, int stride)
		{
			switch (format)
			{
			case TEXTURE_GRAY:
			{
				switch (mDepth)
				{
				case 1:
				{
					for (int i = 0; i < mWidth * mHeight; ++i)
					{
						pixels[offset + i * (stride + 1)] = mPixels[i];
					}
					break;
				}
				case 2:
				{
					for (int i = 0; i < mWidth * mHeight; ++i)
					{
						pixels[offset + i * (stride + 1)] = mPixels[2 * i];
					}
					break;
				}
				case 3:
				{
					for (int i = 0; i < mWidth * mHeight; ++i)
					{
						float r = static_cast<float>(mPixels[3 * i]);
						float g = static_cast<float>(mPixels[3 * i + 1]);
						float b = static_cast<float>(mPixels[3 * i + 2]);
						pixels[offset + i * (stride + 1)] = static_cast<unsigned char>(0.2126f * r + 0.7152f * g + 0.0722f * b);
					}
					break;
				}
				case 4:
				{
					for (int i = 0; i < mWidth * mHeight; ++i)
					{
						float r = static_cast<float>(mPixels[4 * i]);
						float g = static_cast<float>(mPixels[4 * i + 1]);
						float b = static_cast<float>(mPixels[4 * i + 2]);
						pixels[offset + i * (stride + 1)] = static_cast<unsigned char>(0.2126f * r + 0.7152f * g + 0.0722f * b);
					}
					break;
				}
				default:
				{
					return false;
				}
				}
				break;
			}
			case TEXTURE_GRAY_ALPHA:
			{
				switch (mDepth)
				{
				case 1:
				{
					for (int i = 0; i < mWidth * mHeight; ++i)
					{
						pixels[offset + i * (stride + 2)] = mPixels[i];
						pixels[offset + i * (stride + 2) + 1] = 255;
					}
					break;
				}
				case 2:
				{
					for (int i = 0; i < mWidth * mHeight; ++i)
					{
						pixels[offset + i * (stride + 2)] = mPixels[2 * i];
						pixels[offset + i * (stride + 2) + 1] = mPixels[2 * i + 1];
					}
					break;
				}
				case 3:
				{
					for (int i = 0; i < mWidth * mHeight; ++i)
					{
						float r = static_cast<float>(mPixels[3 * i]);
						float g = static_cast<float>(mPixels[3 * i + 1]);
						float b = static_cast<float>(mPixels[3 * i + 2]);
						pixels[offset + i * (stride + 2)] = static_cast<unsigned char>(0.2126f * r + 0.7152f * g + 0.0722f * b);
						pixels[offset + i * (stride + 2) + 1] = 255;
					}
					break;
				}
				case 4:
				{
					for (int i = 0; i < mWidth * mHeight; ++i)
					{
						float r = static_cast<float>(mPixels[4 * i]);
						float g = static_cast<float>(mPixels[4 * i + 1]);
						float b = static_cast<float>(mPixels[4 * i + 2]);
						pixels[offset + i * (stride + 2)] = static_cast<unsigned char>(0.2126f * r + 0.7152f * g + 0.0722f * b);
						pixels[offset + i * (stride + 2) + 1] = mPixels[4 * i + 3];
					}
					break;
				}
				default:
				{
					return false;
				}
				}
				break;
			}
			case TEXTURE_RGB:
			{
				switch (mDepth)
				{
				case 1:
				{
					for (int i = 0; i < mWidth * mHeight; ++i)
					{
						pixels[offset + i * (stride + 3)] = mPixels[i];
						pixels[offset + i * (stride + 3) + 1] = mPixels[i];
						pixels[offset + i * (stride + 3) + 2] = mPixels[i];
					}
					break;
				}
				case 2:
				{
					for (int i = 0; i < mWidth * mHeight; ++i)
					{
						pixels[offset + i * (stride + 3)] = mPixels[2 * i];
						pixels[offset + i * (stride + 3) + 1] = mPixels[2 * i];
						pixels[offset + i * (stride + 3) + 2] = mPixels[2 * i];
					}
					break;
				}
				case 3:
				{
					for (int i = 0; i < mWidth * mHeight; ++i)
					{
						pixels[offset + i * (stride + 3)] = mPixels[3 * i];
						pixels[offset + i * (stride + 3) + 1] = mPixels[3 * i + 1];
						pixels[offset + i * (stride + 3) + 2] = mPixels[3 * i + 2];
					}
					break;
				}
				case 4:
				{
					for (int i = 0; i < mWidth * mHeight; ++i)
					{
						pixels[offset + i * (stride + 3)] = mPixels[4 * i];
						pixels[offset + i * (stride + 3) + 1] = mPixels[4 * i + 1];
						pixels[offset + i * (stride + 3) + 2] = mPixels[4 * i + 2];
					}
					break;
				}
				default:
				{
					return false;
				}
				}
				break;
			}
			case TEXTURE_RGBA:
			{
				switch (mDepth)
				{
				case 1:
				{
					for (int i = 0; i < mWidth * mHeight; ++i)
					{
						pixels[offset + i * (stride + 4)] = mPixels[i];
						pixels[offset + i * (stride + 4) + 1] = mPixels[i];
						pixels[offset + i * (stride + 4) + 2] = mPixels[i];
						pixels[offset + i * (stride + 4) + 3] = 255;
					}
					break;
				}
				case 2:
				{
					for (int i = 0; i < mWidth * mHeight; ++i)
					{
						pixels[offset + i * (stride + 4)] = mPixels[2 * i];
						pixels[offset + i * (stride + 4) + 1] = mPixels[2 * i];
						pixels[offset + i * (stride + 4) + 2] = mPixels[2 * i];
						pixels[offset + i * (stride + 4) + 3] = mPixels[2 * i + 1];
					}
					break;
				}
				case 3:
				{
					for (int i = 0; i < mWidth * mHeight; ++i)
					{
						pixels[offset + i * (stride + 4)] = mPixels[3 * i];
						pixels[offset + i * (stride + 4) + 1] = mPixels[3 * i + 1];
						pixels[offset + i * (stride + 4) + 2] = mPixels[3 * i + 2];
						pixels[offset + i * (stride + 4) + 3] = 255;
					}
					break;
				}
				case 4:
				{
					for (int i = 0; i < mWidth * mHeight; ++i)
					{
						pixels[offset + i * (stride + 4)] = mPixels[4 * i];
						pixels[offset + i * (stride + 4) + 1] = mPixels[4 * i + 1];
						pixels[offset + i * (stride + 4) + 2] = mPixels[4 * i + 2];
						pixels[offset + i * (stride + 4) + 3] = mPixels[4 * i + 3];
					}
					break;
				}
				default:
				{
					return false;
				}
				}
				break;
			}
			case TEXTURE_RED:
			{
				switch (mDepth)
				{
				case 1:
				{
					for (int i = 0; i < mWidth * mHeight; ++i)
					{
						pixels[offset + i * (stride + 1)] = mPixels[i];
					}
					break;
				}
				case 2:
				{
					for (int i = 0; i < mWidth * mHeight; ++i)
					{
						pixels[offset + i * (stride + 1)] = mPixels[2 * i];
					}
					break;
				}
				case 3:
				{
					for (int i = 0; i < mWidth * mHeight; ++i)
					{
						pixels[offset + i * (stride + 1)] = mPixels[3 * i];
					}
					break;
				}
				case 4:
				{
					for (int i = 0; i < mWidth * mHeight; ++i)
					{
						pixels[offset + i * (stride + 1)] = mPixels[4 * i];
					}
					break;
				}
				default:
				{
					return false;
				}
				}
				break;
			}
			case TEXTURE_GREEN:
			{
				switch (mDepth)
				{
				case 1:
				{
					for (int i = 0; i < mWidth * mHeight; ++i)
					{
						pixels[offset + i * (stride + 1)] = mPixels[i];
					}
					break;
				}
				case 2:
				{
					for (int i = 0; i < mWidth * mHeight; ++i)
					{
						pixels[offset + i * (stride + 1)] = mPixels[2 * i];
					}
					break;
				}
				case 3:
				{
					for (int i = 0; i < mWidth * mHeight; ++i)
					{
						pixels[offset + i * (stride + 1)] = mPixels[3 * i + 1];
					}
					break;
				}
				case 4:
				{
					for (int i = 0; i < mWidth * mHeight; ++i)
					{
						pixels[offset + i * (stride + 1)] = mPixels[4 * i + 1];
					}
					break;
				}
				default:
				{
					return false;
				}
				}
				break;
			}
			case TEXTURE_BLUE:
			{
				switch (mDepth)
				{
				case 1:
				{
					for (int i = 0; i < mWidth * mHeight; ++i)
					{
						pixels[offset + i * (stride + 1)] = mPixels[i];
					}
					break;
				}
				case 2:
				{
					for (int i = 0; i < mWidth * mHeight; ++i)
					{
						pixels[offset + i * (stride + 1)] = mPixels[2 * i];
					}
					break;
				}
				case 3:
				{
					for (int i = 0; i < mWidth * mHeight; ++i)
					{
						pixels[offset + i * (stride + 1)] = mPixels[3 * i + 2];
					}
					break;
				}
				case 4:
				{
					for (int i = 0; i < mWidth * mHeight; ++i)
					{
						pixels[offset + i * (stride + 1)] = mPixels[4 * i + 2];
					}
					break;
				}
				default:
				{
					return false;
				}
				}
				break;
			}
			case TEXTURE_ALPHA:
			{
				switch (mDepth)
				{
				case 1:
				{
					for (int i = 0; i < mWidth * mHeight; ++i)
					{
						pixels[offset + i * (stride + 1)] = 255;
					}
					break;
				}
				case 2:
				{
					for (int i = 0; i < mWidth * mHeight; ++i)
					{
						pixels[offset + i * (stride + 1)] = mPixels[2 * i + 1];
					}
					break;
				}
				case 3:
				{
					for (int i = 0; i < mWidth * mHeight; ++i)
					{
						pixels[offset + i * (stride + 1)] = 255;
					}
					break;
				}
				case 4:
				{
					for (int i = 0; i < mWidth * mHeight; ++i)
					{
						pixels[offset + i * (stride + 1)] = mPixels[4 * i + 3];
					}
					break;
				}
				default:
				{
					return false;
				}
				}
				break;
			}
			default:
			{
				return false;
			}
			}

			return true;
		}

		void MeshTexture::ToGrayScale(void)
		{
			if (mDepth != 3 && mDepth != 4)
			{
				return;
			}

			if (mDepth == 3)
			{
				unsigned char* gray = new unsigned char[mWidth * mHeight];
				CopyPixels(gray, TEXTURE_GRAY);
				SetPixels(mWidth, mHeight, 1, gray);
				delete[] gray;
			}
			else
			{
				unsigned char* gray_alpha = new unsigned char[mWidth * mHeight * 2];
				CopyPixels(gray_alpha, TEXTURE_GRAY_ALPHA);
				SetPixels(mWidth, mHeight, 2, gray_alpha);
				delete[] gray_alpha;
			}
		}

		void MeshTexture::InvertColor(void)
		{
			switch (mDepth)
			{
			case 1:
			{
				for (int i = 0; i < mWidth * mHeight; ++i)
				{
					mPixels[i] = 255 - mPixels[i];
				}
				break;
			}
			case 2:
			{
				for (int i = 0; i < mWidth * mHeight; ++i)
				{
					mPixels[2 * i] = 255 - mPixels[2 * i];
				}
				break;
			}
			case 3:
			{
				for (int i = 0; i < mWidth * mHeight * 3; ++i)
				{
					mPixels[i] = 255 - mPixels[i];
				}
				break;
			}
			case 4:
			{
				for (int i = 0; i < mWidth * mHeight; ++i)
				{
					mPixels[4 * i] = 255 - mPixels[4 * i];
					mPixels[4 * i + 1] = 255 - mPixels[4 * i + 1];
					mPixels[4 * i + 2] = 255 - mPixels[4 * i + 2];
				}
				break;
			}
			default:
			{
				return;
			}
			}

			glBindTexture(GL_TEXTURE_2D, mHandle);

			gluBuild2DMipmaps
				(
				GL_TEXTURE_2D,
				TEXTURE_DEPTH_TO_GL_COMPONENTS(mDepth),
				mWidth,
				mHeight,
				TEXTURE_DEPTH_TO_GL_FORMAT(mDepth),
				GL_UNSIGNED_BYTE,
				mPixels
				);
		}

		void MeshTexture::InvertAlpha(void)
		{
			switch (mDepth)
			{
			case 2:
			{
				for (int i = 0; i < mWidth * mHeight; ++i)
				{
					mPixels[2 * i] = 255 - mPixels[2 * i];
				}
				break;
			}
			case 4:
			{
				for (int i = 0; i < mWidth * mHeight; ++i)
				{
					mPixels[4 * i + 3] = 255 - mPixels[4 * i + 3];
				}
				break;
			}
			default:
			{
				return;
			}
			}

			glBindTexture(GL_TEXTURE_2D, mHandle);

			gluBuild2DMipmaps
				(
				GL_TEXTURE_2D,
				TEXTURE_DEPTH_TO_GL_COMPONENTS(mDepth),
				mWidth,
				mHeight,
				TEXTURE_DEPTH_TO_GL_FORMAT(mDepth),
				GL_UNSIGNED_BYTE,
				mPixels
				);
		}

		void MeshTexture::DeleteAlpha(void)
		{
			switch (mDepth)
			{
			case 2:
			{
				for (int i = 0; i < mWidth * mHeight; ++i)
				{
					mPixels[i] = mPixels[2 * i];
				}
				break;
			}
			case 4:
			{
				for (int i = 0; i < mWidth * mHeight; ++i)
				{
					mPixels[3 * i] = mPixels[4 * i];
					mPixels[3 * i + 1] = mPixels[4 * i + 1];
					mPixels[3 * i + 2] = mPixels[4 * i + 2];
				}
				break;
			}
			default:
			{
				return;
			}
			}

			mDepth--;

			glBindTexture(GL_TEXTURE_2D, mHandle);

			gluBuild2DMipmaps
				(
				GL_TEXTURE_2D,
				TEXTURE_DEPTH_TO_GL_COMPONENTS(mDepth),
				mWidth,
				mHeight,
				TEXTURE_DEPTH_TO_GL_FORMAT(mDepth),
				GL_UNSIGNED_BYTE,
				mPixels
				);
		}

		void MeshTexture::FlipH(void)
		{
			for (int i = 0; i < mHeight; ++i)
			{
				for (int j = 0; j < mWidth / 2; ++j)
				{
					for (int k = 0; k < mDepth; ++k)
					{
						unsigned char aux = mPixels[i * mWidth * mDepth + j * mDepth + k];
						mPixels[i * mWidth * mDepth + j * mDepth + k] = mPixels[i * mWidth * mDepth + (mWidth - j - 1) * mDepth + k];
						mPixels[i * mWidth * mDepth + (mWidth - j - 1) * mDepth + k] = aux;
					}
				}
			}

			glBindTexture(GL_TEXTURE_2D, mHandle);

			gluBuild2DMipmaps
				(
				GL_TEXTURE_2D,
				TEXTURE_DEPTH_TO_GL_COMPONENTS(mDepth),
				mWidth,
				mHeight,
				TEXTURE_DEPTH_TO_GL_FORMAT(mDepth),
				GL_UNSIGNED_BYTE,
				mPixels
				);
		}

		void MeshTexture::FlipV(void)
		{
			for (int i = 0; i < mHeight / 2; ++i)
			{
				for (int j = 0; j < mWidth; ++j)
				{
					for (int k = 0; k < mDepth; ++k)
					{
						unsigned char aux = mPixels[i * mWidth * mDepth + j * mDepth + k];
						mPixels[i * mWidth * mDepth + j * mDepth + k] = mPixels[(mHeight - i - 1) * mWidth * mDepth + j * mDepth + k];
						mPixels[(mHeight - i - 1) * mWidth * mDepth + j * mDepth + k] = aux;
					}
				}
			}

			glBindTexture(GL_TEXTURE_2D, mHandle);

			gluBuild2DMipmaps
				(
				GL_TEXTURE_2D,
				TEXTURE_DEPTH_TO_GL_COMPONENTS(mDepth),
				mWidth,
				mHeight,
				TEXTURE_DEPTH_TO_GL_FORMAT(mDepth),
				GL_UNSIGNED_BYTE,
				mPixels
				);
		}

		void MeshTexture::SetPixel(int x, int y, const unsigned char* pixel)
		{
			for (int i = 0; i < mDepth; ++i)
			{
				mPixels[(x + y * mWidth) * mDepth + i] = pixel[i];
			}
		}

		void MeshTexture::GetPixel(int x, int y, unsigned char* pixel)
		{
			for (int i = 0; i < mDepth; ++i)
			{
				pixel[i] = mPixels[(x + y * mWidth) * mDepth + i];
			}
		}

		void MeshTexture::Resize(int width, int height)
		{
			if (width < 0 || height < 0)
			{
				return;
			}

			if (width == 0)
			{
				width = mWidth;
			}

			if (height == 0)
			{
				height = mHeight;
			}

			if (width == mWidth && height == mHeight)
			{
				return;
			}

			if (mDepth == 2)
			{
				unsigned char* pixels = new unsigned char[mWidth * mHeight * 4];
				CopyPixels(pixels, TEXTURE_RGBA);
				FIBITMAP* src = FreeImage_Allocate(mWidth, mHeight, 32);
				memcpy(FreeImage_GetBits(src), pixels, mWidth * mHeight * 4);
				delete[] pixels;
				FIBITMAP* dst = FreeImage_Rescale(src, width, height, FILTER_CATMULLROM);
				FreeImage_Unload(src);
				SetPixels(width, height, 2, FreeImage_GetBits(dst), 2, 2);
				FreeImage_Unload(dst);
			}
			else
			{
				FIBITMAP* src = FreeImage_Allocate(mWidth, mHeight, 8 * mDepth);
				memcpy(FreeImage_GetBits(src), mPixels, mWidth * mHeight * mDepth);
				FIBITMAP* dst = FreeImage_Rescale(src, width, height, FILTER_CATMULLROM);
				FreeImage_Unload(src);
				SetPixels(width, height, mDepth, FreeImage_GetBits(dst));
				FreeImage_Unload(dst);
			}
		}

		bool MeshTexture::IsGrayScale(void)
		{
			if (mDepth == 1 || mDepth == 2)
			{
				return true;
			}

			float deviation = 0.0;
			for (int i = 0; i < mWidth * mHeight; ++i)
			{
				float r = static_cast<float>(mPixels[mDepth * i]);
				float g = static_cast<float>(mPixels[mDepth * i + 1]);
				float b = static_cast<float>(mPixels[mDepth * i + 2]);
				unsigned char gray = static_cast<unsigned char>(0.2126f * r + 0.7152f * g + 0.0722f * b);
				deviation += abs(r - gray);
				deviation += abs(g - gray);
				deviation += abs(b - gray);
			}
			deviation /= mWidth * mHeight;
			deviation /= 255.0f;

			return deviation < 0.02f;
		}

		bool MeshTexture::IsMonochrome(void)
		{
			if (mDepth != 1 && mDepth != 2)
			{
				return false;
			}

			float deviation = 0.0;
			for (int i = 0; i < mWidth * mHeight; ++i)
			{
				deviation += abs(mPixels[mDepth * i] - 128);
			}
			deviation /= mWidth * mHeight;
			deviation /= 128.0f;

			return deviation > 0.98f;
		}

		bool SortTexturesIncreasingRespectName(class MeshTexture* texture_a, class MeshTexture* texture_b)
		{
			return strcmp(texture_a->mName.c_str(), texture_b->mName.c_str()) < 0;
		}
	}
}
