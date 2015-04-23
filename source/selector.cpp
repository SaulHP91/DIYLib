#include <diy/Selector.hpp>
#include <diy/Selectable.hpp>
#include <diy/Camera.hpp>

#include <algorithm>

namespace diy
{

	Selector::Selector(void) :
		mEnabled(true),
		mCamera(0),
		mSelected(0),
		mOldSelected(0),
		mMouseDown(false),
		mDrag(false)
	{
		;
	}

	Selector::~Selector(void)
	{
		Clear();
	}

	void Selector::SetEnabled(bool enabled)
	{
		mEnabled = true;
	}

	bool Selector::GetEnabled(void)
	{
		return mEnabled;
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
		if (!mEnabled)
		{
			return;
		}

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
		if (!mEnabled)
		{
			return;
		}

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
		if (!mEnabled)
		{
			return;
		}

		if (!mSelected)
		{
			return;
		}

		if (!mSelected->mOnMouseDown)
		{
			return;
		}

		mSelected->mOnMouseDown(mSelected, mousePos);

		mMouseDown = true;
	}

	void Selector::MouseMove(glm::vec2 mousePos)
	{
		if (!mEnabled)
		{
			return;
		}

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
			if (mMouseDown && !mDrag && mSelected->mOnEnterDrag)
			{
				mDrag = mSelected->mOnEnterDrag(mSelected, mousePos);
			}

			if (mDrag)
			{
				if (mSelected->mOnDragMove)
				{
					mDrag = mSelected->mOnDragMove(mSelected, mousePos);
				}
			}

			if (mDrag)
			{
				return;
			}

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

			if ((*selectable)->Intersect(origin, direction))
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

			mMouseDown = false;
			mDrag = false;
		}
	}

	void Selector::MouseUp(glm::vec2 mousePos)
	{
		if (!mEnabled)
		{
			return;
		}

		if (!mSelected)
		{
			return;
		}

		if (mDrag && mSelected->mOnDrop)
		{
			mSelected->mOnDrop(mSelected, mousePos);
		}

		if (mSelected->mOnMouseUp)
		{
			mSelected->mOnMouseUp(mSelected, mousePos);
		}

		mMouseDown = false;
		mDrag = false;
	}

	void Selector::Add(Selectable* selectable)
	{
		selectable->mSelector = this;
		mSelectables.push_back(selectable);
	}

	void Selector::Delete(Selectable* selectable)
	{
		std::vector<Selectable*>::iterator iter = find(mSelectables.begin(), mSelectables.end(), selectable);

		if (iter == mSelectables.end())
		{
			return;
		}

		if (*iter == mOldSelected)
		{
			mOldSelected = nullptr;
		}

		if (*iter == mSelected)
		{
			mSelected = nullptr;
		}

		if ((*iter)->mCreator == this)
		{
			delete *iter;
		}

		mSelectables.erase(iter);
	}

	void Selector::Clear(void)
	{
		for (std::vector<Selectable*>::iterator selectable = mSelectables.begin(); selectable != mSelectables.end(); ++selectable)
		{
			if ((*selectable)->mCreator == this)
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
