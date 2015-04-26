#include <gl/glew.h>

#include <diy/grid.hpp>
#include <diy/camera.hpp>

#include <glm/ext.hpp>

namespace diy
{

	Grid::Grid(void) :
		mMajorStep(1.0f),
		mMinorSteps(5),
		mMinorStep(0.2f),
		mVisible(true),
		mShowMinor(true),
		mSize(-5, -5, 5, 5),
		mMajorColor(0.5f, 0.5f, 0.5f),
		mMinorColor(0.8f, 0.8f, 0.8f),
		mMajorThickness(2),
		mMinorThickness(1)
	{
		;
	}

	Grid::~Grid(void)
	{
		;
	}

	void Grid::SetVisible(bool visible)
	{
		mVisible = visible;
	}

	bool Grid::GetVisible(void)
	{
		return mVisible;
	}

	void Grid::SetMajorStep(float majorStep)
	{
		if (majorStep > 0.0)
		{
			mMajorStep = majorStep;

			mMinorStep = mMajorStep / float(mMinorSteps + 1);
		}
	}

	float Grid::GetMajorStep(void)
	{
		return mMajorStep;
	}

	void Grid::SetMinorSteps(int minorSteps)
	{
		if (minorSteps > 0)
		{
			mMinorSteps = minorSteps;
		}

		mMinorStep = mMajorStep / float(mMinorSteps);
	}

	int Grid::GetMinorSteps(void)
	{
		return mMinorSteps;
	}

	void Grid::SetShowMinor(bool showMinor)
	{
		mShowMinor = showMinor;
	}

	bool Grid::GetShowMinor(void)
	{
		return mShowMinor;
	}

	void Grid::SetSize(glm::ivec4 size)
	{
		if (size[0] < size[2] && size[1] < size[3])
		{
			mSize = size;
		}
	}

	glm::ivec4 Grid::GetSize(void)
	{
		return mSize;
	}

	void Grid::SetMajorColor(glm::vec3 majorColor)
	{
		mMajorColor = majorColor;
	}

	glm::vec3 Grid::GetMajorColor(void)
	{
		return mMajorColor;
	}

	void Grid::SetMinorColor(glm::vec3 minorColor)
	{
		mMinorColor = minorColor;
	}

	glm::vec3 Grid::GetMinorColor(void)
	{
		return mMinorColor;
	}

	void Grid::SetMajorThickness(int majorThickness)
	{
		if (majorThickness > 0 && majorThickness >= mMinorThickness)
		{
			mMajorThickness = majorThickness;
		}
	}

	int Grid::GetMajorThickness(void)
	{
		return mMajorThickness;
	}

	void Grid::SetMinorThickness(int minorThickness)
	{
		if (minorThickness > 0 && minorThickness <= mMajorThickness)
		{
			mMinorThickness = minorThickness;
		}
	}

	int Grid::GetMinorThickness(void)
	{
		return mMinorThickness;
	}

	void Grid::Render(Camera* camera, glm::mat4 localMatrix)
	{
		if (!mVisible)
		{
			return;
		}

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadMatrixf(glm::value_ptr(camera->GetProjectionMatrix()));

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadMatrixf(glm::value_ptr(camera->GetFinalMatrix()));
		glMultMatrixf(glm::value_ptr(localMatrix));

		glColor3fv(glm::value_ptr(mMajorColor));
		glLineWidth(float(mMajorThickness));

		glBegin(GL_LINES);

		for (int i = mSize[0]; i < mSize[2]; ++i)
		{
			glVertex3f(float(i) * mMajorStep, 0.0f, mSize[1] * mMajorStep);
			glVertex3f(float(i) * mMajorStep, 0.0f, mSize[3] * mMajorStep);
		}
		glVertex3f(float(mSize[2]) * mMajorStep, 0.0f, mSize[1] * mMajorStep);
		glVertex3f(float(mSize[2]) * mMajorStep, 0.0f, mSize[3] * mMajorStep);

		for (int i = mSize[1]; i < mSize[3]; ++i)
		{
			glVertex3f(mSize[0] * mMajorStep, 0.0f, float(i) * mMajorStep);
			glVertex3f(mSize[2] * mMajorStep, 0.0f, float(i) * mMajorStep);
		}
		glVertex3f(mSize[0] * mMajorStep, 0.0f, float(mSize[3]) * mMajorStep);
		glVertex3f(mSize[2] * mMajorStep, 0.0f, float(mSize[3]) * mMajorStep);

		glEnd();

		glColor3fv(glm::value_ptr(mMinorColor));
		glLineWidth(float(mMinorThickness));

		glBegin(GL_LINES);

		for (int i = mSize[0]; i < mSize[2]; ++i)
		{
			for (int j = 1; j < mMinorSteps; ++j)
			{
				glVertex3f(float(i) * mMajorStep + float(j) * mMinorStep, 0.0f, mSize[1] * mMajorStep);
				glVertex3f(float(i) * mMajorStep + float(j) * mMinorStep, 0.0f, mSize[3] * mMajorStep);
			}
		}

		for (int i = mSize[1]; i < mSize[3]; ++i)
		{
			for (int j = 1; j < mMinorSteps; ++j)
			{
				glVertex3f(mSize[0] * mMajorStep, 0.0f, float(i) * mMajorStep + float(j) * mMinorStep);
				glVertex3f(mSize[2] * mMajorStep, 0.0f, float(i) * mMajorStep + float(j) * mMinorStep);
			}
		}

		glEnd();

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		glLineWidth(1.0f);
	}

}
