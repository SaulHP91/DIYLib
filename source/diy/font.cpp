#include <gl/glew.h>

#include <diy/font.hpp>
#include <diy/camera.hpp>
#include <diy/shader.hpp>
#include <diy/matrixStack.hpp>

#include <glm/ext.hpp>

#include <tinyxml2.h>

#include <FreeImage.h>

namespace diy
{

	Font::Font(void) :
		mSize(8.0f),
		mColor(0.0f, 0.0f, 0.0f, 1.0f),
		mCursor(0.0f),
		mCurrentCursor(0.0f),
		mLineHeight(1.0f),
		mSpaceAdvance(1.0f),
		mSpaceAdvancesPerTab(8),
		mPositionTexCoordBuffer(0),
		mFaceBuffer(0),
		mInitialized(false),
		mInitializedShader(false)
	{
		mShader = new Shader();

		mMatrixStack = new MatrixStack();
	}

	Font::~Font(void)
	{
		if (mPositionTexCoordBuffer)
		{
			glDeleteBuffers(1, &mPositionTexCoordBuffer);
		}

		if (mFaceBuffer)
		{
			glDeleteBuffers(1, &mFaceBuffer);
		}

		delete mShader;

		delete mMatrixStack;

		Clear();
	}

	void Font::LoadFromFile(std::string file)
	{
		tinyxml2::XMLDocument xml_in;
		xml_in.LoadFile(file.c_str());

		tinyxml2::XMLElement* font = xml_in.FirstChildElement("font");
		if (font)
		{
			Clear();

			tinyxml2::XMLElement* info = font->FirstChildElement("info");
			if (info)
			{
				mInfo.face = info->Attribute("face");
				mInfo.size = strtol(info->Attribute("size"), 0, 10);
			}

			tinyxml2::XMLElement* common = font->FirstChildElement("common");
			if (common)
			{
				mCommon.lineHeight = strtol(common->Attribute("lineHeight"), 0, 10);
				mCommon.base = strtol(common->Attribute("base"), 0, 10);
				mCommon.scaleW = strtol(common->Attribute("scaleW"), 0, 10);
				mCommon.scaleH = strtol(common->Attribute("scaleH"), 0, 10);
				mLineHeight = float(mCommon.lineHeight) / float(mInfo.size);
			}

			tinyxml2::XMLElement* pages = font->FirstChildElement("pages");
			if (pages)
			{
				tinyxml2::XMLElement* page = pages->FirstChildElement("page");
				while (page)
				{
					mPages.push_back(Page());
					mPages.back().id = strtol(page->Attribute("id"), 0, 10);
					mPages.back().file = page->Attribute("file");

					FREE_IMAGE_FORMAT format = FreeImage_GetFileType(mPages.back().file.c_str());
					FIBITMAP* bitmap = FreeImage_Load(format, mPages.back().file.c_str(), 0);
					int width = FreeImage_GetWidth(bitmap);
					int height = FreeImage_GetHeight(bitmap);
					int depth = FreeImage_GetBPP(bitmap) / 8;
					unsigned char* pixels = FreeImage_GetBits(bitmap);
					unsigned char* luminance = new unsigned char[width * height];
					for (int n = 0; n < width * height; ++n)
					{
						luminance[n] = pixels[depth * n];
					}

					glGenTextures(1, &mPages.back().handle);
					glBindTexture(GL_TEXTURE_2D, mPages.back().handle);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

					gluBuild2DMipmaps
						(
						GL_TEXTURE_2D,
						GL_LUMINANCE8,
						width,
						height,
						GL_LUMINANCE,
						GL_UNSIGNED_BYTE,
						luminance
						);

					delete[] luminance;

					FreeImage_Unload(bitmap);

					page = page->NextSiblingElement("page");
				}
			}

			tinyxml2::XMLElement* chars = font->FirstChildElement("chars");
			if (chars)
			{
				std::vector<glm::ivec3> faces;
				std::vector<glm::vec2> positionTexCoords;
				int charCount = 0;
				tinyxml2::XMLElement* _char = chars->FirstChildElement("char");
				while (_char)
				{
					std::pair<int, Char> newChar;
					newChar.first = strtol(_char->Attribute("id"), 0, 10);
					newChar.second.x = strtol(_char->Attribute("x"), 0, 10);
					newChar.second.y = strtol(_char->Attribute("y"), 0, 10);
					newChar.second.width = strtol(_char->Attribute("width"), 0, 10);
					newChar.second.height = strtol(_char->Attribute("height"), 0, 10);
					newChar.second.xoffset = strtol(_char->Attribute("xoffset"), 0, 10);
					newChar.second.yoffset = strtol(_char->Attribute("yoffset"), 0, 10);
					newChar.second.xadvance = strtol(_char->Attribute("xadvance"), 0, 10);
					newChar.second.page = strtol(_char->Attribute("page"), 0, 10);

					newChar.second.size = glm::vec2(float(newChar.second.width) / float(mInfo.size), float(newChar.second.height) / float(mInfo.size));
					//            newChar.second.offset = glm::vec2(float(newChar.second.xoffset) / float(mInfo.size), float(mInfo.size - newChar.second.height - newChar.second.yoffset) / float(mInfo.size));
					newChar.second.offset = glm::vec2(float(newChar.second.xoffset) / float(mInfo.size), float(mCommon.base - newChar.second.height - newChar.second.yoffset) / float(mInfo.size));
					//            newChar.second.offset = glm::vec2(float(newChar.second.xoffset) / float(mInfo.size), float(-newChar.second.height - newChar.second.yoffset) / float(mInfo.size));
					newChar.second.advance = float(newChar.second.xadvance) / float(mInfo.size);

					glm::vec2 position = newChar.second.offset;
					glm::vec2 size = newChar.second.size;
					glm::vec2 texCoord(float(newChar.second.x) / float(mCommon.scaleW), float(mCommon.scaleH - newChar.second.y - newChar.second.height) / float(mCommon.scaleH));
					glm::vec2 texSize(float(newChar.second.width) / float(mCommon.scaleW), float(newChar.second.height) / float(mCommon.scaleH));

					newChar.second.index = charCount;

					positionTexCoords.push_back(position);
					positionTexCoords.push_back(texCoord);

					positionTexCoords.push_back(position + glm::vec2(size.x, 0.0f));
					positionTexCoords.push_back(texCoord + glm::vec2(texSize.x, 0.0f));

					positionTexCoords.push_back(position + glm::vec2(size.x, size.y));
					positionTexCoords.push_back(texCoord + glm::vec2(texSize.x, texSize.y));

					positionTexCoords.push_back(position + glm::vec2(0.0f, size.y));
					positionTexCoords.push_back(texCoord + glm::vec2(0.0f, texSize.y));

					int v0 = 4 * charCount;
					int v1 = v0 + 1;
					int v2 = v0 + 2;
					int v3 = v0 + 3;
					faces.push_back(glm::ivec3(v0, v1, v2));
					faces.push_back(glm::ivec3(v0, v2, v3));

					charCount++;

					if (newChar.first == ' ')
					{
						mSpaceAdvance = newChar.second.advance;
					}

					mChars.insert(newChar);

					_char = _char->NextSiblingElement("char");
				}

				if (!mPositionTexCoordBuffer)
				{
					glGenBuffers(1, &mPositionTexCoordBuffer);
				}

				glBindBuffer(GL_ARRAY_BUFFER, mPositionTexCoordBuffer);
				glBufferData(GL_ARRAY_BUFFER, mChars.size() * 16 * sizeof(float), &positionTexCoords[0], GL_STATIC_DRAW);

				if (!mFaceBuffer)
				{
					glGenBuffers(1, &mFaceBuffer);
				}

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mFaceBuffer);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, mChars.size() * 6 * sizeof(int), &faces[0], GL_STATIC_DRAW);

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			}
		}

		xml_in.Clear();

		mInitialized = true;
	}

	void Font::RenderText(Camera* camera, glm::vec2 position, const char* format, ...)
	{
		if (!mInitialized)
		{
			Initialize();
		}
		if (!mInitialized)
		{
			return;
		}

		if (!mInitializedShader)
		{
			InitializeShader();
		}
		if (!mInitializedShader)
		{
			return;
		}

		static char buffer[1048576];
		va_list argptr;
		va_start(argptr, format);
		vsprintf(buffer, format, argptr);
		va_end(argptr);

		if (!*buffer)
		{
			return;
		}

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);

		float screenHeight = camera->GetViewport()[3];
		float screenWidth = camera->GetAspect() * screenHeight;

		mMatrixStack->Top() = glm::ortho(0.0f, screenWidth, 0.0f, screenHeight) * glm::translate(glm::vec3(position.x, position.y, 1.0f)) * glm::scale(glm::vec3(mSize));

		for (char* key = buffer; *key; ++key)
		{
			if (*key == '\r' || *key == '\n')
			{
				mCurrentCursor.x = mCursor.x;
				mCurrentCursor.y -= mLineHeight;

				continue;
			}

			if (*key == ' ')
			{
				mCurrentCursor.x += mSpaceAdvance;

				continue;
			}

			if (*key == '\t')
			{
				float tab = mSpaceAdvancesPerTab * mSpaceAdvance;
				mCurrentCursor.x = mCursor.x + tab * floor(((mCurrentCursor.x - mCursor.x) + tab) / tab);

				continue;
			}

			std::map<int, Char>::iterator _char = mChars.find(*key);
			if (_char == mChars.end())
			{
				_char = mChars.find('?');
			}
			if (_char == mChars.end())
			{
				continue;
			}

			mMatrixStack->Push();
			mMatrixStack->Top() *= glm::translate(glm::vec3(mCurrentCursor.x, mCurrentCursor.y, 0.0f));

			glBindTexture(GL_TEXTURE_2D, mPages[_char->second.page].handle);

			mShader->Begin();
			mShader->EnableVertexAttribArray("aPositionTexCoord");

			mShader->Uniform(GL_FLOAT_MAT4, "uFTransform", 1, glm::value_ptr(mMatrixStack->Top()));
			mShader->Uniform(GL_SAMPLER_2D, "uTexture", 0);
			mShader->Uniform(GL_FLOAT_VEC4, "uColor", 1, glm::value_ptr(mColor));

			glBindBuffer(GL_ARRAY_BUFFER, mPositionTexCoordBuffer);
			mShader->VertexAttribPointer(GL_FLOAT_VEC4, "aPositionTexCoord", 0);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mFaceBuffer);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6 * _char->second.index * sizeof(unsigned int)));

			mShader->DisableVertexAttribArray("aPositionTexCoord");
			mShader->End();

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			mMatrixStack->Pop();

			mCurrentCursor.x += _char->second.advance;
		}

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glDisable(GL_ALPHA_TEST);
	}

	void Font::RenderText(Camera* camera, glm::vec3 position, const char* format, ...)
	{
		if (!mInitialized)
		{
			Initialize();
		}
		if (!mInitialized)
		{
			return;
		}

		if (!mInitializedShader)
		{
			InitializeShader();
		}
		if (!mInitializedShader)
		{
			return;
		}

		static char buffer[1048576];
		va_list argptr;
		va_start(argptr, format);
		vsprintf(buffer, format, argptr);
		va_end(argptr);

		if (!*buffer)
		{
			return;
		}

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);

		float f = camera->GetScaleFactor(position, mSize);
		mMatrixStack->Top() = camera->GetProjectionMatrix() * camera->GetFinalMatrix() * glm::translate(glm::vec3(position)) * glm::scale(glm::vec3(f)) * camera->GetBillboardMatrix();

		for (char* key = buffer; *key; ++key)
		{
			if (*key == '\r' || *key == '\n')
			{
				mCurrentCursor.x = mCursor.x;
				mCurrentCursor.y -= mLineHeight;

				continue;
			}

			if (*key == ' ')
			{
				mCurrentCursor.x += mSpaceAdvance;

				continue;
			}

			if (*key == '\t')
			{
				float tab = mSpaceAdvancesPerTab * mSpaceAdvance;
				mCurrentCursor.x = mCursor.x + tab * floor(((mCurrentCursor.x - mCursor.x) + tab) / tab);

				continue;
			}

			std::map<int, Char>::iterator _char = mChars.find(*key);
			if (_char == mChars.end())
			{
				_char = mChars.find('?');
			}
			if (_char == mChars.end())
			{
				continue;
			}

			mMatrixStack->Push();
			mMatrixStack->Top() *= glm::translate(glm::vec3(mCurrentCursor.x, mCurrentCursor.y, 0.0f));

			glBindTexture(GL_TEXTURE_2D, mPages[_char->second.page].handle);

			mShader->Begin();
			mShader->EnableVertexAttribArray("aPositionTexCoord");

			mShader->Uniform(GL_FLOAT_MAT4, "uFTransform", 1, glm::value_ptr(mMatrixStack->Top()));
			mShader->Uniform(GL_SAMPLER_2D, "uTexture", 0);
			mShader->Uniform(GL_FLOAT_VEC4, "uColor", 1, glm::value_ptr(mColor));

			glBindBuffer(GL_ARRAY_BUFFER, mPositionTexCoordBuffer);
			mShader->VertexAttribPointer(GL_FLOAT_VEC4, "aPositionTexCoord", 0);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mFaceBuffer);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6 * _char->second.index * sizeof(unsigned int)));

			mShader->DisableVertexAttribArray("aPositionTexCoord");
			mShader->End();

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			mMatrixStack->Pop();

			mCurrentCursor.x += _char->second.advance;
		}

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glDisable(GL_ALPHA_TEST);
	}

	glm::vec4 Font::GetTextLimits(const char* format, ...)
	{
		if (!mInitialized)
		{
			Initialize();
		}
		if (!mInitialized)
		{
			return glm::vec4(0.0f);
		}

		static char buffer[1048576];
		va_list argptr;
		va_start(argptr, format);
		vsprintf(buffer, format, argptr);
		va_end(argptr);

		float minX = 0.0f, minY = 0.0f, maxX = 0.0f, maxY = 0.0f;

		bool first = true;
		glm::vec2 cursor(0.0f);
		for (char* key = buffer; *key; ++key)
		{
			if (*key == '\r' || *key == '\n')
			{
				cursor.x = 0.0f;
				cursor.y -= mLineHeight;

				continue;
			}

			if (*key == ' ')
			{
				cursor.x += mSpaceAdvance;

				continue;
			}

			if (*key == '\t')
			{
				float tab = mSpaceAdvancesPerTab * mSpaceAdvance;
				cursor.x = tab * floor((cursor.x + tab) / tab);

				continue;
			}

			std::map<int, Font::Char>::iterator _char = mChars.find(*key);
			if (_char == mChars.end())
			{
				_char = mChars.find('?');
			}
			if (_char == mChars.end())
			{
				continue;
			}

			glm::vec2 p = _char->second.offset;
			glm::vec2 s = _char->second.size;

			if (first)
			{
				minX = cursor.x + p.x;
				minY = cursor.y + p.y;
				maxX = cursor.x + p.x + s.x;
				maxY = cursor.y + p.y + s.y;

				first = false;
			}
			else
			{
				if (cursor.x + p.x < minX)
				{
					minX = cursor.x + p.x;
				}
				if (cursor.y + p.y < minY)
				{
					minY = cursor.y + p.y;
				}
				if (cursor.x + p.x + s.x > maxX)
				{
					maxX = cursor.x + p.x + s.x;
				}
				if (cursor.y + p.y + s.y > maxY)
				{
					maxY = cursor.y + p.y + s.y;
				}
			}

			cursor.x += _char->second.advance;
		}

		return glm::vec4(minX, minY, maxX, maxY);
	}

	void Font::Clear(void)
	{
		mChars.clear();
		mChars.swap(std::map<int, Char>(mChars));

		for (std::vector<Page>::iterator page = mPages.begin(); page != mPages.end(); ++page)
		{
			glDeleteTextures(1, &page->handle);
		}

		mPages.clear();
		mPages.swap(std::vector<Page>(mPages));

		mInitialized = false;
	}

	void Font::SetSize(float size)
	{
		if (size <= 0.0f)
		{
			return;
		}

		mSize = size;
	}

	float Font::GetSize(void)
	{
		return mSize;
	}

	void Font::SetColor(glm::vec3 color)
	{
		mColor = glm::vec4(color, 1.0f);
	}

	void Font::SetColor(glm::vec4 color)
	{
		mColor = color;
	}

	glm::vec4 Font::GetColor(void)
	{
		return mColor;
	}

	void Font::SetCursor(glm::vec2 cursor)
	{
		mCursor = cursor;
		mCurrentCursor = mCursor;
	}

	glm::vec2 Font::GetCursor(void)
	{
		return mCurrentCursor;
	}

