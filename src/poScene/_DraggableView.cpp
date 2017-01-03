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
		, mDraggingEventId(DraggableView::DRAGGING_EVENT_ID_NONE)
		, mSnapsBack(false)
	{
		connectEvents();
	}

	DraggableView::DraggableView(ci::vec2 snapBackPosition)
		: mIsDragging(false)
		, mDraggingEventId(DraggableView::DRAGGING_EVENT_ID_NONE)
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

	void DraggableView::handleDragStartEvent(ci::vec2 localPos, ci::vec2 windowPos, int eventId) {
		mPrevDragPosition = windowPos;
		mDraggingEventId = eventId;

		DraggableViewRef ref = std::static_pointer_cast<DraggableView>(shared_from_this());
		mSignalDragBegan.emit(ref);

		mIsDragging = true;
	}

	void DraggableView::handleDragEvent(ci::vec2 localPos, ci::vec2 windowPos, int eventId) {
		if (mIsDragging && mDraggingEventId == eventId) {
			setPosition(getPosition() + (windowPos - mPrevDragPosition));
			mPrevDragPosition = windowPos;

			DraggableViewRef ref = std::static_pointer_cast<DraggableView>(shared_from_this());
			mSignalDragged.emit(ref);
		}
	}

	void DraggableView::handleDragEndEvent(ci::vec2 localPos, ci::vec2 windowPos, int eventId) {
		if (mIsDragging) {
			mIsDragging = false;

			DraggableViewRef ref = std::static_pointer_cast<DraggableView>(shared_from_this());
			mSignalDragEnded.emit(ref);
		}
	}

	void DraggableView::mouseEventHandler(po::scene::MouseEvent &event) {
		switch(event.getType()) {
			// Start Dragging
			case po::scene::MouseEvent::DOWN_INSIDE:
				handleDragStartEvent(event.getLocalPos(), event.getWindowPos(), 0);
				break;

			// Move if we are dragging
			case po::scene::MouseEvent::DRAG:
				handleDragEvent(event.getLocalPos(), event.getWindowPos(), 0);
				break;

			// Stop any dragging
			case po::scene::MouseEvent::UP:
				handleDragEndEvent(event.getLocalPos(), event.getWindowPos(), 0);
				break;
		}
	}

	void DraggableView::touchEventHandler(po::scene::TouchEvent &event) {
		switch (event.getType()) {
			// Start Dragging
			case po::scene::TouchEvent::BEGAN_INSIDE: 
				handleDragStartEvent(event.getLocalPos(), event.getWindowPos(), event.getId());
				break;

			// Move if we are dragging
			case po::scene::TouchEvent::MOVED:
				handleDragEvent(event.getLocalPos(), event.getWindowPos(), event.getId());
				break;

			// Stop any dragging
			case po::scene::TouchEvent::ENDED:
				handleDragEndEvent(event.getLocalPos(), event.getWindowPos(), event.getId());
				break;
		}
	}
}}
