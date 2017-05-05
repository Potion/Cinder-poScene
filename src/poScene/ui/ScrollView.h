#pragma once

#include "poScene/View.h"

namespace po
{
	namespace scene
	{
		namespace ui
		{
			class ScrollView;
			typedef std::shared_ptr<ScrollView> ScrollViewRef;

			class ScrollViewDelegate;
			typedef std::shared_ptr<ScrollViewDelegate> ScrollViewDelegateRef;

			class ScrollViewDelegate
			{

			};

			class ScrollView : public po::scene::View
			{
				public:
					static ScrollViewRef create();

					void setContentOffset( ci::vec2 offset );

					po::scene::ViewRef getContentView() { return mContentView; }
					ci::vec2 getContentOffset() { return mContentView->getPosition(); }

				protected:
					ScrollView();

					void setup();
					void update();

					po::scene::ViewRef mContentView;

					// Scrolling
					ci::vec2 getSnapPos( ci::vec2 pos );

					bool mHorizontalScrollingEnabled, mVerticalScrollingEnabled;

					int mTouchId;
					ci::vec2 mStartTouchPos, mCurTouchPos, mPrevTouchPos;
					bool mIsScrolling;
					ci::vec2 mScrollTargetPos;
					float mMaxAccel;
					float mDecel;

					// Event handlers
					void touchBeganInside( po::scene::TouchEvent& event );
					void touchMoved( po::scene::TouchEvent& event );
					void touchEnded( po::scene::TouchEvent& event );
			};
		}
	}
}