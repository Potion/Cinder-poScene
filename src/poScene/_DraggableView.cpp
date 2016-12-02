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
	{}

	DraggableView::DraggableView(ci::vec2 snapBackPosition)
		: mIsDragging(false)
		, mDraggingEventID(DraggableView::DRAGGING_EVENT_ID_NONE)
		, mSnapsBack(true)
		, mSnapPosition(snapBackPosition)
	{
		setPosition(snapBackPosition);
	}

	void DraggableView::setup() {
		po::scene::ShapeViewRef rect = po::scene::ShapeView::createRect(50, 50);

		rect->setFillColor(ci::Color(0.0, 1.0, 0.0))
			.setAlignment(po::scene::Alignment::CENTER_CENTER);

		addChild(rect);

		connectEvents();
	}

	void DraggableView::connectEvents() {
		// Attach drag events
		getSignal(po::scene::MouseEvent::DOWN_INSIDE).connect(std::bind(&DraggableView::mouseEventHandler, this, std::placeholders::_1));
		getSignal(po::scene::MouseEvent::DRAG).connect(std::bind(&DraggableView::mouseEventHandler, this, std::placeholders::_1));
		getSignal(po::scene::MouseEvent::UP).connect(std::bind(&DraggableView::mouseEventHandler, this, std::placeholders::_1));
	}

	void DraggableView::mouseEventHandler(po::scene::MouseEvent &event) {
		switch(event.getType()) {
			// Start Dragging
			case po::scene::MouseEvent::DOWN_INSIDE:
				mIsDragging = true;
				mDragOffset = event.getLocalPos();
				mSignalDragBegan.emit(std::static_pointer_cast<DraggableView>(shared_from_this()));
				break;

			// Move if we are dragging
			case po::scene::MouseEvent::DRAG:
				if(mIsDragging) {
					po::scene::ViewRef parent = getParent();
					if(parent != nullptr) {
						setPosition(parent->windowToLocal(event.getWindowPos() - mDragOffset) ) ;
					}

					mSignalDragged.emit(std::static_pointer_cast<DraggableView>(shared_from_this()));
				}
				break;

			// Stop any dragging
			case po::scene::MouseEvent::UP:
				if(mIsDragging) {
					mIsDragging = false;
					mSignalDragEnded.emit(std::static_pointer_cast<DraggableView>(shared_from_this()));
				}
				break;
		}
	}

	void DraggableView::touchEventHandler(po::scene::TouchEvent &event) {
		switch (event.getType()) {
			// Start Dragging
		case po::scene::TouchEvent::BEGAN_INSIDE:
			mIsDragging = true;
			mDraggingEventID = event.getCiEvent().getId();
			mDragOffset = event.getLocalPos();

			mSignalDragBegan.emit(std::static_pointer_cast<DraggableView>(shared_from_this()));
			break;

			// Move if we are dragging
		case po::scene::TouchEvent::MOVED:
			if (mIsDragging && event.getCiEvent().getId() == mDraggingEventID) {
				po::scene::ViewRef parent = getParent();
				if (parent != nullptr) {
					setPosition(parent->windowToLocal(event.getWindowPos() - mDragOffset));
				}

				mSignalDragged.emit(std::static_pointer_cast<DraggableView>(shared_from_this()));
			}
			break;

			// Stop any dragging
		case po::scene::TouchEvent::ENDED:
			if (mIsDragging && event.getCiEvent().getId() == mDraggingEventID) {
				mIsDragging = false;
				mDraggingEventID = DRAGGING_EVENT_ID_NONE;
				mSignalDragEnded.emit(std::static_pointer_cast<DraggableView>(shared_from_this()));
			}
			break;
		}
	}
}}