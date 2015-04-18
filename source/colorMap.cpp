#include <gl/glew.h>

#include <diy/colorMap.hpp>
#include <diy/camera.hpp>
#include <diy/font.hpp>

#include <glm/ext.hpp>

namespace diy
{

	ColorMap::ColorMap(void) :
		mMinimumValue(0.0f),
		mMaximumValue(1.0f),
		mStepCount(10),
		mTextColor(0.0f),
		mTitle(),
		mUnits(),
		mTextureHandle(0),
		mDirty(true)
	{
		SetGradient(Rainbow);
	}

	ColorMap::~ColorMap(void)
	{
		Clear();
	}

	void ColorMap::SetMinimumValue(float minimumValue)
	{
		mMinimumValue = minimumValue;

		if (mMinimumValue > mMaximumValue)
		{
			mMaximumValue = mMinimumValue;
		}
	}

	float ColorMap::GetMinimumValue(void)
	{
		return mMinimumValue;
	}

	void ColorMap::SetMaximumValue(float maximumValue)
	{
		mMaximumValue = maximumValue;

		if (mMaximumValue < mMinimumValue)
		{
			mMinimumValue = mMaximumValue;
		}
	}

	float ColorMap::GetMaximumValue(void)
	{
		return mMaximumValue;
	}

	void ColorMap::SetGradient(Gradient gradient)
	{
		switch (gradient)
		{
		case GrayScale:
		{
			mGradient.clear();

			mGradient.push_back(std::pair<float, glm::vec3>(0.0f, glm::vec3(0.0f, 0.0f, 0.0f)));
			mGradient.push_back(std::pair<float, glm::vec3>(1.0f, glm::vec3(1.0f, 1.0f, 1.0f)));
		}
		break;
		case Rainbow:
		{
			Clear();

			mGradient.push_back(std::pair<float, glm::vec3>(0.0f, glm::vec3(0.0f, 0.0f, 1.0f)));
			mGradient.push_back(std::pair<float, glm::vec3>(0.25f, glm::vec3(0.0f, 1.0f, 1.0f)));
			mGradient.push_back(std::pair<float, glm::vec3>(0.5f, glm::vec3(0.0f, 1.0f, 0.0f)));
			mGradient.push_back(std::pair<float, glm::vec3>(0.75f, glm::vec3(1.0f, 1.0f, 0.0f)));
			mGradient.push_back(std::pair<float, glm::vec3>(1.0f, glm::vec3(1.0f, 0.0f, 0.0f)));
		}
		break;
		case Altitude:
		{
			Clear();

			mGradient.push_back(std::pair<float, glm::vec3>(0.0f, glm::vec3(0.0f, 0.0f, 0.333f)));
			mGradient.push_back(std::pair<float, glm::vec3>(0.125f, glm::vec3(0.0f, 0.0f, 1.0f)));
			mGradient.push_back(std::pair<float, glm::vec3>(0.333f, glm::vec3(0.0f, 1.0f, 1.0f)));
			mGradient.push_back(std::pair<float, glm::vec3>(0.666f, glm::vec3(1.0f, 1.0f, 0.0f)));
			mGradient.push_back(std::pair<float, glm::vec3>(0.875f, glm::vec3(1.0f, 0.0f, 0.0f)));
			mGradient.push_back(std::pair<float, glm::vec3>(1.0f, glm::vec3(0.333f, 0.0f, 0.0f)));
		}
		break;
		case PH:
		{
			Clear();

			mGradient.push_back(std::pair<float, glm::vec3>(0.0f, glm::vec3(0.75f, 0.0f, 0.0f)));
			mGradient.push_back(std::pair<float, glm::vec3>(0.4f, glm::vec3(1.0f, 1.0f, 0.0f)));
			mGradient.push_back(std::pair<float, glm::vec3>(0.6f, glm::vec3(0.0f, 1.0f, 0.0f)));
			mGradient.push_back(std::pair<float, glm::vec3>(0.8f, glm::vec3(0.0f, 0.0f, 1.0f)));
			mGradient.push_back(std::pair<float, glm::vec3>(1.0f, glm::vec3(0.5f, 0.0f, 0.5f)));
		}
		break;
		case Heat:
		{
			Clear();

			mGradient.push_back(std::pair<float, glm::vec3>(0.0f, glm::vec3(0.0f, 0.0f, 0.0f)));
			mGradient.push_back(std::pair<float, glm::vec3>(0.2f, glm::vec3(1.0f, 0.0f, 0.0f)));
			mGradient.push_back(std::pair<float, glm::vec3>(0.5f, glm::vec3(1.0f, 0.5f, 0.0f)));
			mGradient.push_back(std::pair<float, glm::vec3>(0.8f, glm::vec3(1.0f, 1.0f, 0.0f)));
			mGradient.push_back(std::pair<float, glm::vec3>(1.0f, glm::vec3(1.0f, 1.0f, 1.0f)));
		}
		break;
		case ColdHot:
		{
			Clear();

			mGradient.push_back(std::pair<float, glm::vec3>(0.0f, glm::vec3(0.0f, 0.0f, 1.0f)));
			mGradient.push_back(std::pair<float, glm::vec3>(0.49f, glm::vec3(0.5f, 0.5f, 1.0f)));
			mGradient.push_back(std::pair<float, glm::vec3>(0.5f, glm::vec3(1.0f, 1.0f, 1.0f)));
			mGradient.push_back(std::pair<float, glm::vec3>(0.51f, glm::vec3(1.0f, 0.5f, 0.5f)));
			mGradient.push_back(std::pair<float, glm::vec3>(1.0f, glm::vec3(1.0f, 0.0f, 0.0f)));
		}
		break;
		case IceFire:
		{
			Clear();

			mGradient.push_back(std::pair<float, glm::vec3>(0.0f, glm::vec3(0.75f, 1.0f, 1.0f)));
			mGradient.push_back(std::pair<float, glm::vec3>(0.2f, glm::vec3(0.0f, 1.0f, 1.0f)));
			mGradient.push_back(std::pair<float, glm::vec3>(0.4f, glm::vec3(0.0f, 0.0f, 1.0f)));
			mGradient.push_back(std::pair<float, glm::vec3>(0.5f, glm::vec3(0.0f, 0.0f, 0.0f)));
			mGradient.push_back(std::pair<float, glm::vec3>(0.6f, glm::vec3(1.0f, 0.0f, 0.0f)));
			mGradient.push_back(std::pair<float, glm::vec3>(0.8f, glm::vec3(1.0f, 1.0f, 0.0f)));
			mGradient.push_back(std::pair<float, glm::vec3>(1.0f, glm::vec3(1.0f, 1.0f, 0.75f)));
		}
		break;
		case Zebra:
		{
			Clear();

			for (int i = 0; i < 2 * mStepCount + 1; ++i)
			{
				mGradient.push_back(std::pair<float, glm::vec3>(float(i) / float(mStepCount) / 2.0f, glm::vec3(float((i + 1) % 2))));
				mGradient.push_back(std::pair<float, glm::vec3>(float(i) / float(mStepCount) / 2.0f, glm::vec3(float(i % 2))));
			}
		}
		break;
		default:
		{
			;
		}
		}

		mDirty = true;
	}

