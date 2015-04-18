#include <gl/glew.h>

#include <diy/tag.hpp>
#include <diy/camera.hpp>
#include <diy/font.hpp>

#include <glm/ext.hpp>

namespace diy
{

	Tag::Tag(std::string text) :
		mTagCloud(0),
		mText(text),
		mCenter(0.0f),
		mTextColor(0.0f),
		mLineColor(0.0f),
		mData(0)
	{
		;
	}

	Tag::~Tag(void)
	{
		Clear();
	}

	void Tag::SetText(std::string text)
	{
		mText = text;
	}

	std::string Tag::GetText(void)
	{
		return mText;
	}

	void Tag::SetCenter(glm::vec3 center)
	{
		mCenter = center;
	}

	glm::vec3 Tag::GetCenter(void)
	{
		return mCenter;
	}

	void Tag::SetTextColor(glm::vec3 textColor)
	{
		mTextColor = textColor;
	}

	glm::vec3 Tag::GetTextColor(void)
	{
		return mTextColor;
	}

	void Tag::SetLineColor(glm::vec3 lineColor)
	{
		mLineColor = lineColor;
	}

	glm::vec3 Tag::GetLineColor(void)
	{
		return mLineColor;
	}

	void Tag::SetData(void* data)
	{
		mData = data;
	}

	void* Tag::GetData(void)
	{
		return mData;
	}

	void Tag::Render(Camera* camera, Font* font, glm::vec3 center)
	{
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadMatrixf(glm::value_ptr(camera->GetProjectionMatrix()));

		glm::vec3 p = mCenter;

		glm::vec3 dir = glm::vec3(camera->GetInverseFinalMatrix() * (camera->GetFinalMatrix() * glm::vec4(p, 1.0f) - camera->GetFinalMatrix() * glm::vec4(center, 1.0f)));
		float dirLength = glm::length(dir);
		if (dirLength > 0.0f)
		{
			dir /= dirLength;
		}
		else
		{
			dir = glm::normalize(glm::vec3(camera->GetInverseFinalMatrix() * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f)));
		}
		glm::vec3 dir1 = glm::normalize(glm::vec3(camera->GetFinalMatrix() * glm::vec4(dir, 0.0f)));
		glm::vec3 dir2 = glm::normalize(glm::vec3(camera->GetInverseFinalMatrix() * glm::vec4(dir1.x > 0.0f ? 1.0f : -1.0f, 0.0f, 0.0f, 0.0f)));
		float f = camera->GetScaleFactor(p, 100.0f);
		glm::vec3 q = p + f * dir;
		float g = camera->GetScaleFactor(p, 100.0f);
		glm::vec3 r = q + g * dir2;

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadMatrixf(glm::value_ptr(camera->GetFinalMatrix()));

		glBegin(GL_LINE_STRIP);
		glColor3fv(glm::value_ptr(mLineColor));
		glVertex3fv(glm::value_ptr(p));
		glVertex3fv(glm::value_ptr(q));
		glVertex3fv(glm::value_ptr(r));
		glEnd();

		glPopMatrix();

		font->SetColor(mTextColor);

		glm::vec4 limits = font->GetTextLimits("%s", mText.c_str());
		if (dir1.x > 0.0f)
		{
			font->SetCursor(glm::vec2(-limits[0], -limits[1] - (limits[3] - limits[1]) / 2.0f));
		}
		else
		{
			font->SetCursor(glm::vec2(-limits[2], -limits[1] - (limits[3] - limits[1]) / 2.0f));
		}

		font->RenderText(camera, glm::vec3(r.x, r.y, r.z), "%s", mText.c_str());

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
	}

	void Tag::Clear(void)
	{
		mText.clear();

		mData = 0;
	}

}
