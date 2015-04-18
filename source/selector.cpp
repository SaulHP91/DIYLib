#include "diy\selector.hpp"
#include "diy\selectable.hpp"
#include "diy\camera.hpp"

#include <algorithm>

namespace diy
{

	Selector::Selector(void) :
		mSelected(0),
		mOldSelected(0)
	{
		;
	}

	Selector::~Selector(void)
	{
		ClearSelectables();
	}

	void Selector::SetCamera(Camera* camera)
	{
		mCamera = camera;
	}

	Camera* Selector::GetCamera(void)
	{
		return mCamera;
	}

	void Selector::Click(void)
	{
		if (!mSelected)
		{
			return;
		}

		if (!mSelected->mOnClick)
		{
			return;
		}

		mSelected->mOnClick(mSelected);
	}

	void Selector::DoubleClick(void)
	{
		if (!mSelected)
		{
			return;
		}

		if (!mSelected->mOnDoubleClick)
		{
			return;
		}

		mSelected->mOnDoubleClick(mSelected);
	}

	void Selector::MouseDown(glm::vec2 mousePos)
	{
		if (!mSelected)
		{
			return;
		}

		if (!mSelected->mOnMouseDown)
		{
			return;
		}

		mSelected->mOnMouseDown(mSelected, mousePos);
	}

	void Selector::MouseMove(glm::vec2 mousePos)
	{
		if (!mCamera)
		{
			return;
		}

		if (mSelectables.empty())
		{
			return;
		}

		if (mSelected)
		{
			if (mSelected->mOnMouseMove)
			{
				mSelected->mOnMouseMove(mSelected, mousePos);
			}
		}

		glm::vec3 origin;
		glm::vec3 direction;
		mCamera->GetRay(mousePos, origin, direction);

		float minDistance;
		mOldSelected = mSelected;
		mSelected = 0;
		for (std::vector<Selectable*>::iterator selectable = mSelectables.begin(); selectable != mSelectables.end(); ++selectable)
		{
			if (!(*selectable)->mEnabled)
			{
				continue;
			}

			if ((*selectable)->Pick(origin, direction))
			{
				if (mSelected)
				{
					float distance = mCamera->GetDistanceTo((*selectable)->mIntersection);
					if (distance < minDistance)
					{
						minDistance = distance;
						mSelected = *selectable;
					}
				}
				else
				{
					minDistance = mCamera->GetDistanceTo((*selectable)->mIntersection);
					mSelected = *selectable;
				}
			}
		}

		if (mSelected == mOldSelected)
		{
			return;
		}

		if (mSelected)
		{
			mSelected->mSelected = true;

			if (mSelected->mOnEnter)
			{
				mSelected->mOnEnter(mSelected);
			}
		}

		if (mOldSelected)
		{
			mOldSelected->mSelected = false;

			if (mOldSelected->mOnExit)
			{
				mOldSelected->mOnExit(mOldSelected);
			}
		}
	}

	void Selector::MouseUp(glm::vec2 mousePos)
	{
		if (!mSelected)
		{
			return;
		}

		if (!mSelected->mOnMouseUp)
		{
			return;
		}

		mSelected->mOnMouseUp(mSelected, mousePos);
	}

	void Selector::AddSelectable(Selectable* selectable)
	{
		std::vector<Selectable*>::iterator iter = find(mSelectables.begin(), mSelectables.end(), selectable);

		if (iter != mSelectables.end())
		{
			return;
		}

		mSelectables.push_back(selectable);
	}

	void Selector::DeleteSelectable(Selectable* selectable)
	{
		std::vector<Selectable*>::iterator iter = find(mSelectables.begin(), mSelectables.end(), selectable);

		if (iter == mSelectables.end())
		{
			return;
		}

		if ((*iter)->mSelector == this)
		{
			delete *iter;
		}

		mSelectables.erase(iter);
	}

	void Selector::ClearSelectables(void)
	{
		for (std::vector<Selectable*>::iterator selectable = mSelectables.begin(); selectable != mSelectables.end(); ++selectable)
		{
			if ((*selectable)->mSelector == this)
			{
				delete *selectable;
			}
		}

		mSelectables.clear();
		mSelectables.swap(std::vector<Selectable*>(mSelectables));
	}

	Selectable* Selector::GetSelected(void)
	{
		return mSelected;
	}

}