	void ColorMap::SetGradient(std::vector<std::pair<float, glm::vec3> > gradient)
	{
		if (gradient.front().first != 0.0f && gradient.back().first != 1.0f)
		{
			return;
		}

		for (std::vector<std::pair<float, glm::vec3> >::iterator color = gradient.begin() + 1; color != gradient.end(); ++color)
		{
			if ((color - 1)->first > color->first)
			{
				return;
			}
		}

		mGradient = gradient;

		mDirty = true;
	}

	std::vector<std::pair<float, glm::vec3> > ColorMap::GetGradient(void)
	{
		return mGradient;
	}

	glm::vec3 ColorMap::GetColorAt(float value)
	{
		if (value < mMinimumValue)
		{
			value = mMinimumValue;
		}
		else if (value > mMaximumValue)
		{
			value = mMaximumValue;
		}

		float f = (value - mMinimumValue) / (mMaximumValue - mMinimumValue);
		for (std::vector<std::pair<float, glm::vec3> >::iterator color = mGradient.begin(); color != mGradient.end() - 1; ++color)
		{
			if ((color + 1)->first >= f)
			{
				return glm::lerp(color->second, (color + 1)->second, (f - color->first) / ((color + 1)->first - color->first));
			}
		}

		return mGradient.back().second;
	}

	void ColorMap::SetStepCount(int stepCount)
	{
		if (stepCount <= 0)
		{
			return;
		}

		mStepCount = stepCount;
	}

	int ColorMap::GetStepCount(void)
	{
		return mStepCount;
	}

	void ColorMap::SetTextColor(glm::vec3 textColor)
	{
		mTextColor = textColor;
	}

	glm::vec3 ColorMap::GetTextColor(void)
	{
		return mTextColor;
	}

	void ColorMap::SetTitle(std::string title)
	{
		mTitle = title;
	}

	std::string ColorMap::GetTitle(void)
	{
		return mTitle;
	}

	void ColorMap::SetUnits(std::string units)
	{
		mUnits = units;
	}

	std::string ColorMap::GetUnits(void)
	{
		return mUnits;
	}