#include "default.c"

	void Font::Initialize(void)
	{
		mInfo.face = "default";
		mInfo.size = 8;

		mCommon.lineHeight = 8;
		mCommon.base = 7;
		mCommon.scaleW = 128;
		mCommon.scaleH = 128;

		mPages.push_back(Page());
		mPages.back().id = 0;
		mPages.back().file = "default";

		glGenTextures(1, &mPages.back().handle);

		if (!mPages.back().handle)
		{
			return;
		}

		glBindTexture(GL_TEXTURE_2D, mPages.back().handle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		glTexImage2D
			(
			GL_TEXTURE_2D,
			0,
			GL_LUMINANCE8_ALPHA8,
			128,
			128,
			0,
			GL_LUMINANCE_ALPHA,
			GL_UNSIGNED_BYTE,
			pixels
			);

		std::vector<glm::vec2> positionTexCoords;
		std::vector<glm::ivec3> faces;
		int charCount = 0;
		for (int y = 0; y < 16; ++y)
		{
			for (int x = 0; x < 16; ++x)
			{
				std::pair<int, Char> newChar;
				newChar.first = 16 * y + x;
				newChar.second.x = 8 * x;
				newChar.second.y = 8 * y;
				newChar.second.width = 8;
				newChar.second.height = 8;
				newChar.second.xoffset = 0;
				newChar.second.yoffset = 1;
				newChar.second.xadvance = 8;
				newChar.second.page = 0;

				newChar.second.size = glm::vec2(float(newChar.second.width) / float(mInfo.size), float(newChar.second.height) / float(mInfo.size));
				//         newChar.second.offset = glm::vec2(float(newChar.second.xoffset) / float(mInfo.size), float(mInfo.size - newChar.second.height - newChar.second.yoffset) / float(mInfo.size));
				newChar.second.offset = glm::vec2(float(newChar.second.xoffset) / float(mInfo.size), float(mCommon.base - newChar.second.height - newChar.second.yoffset) / float(mInfo.size));
				//         newChar.second.offset = glm::vec2(float(newChar.second.xoffset) / float(mInfo.size), float(-newChar.second.height - newChar.second.yoffset) / float(mInfo.size));
				newChar.second.advance = float(newChar.second.xadvance) / float(mInfo.size);

				glm::vec2 position = newChar.second.offset;
				glm::vec2 size = newChar.second.size;
				glm::vec2 texCoord(float(newChar.second.x) / float(mCommon.scaleW), float(mCommon.scaleH - newChar.second.y - newChar.second.height) / float(mCommon.scaleH));
				glm::vec2 texSize(float(newChar.second.width) / float(mCommon.scaleW), float(newChar.second.height) / float(mCommon.scaleH));

				newChar.second.index = charCount;

				positionTexCoords.push_back(position);
				positionTexCoords.push_back(texCoord);

				positionTexCoords.push_back(position + glm::vec2(size.x, 0.0f));
				positionTexCoords.push_back(texCoord + glm::vec2(texSize.x, 0.0f));

				positionTexCoords.push_back(position + glm::vec2(size.x, size.y));
				positionTexCoords.push_back(texCoord + glm::vec2(texSize.x, texSize.y));

				positionTexCoords.push_back(position + glm::vec2(0.0f, size.y));
				positionTexCoords.push_back(texCoord + glm::vec2(0.0f, texSize.y));

				int v0 = 4 * charCount;
				int v1 = v0 + 1;
				int v2 = v0 + 2;
				int v3 = v0 + 3;
				faces.push_back(glm::ivec3(v0, v1, v2));
				faces.push_back(glm::ivec3(v0, v2, v3));

				charCount++;

				mChars.insert(newChar);
			}
		}

		if (!mPositionTexCoordBuffer)
		{
			glGenBuffers(1, &mPositionTexCoordBuffer);
		}

		glBindBuffer(GL_ARRAY_BUFFER, mPositionTexCoordBuffer);
		glBufferData(GL_ARRAY_BUFFER, mChars.size() * 16 * sizeof(float), &positionTexCoords[0], GL_STATIC_DRAW);

		if (!mFaceBuffer)
		{
			glGenBuffers(1, &mFaceBuffer);
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mFaceBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mChars.size() * 6 * sizeof(int), &faces[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		mInitialized = true;
	}

	void Font::InitializeShader(void)
	{
		if (mInitializedShader)
		{
			return;
		}

		mShader->CompileSource("uniform mat4 uFTransform;attribute vec4 aPositionTexCoord;varying vec2 vTexCoord;void main(){vTexCoord = aPositionTexCoord.zw;gl_Position = uFTransform * vec4(aPositionTexCoord.xy, 0.0, 1.0);}", GL_VERTEX_SHADER);

		mShader->CompileSource("uniform sampler2D uTexture;uniform vec4 uColor;varying vec2 vTexCoord;void main(){float texture = texture2D(uTexture, vTexCoord).r;if(texture < 0.1){discard;}gl_FragColor =  uColor * texture;}", GL_FRAGMENT_SHADER);

		mShader->Link();

		mInitializedShader = true;
	}

}
