#include <diy/diy.hpp>

#include <algorithm>

namespace diy
{
	namespace mesh
	{
		MeshTextures::MeshTextures(MeshFile* file) :
			mFile(file)
		{
			mTexture = mTextures.end();
		}

		MeshTextures::~MeshTextures(void)
		{
			Clear();
		}

		MeshFile* MeshTextures::GetFile(void)
		{
			return mFile;
		}

		int MeshTextures::GetCount(void)
		{
			return mTextures.size();
		}

		MeshTexture* MeshTextures::New(std::string name)
		{
			if (name.empty())
			{
				return 0;
			}

			if (Find(name))
			{
				return 0;
			}

			mTextures.push_back(new MeshTexture(this, mTextures.size(), name));
			return mTextures.back();
		}

		void MeshTextures::Clear(void)
		{
			while (mTextures.size())
			{
				delete mTextures.back();
				mTextures.pop_back();
			}
			mTextures.swap(std::vector<MeshTexture*>(mTextures));

			mTexture = mTextures.end();
		}

		bool MeshTextures::Delete(MeshTexture* texture)
		{
			std::vector<MeshTexture*>::iterator iterator = find(mTextures.begin(), mTextures.end(), texture);

			if (iterator == mTextures.end())
			{
				return false;
			}

			if ((*iterator)->IsInUse())
			{
				return false;
			}

			delete *iterator;
			iterator = mTextures.erase(iterator);

			for (iterator = iterator; iterator != mTextures.end(); ++iterator)
			{
				--(*iterator)->mIndex;
			}

			return true;
		}

		void MeshTextures::DeleteUnused(void)
		{
			for (std::vector<MeshTexture*>::iterator texture = mTextures.begin(); texture != mTextures.end();)
			{
				if (!(*texture)->IsInUse())
				{
					delete *texture;
					mTextures.erase(texture);
					texture = mTextures.begin();
					continue;
				}
				++texture;
			}
			int index = 0;
			for (std::vector<MeshTexture*>::iterator texture = mTextures.begin(); texture != mTextures.end(); ++texture)
			{
				(*texture)->mIndex = index++;
			}
		}

		bool MeshTextures::LoadFromFStream(std::ifstream& in)
		{
			int count;
			in.read(reinterpret_cast<char*>(&count), 4);

			mTextures.reserve(count);

			for (int i = 0; i < count; ++i)
			{
				int length;

				in.read(reinterpret_cast<char*>(&length), 4);
				char* name = new char[length];
				in.read(name, length);

				MeshTexture* texture = Find(name, true);
				delete[] name;

				in.read(reinterpret_cast<char*>(&length), 4);
				char* comment = new char[length];
				in.read(comment, length);
				texture->mComment = std::string(comment);
				delete[] comment;

				if (texture->mPixels)
				{
					in.ignore(16);
					int size;
					in.read(reinterpret_cast<char*>(&size), 4);
					in.ignore(size);
				}
				else
				{
					texture->LoadPixelsFromFStream(in);
				}

				in.read(reinterpret_cast<char*>(&texture->mWrapS), 4);
				in.read(reinterpret_cast<char*>(&texture->mWrapT), 4);
				in.read(reinterpret_cast<char*>(&texture->mMinFilter), 4);
				in.read(reinterpret_cast<char*>(&texture->mMagFilter), 4);

				int endTexture;
				in.read(reinterpret_cast<char*>(&endTexture), 4);
			}

			int endTextures;
			in.read(reinterpret_cast<char*>(&endTextures), 4);

			return true;
		}

		void MeshTextures::SaveToFStream(std::ofstream& out)
		{
			int count = mTextures.size();
			out.write(reinterpret_cast<char*>(&count), 4);

			for (std::vector<MeshTexture*>::iterator texture = mTextures.begin(); texture != mTextures.end(); ++texture)
			{
				int length = strlen((*texture)->mName.c_str()) + 1;
				out.write(reinterpret_cast<char*>(&length), 4);
				out.write((*texture)->mName.c_str(), length);
				length = strlen((*texture)->mComment.c_str()) + 1;
				out.write(reinterpret_cast<char*>(&length), 4);
				out.write((*texture)->mComment.c_str(), length);

				(*texture)->SavePixelsToFStream(out);

				out.write(reinterpret_cast<char*>(&(*texture)->mWrapS), 4);
				out.write(reinterpret_cast<char*>(&(*texture)->mWrapT), 4);
				out.write(reinterpret_cast<char*>(&(*texture)->mMinFilter), 4);
				out.write(reinterpret_cast<char*>(&(*texture)->mMagFilter), 4);

				int endTexture = 0;
				out.write(reinterpret_cast<char*>(&endTexture), 4);
			}

			int endTextures = 0;
			out.write(reinterpret_cast<char*>(&endTextures), 4);
		}