	void ColorMap::Render(Camera* camera, Font* font)
	{
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();

		float screenHeight = camera->GetViewport()[3];
		float screenWidth = camera->GetAspect() * screenHeight;

		gluOrtho2D(0.0f, screenWidth, 0.0f, screenHeight);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		static const float barWidth = 24.0f;
		static const float barUpperGap = 96.0f;
		static const float barLowerGap = 48.0f;
		static const float barMargin = 128.0f;
		static const float textGap = 16.0f;

		float barBottom = barLowerGap;
		float barTop = screenHeight - barUpperGap;
		float barHeight = barTop - barBottom;
		float barLeft = screenWidth - barMargin - barWidth;
		float barRight = screenWidth - barMargin;

		glEnable(GL_TEXTURE_1D);

		glBindTexture(GL_TEXTURE_1D, GetTextureHandle());

		glBegin(GL_QUADS);
		glColor3f(1.0f, 1.0f, 1.0f);
		glTexCoord1f(0.0f);
		glVertex3f(barLeft, barBottom, 1.0f);
		glVertex3f(barRight, barBottom, 1.0f);
		glTexCoord1f(1.0f);
		glVertex3f(barRight, barBottom + barHeight, 1.0f);
		glVertex3f(barLeft, barBottom + barHeight, 1.0f);
		glEnd();

		glDisable(GL_TEXTURE_1D);

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		font->SetSize(18.0f);
		font->SetColor(mTextColor);

		glm::vec4 limits = font->GetTextLimits("%s\r%s", mTitle.c_str(), mUnits.c_str());
		font->SetCursor(glm::vec2(-limits[0], -limits[1] - (limits[3] - limits[1]) / 2.0f));
		font->RenderText(camera, glm::vec2(barLeft, barTop + barUpperGap / 2.0f), "%s\r%s", mTitle.c_str(), mUnits.c_str());

		font->SetSize(14.0f);

		limits = font->GetTextLimits("%.2E", mMinimumValue);
		font->SetCursor(glm::vec2(-limits[0], -limits[1] - (limits[3] - limits[1]) / 2.0f));
		font->RenderText(camera, glm::vec2(barRight + textGap, barBottom), "%.2E", mMinimumValue);

		float interval = 1.0f / float(mStepCount);
		float valueDelta = interval * (mMaximumValue - mMinimumValue);
		float barDelta = interval * barHeight;
		for (int i = 1; i < mStepCount; ++i)
		{
			font->SetCursor(glm::vec2(-limits[0], -limits[1] - (limits[3] - limits[1]) / 2.0f));
			font->RenderText(camera, glm::vec2(barRight + textGap, barBottom + float(i) * barDelta), "%.2E", mMinimumValue + float(i) * valueDelta);
		}

		font->SetCursor(glm::vec2(-limits[0], -limits[1] - (limits[3] - limits[1]) / 2.0f));
		font->RenderText(camera, glm::vec2(barRight + textGap, barTop), "%.2E", mMaximumValue);
	}

	void ColorMap::Clear(void)
	{
		mGradient.clear();
		mGradient.swap(std::vector<std::pair<float, glm::vec3> >(mGradient));
	}

#define COLORMAP_TEXTUREWIDTH 1024

	unsigned int ColorMap::GetTextureHandle(void)
	{
		if (mTextureHandle && !mDirty)
		{
			return mTextureHandle;
		}

		if (!mTextureHandle)
		{
			glGenTextures(1, &mTextureHandle);
		}

		if (mTextureHandle && mDirty)
		{
			unsigned char* pixels = new unsigned char[3 * COLORMAP_TEXTUREWIDTH];

			for (int i = 0; i < COLORMAP_TEXTUREWIDTH; ++i)
			{
				float f = float(i) / float(COLORMAP_TEXTUREWIDTH - 1);
				glm::vec3 rgb;
				bool assigned = false;

				for (std::vector<std::pair<float, glm::vec3> >::iterator color = mGradient.begin(); color != mGradient.end() - 1; ++color)
				{
					if ((color + 1)->first >= f)
					{
						float delta = (color + 1)->first - color->first;
						if (delta > 0.0f)
						{
							rgb = glm::lerp(color->second, (color + 1)->second, (f - color->first) / delta);
						}
						else
						{
							rgb = (color + 1)->second;
						}
						assigned = true;
						break;
					}
				}

				if (!assigned)
				{
					rgb = mGradient.back().second;
				}

				pixels[3 * i] = static_cast<unsigned char>(255.0f * rgb[0]);
				pixels[3 * i + 1] = static_cast<unsigned char>(255.0f * rgb[1]);
				pixels[3 * i + 2] = static_cast<unsigned char>(255.0f * rgb[2]);
			}

			glBindTexture(GL_TEXTURE_1D, mTextureHandle);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

			gluBuild1DMipmaps
				(
				GL_TEXTURE_1D,
				GL_RGB8,
				COLORMAP_TEXTUREWIDTH,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				pixels
				);
			delete[] pixels;

			mDirty = false;
		}

		return mTextureHandle;
	}

}
