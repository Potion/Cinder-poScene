#include "DraggableView.h"

#include "cinder/app/App.h"
#include "poScene/ShapeView.h"

namespace po { namespace scene {
	// Drag And Drop View
	DraggableViewRef DraggableView::create() {
		DraggableViewRef ref(new DraggableView());
		ref->setup();
		return ref;
	}

	DraggableViewRef DraggableView::create(ci::vec2 snapBackPosition) {
		DraggableViewRef ref(new DraggableView(snapBackPosition));
		ref->setup();
		return ref;
	}

	DraggableView::DraggableView()
		: mIsDragging(false)
		, mDraggingEventID(DraggableView::DRAGGING_EVENT_ID_NONE)
		, mSnapsBack(false)
	{
		connectEvents();
	}

	DraggableView::DraggableView(ci::vec2 snapBackPosition)
		: mIsDragging(false)
		, mDraggingEventID(DraggableView::DRAGGING_EVENT_ID_NONE)
		, mSnapsBack(true)
		, mSnapPosition(snapBackPosition)
	{
		setPosition( snapBackPosition );
		connectEvents();
	}

	void DraggableView::setup() {}

	void DraggableView::setSnapBackPosition(ci::vec2 snapBackPosition) {
		mSnapPosition = snapBackPosition;
		mSnapsBack = true;
	}

	void DraggableView::connectEvents() {
		// Attach drag events
		mConnections += getSignal(po::scene::MouseEvent::DOWN_INSIDE).connect(std::bind(&DraggableView::mouseEventHandler, this, std::placeholders::_1));
		mConnections += getSignal(po::scene::MouseEvent::DRAG).connect(std::bind(&DraggableView::mouseEventHandler, this, std::placeholders::_1));
		mConnections += getSignal(po::scene::MouseEvent::UP).connect(std::bind(&DraggableView::mouseEventHandler, this, std::placeholders::_1));
	}

	void DraggableView::mouseEventHandler(po::scene::MouseEvent &event) {
		switch(event.getType()) {
			// Start Dragging
			case po::scene::MouseEvent::DOWN_INSIDE: {
				mIsDragging = true;
				mPrevDragPosition = event.getWindowPos();
				DraggableViewRef ref = std::static_pointer_cast<DraggableView>(shared_from_this());
				mSignalDragBegan.emit(ref);
				break;
			}

			// Move if we are dragging
			case po::scene::MouseEvent::DRAG:
				if(mIsDragging) {
					po::scene::ViewRef parent = getParent();
					if(parent != nullptr) {
						setPosition( getPosition() + ( event.getWindowPos() - mPrevDragPosition ) );
						mPrevDragPosition = event.getWindowPos();
					}
					DraggableViewRef ref = std::static_pointer_cast<DraggableView>(shared_from_this());
					mSignalDragged.emit(ref);
				}
				break;

			// Stop any dragging
			case po::scene::MouseEvent::UP:
				if(mIsDragging) {
					mIsDragging = false;
					DraggableViewRef ref = std::static_pointer_cast<DraggableView>(shared_from_this());
					mSignalDragEnded.emit(ref);
				}
				break;
		}
	}

	void DraggableView::touchEventHandler(po::scene::TouchEvent &event) {
		switch (event.getType()) {
			// Start Dragging
			case po::scene::TouchEvent::BEGAN_INSIDE: {
				if(mIsDragging) return;
				mIsDragging = true;
				mDraggingEventID = event.getCiEvent().getId();
				mPrevDragPosition = event.getWindowPos();
				DraggableViewRef ref = std::static_pointer_cast<DraggableView>(shared_from_this());
				mSignalDragBegan.emit(ref);
				break;
			}

			// Move if we are dragging
		case po::scene::TouchEvent::MOVED:
			if (mIsDragging && event.getCiEvent().getId() == mDraggingEventID) {
				po::scene::ViewRef parent = getParent();
				if (parent != nullptr) {
					setPosition(getPosition() + (event.getWindowPos() - mPrevDragPosition));
					mPrevDragPosition = event.getWindowPos();
				}
				DraggableViewRef ref = std::static_pointer_cast<DraggableView>(shared_from_this());
				mSignalDragged.emit(ref);
			}
			break;

			// Stop any dragging
		case po::scene::TouchEvent::ENDED:
			if (mIsDragging && event.getCiEvent().getId() == mDraggingEventID) {
				mIsDragging = false;
				mDraggingEventID = DRAGGING_EVENT_ID_NONE;
				DraggableViewRef ref = std::static_pointer_cast<DraggableView>(shared_from_this());
				mSignalDragEnded.emit(ref);
			}
			break;
		}
	}
}}