		MeshTexture* MeshTextures::operator[](int index)
		{
			if (index < 0 || index >= int(mTextures.size()))
			{
				return 0;
			}

			return mTextures[index];
		}

		MeshTexture* MeshTextures::Find(std::string name, bool create)
		{
			for (std::vector<MeshTexture*>::iterator texture = mTextures.begin(); texture != mTextures.end(); ++texture)
			{
				if (!_stricmp((*texture)->mName.c_str(), name.c_str()))
				{
					return *texture;
				}
			}

			if (create)
			{
				return New(name);
			}

			return 0;
		}

		MeshTexture* MeshTextures::operator()(void)
		{
			if (mTexture == mTextures.end())
			{
				return 0;
			}

			return *mTexture;
		}

		MeshTexture* MeshTextures::First(void)
		{
			if (mTextures.empty())
			{
				mTexture = mTextures.end();
			}
			else
			{
				mTexture = mTextures.begin();
			}

			return operator()();
		}

		MeshTexture* MeshTextures::Next(void)
		{
			if (mTexture != mTextures.end())
			{
				++mTexture;
			}

			return operator()();
		}

		MeshTexture* MeshTextures::Last(void)
		{
			if (mTextures.empty())
			{
				mTexture = mTextures.end();
			}
			else
			{
				mTexture = mTextures.end() - 1;
			}

			return operator()();
		}

		MeshTexture* MeshTextures::Previous(void)
		{
			if (mTexture == mTextures.begin())
			{
				mTexture = mTextures.end();
			}
			else if (mTexture != mTextures.end())
			{
				--mTexture;
			}

			return operator()();
		}

		class MeshTexture* MeshTextures::SeparateAlpha(MeshTexture* texture)
		{
			if (texture->mDepth != 4)
			{
				return 0;
			}

			MeshTexture* new_texture = New(texture->mName + "_alpha");
			new_texture->SetPixels(texture->mWidth, texture->mHeight, 1, texture->mPixels, 3, 3);

			unsigned char* rgb = new unsigned char[3 * texture->mWidth * texture->mHeight];
			texture->CopyPixels(rgb, TEXTURE_RGB);
			texture->SetPixels(texture->mWidth, texture->mHeight, 3, rgb);
			delete[] rgb;

			return new_texture;
		}

		bool MeshTextures::MergeAlpha(MeshTexture* destiny, MeshTexture* source)
		{
			if (destiny->mDepth != 1 && destiny->mDepth != 3)
			{
				return false;
			}

			if (destiny->mWidth != source->mWidth || destiny->mHeight != destiny->mHeight)
			{
				return false;
			}

			if (destiny->mDepth == 1)
			{
				unsigned char* gray_alpha = new unsigned char[destiny->mWidth * destiny->mHeight * 2];
				destiny->CopyPixels(gray_alpha, TEXTURE_GRAY, 0, 1);
				source->CopyPixels(gray_alpha, TEXTURE_GRAY, 1, 1);
				destiny->SetPixels(destiny->mWidth, destiny->mHeight, 2, gray_alpha);
				delete[] gray_alpha;
			}
			else
			{
				unsigned char* rgba = new unsigned char[destiny->mWidth * destiny->mHeight * 4];
				destiny->CopyPixels(rgba, TEXTURE_RGB, 0, 1);
				source->CopyPixels(rgba, TEXTURE_GRAY, 3, 3);
				destiny->SetPixels(destiny->mWidth, destiny->mHeight, 4, rgba);
				delete[] rgba;
			}

			if (destiny != source)
			{
				Delete(source);
			}

			return true;
		}

		void MeshTextures::Sort(void)
		{
			sort(mTextures.begin(), mTextures.end(), SortTexturesIncreasingRespectName);

			int index = 0;
			for (std::vector<MeshTexture*>::iterator texture = mTextures.begin(); texture != mTextures.end(); ++texture)
			{
				(*texture)->mIndex = index++;
			}
		}

		void MeshTextures::Reverse(void)
		{
			reverse(mTextures.begin(), mTextures.end());

			int index = 0;
			for (std::vector<MeshTexture*>::iterator texture = mTextures.begin(); texture != mTextures.end(); ++texture)
			{
				(*texture)->mIndex = index++;
			}
		}
	}
}
