#pragma once

#include "poScene/View.h"

#include "cinder/Signals.h"

namespace po { namespace scene {
	typedef std::shared_ptr<class DraggableView> DraggableViewRef;

	class DraggableView : public po::scene::View {
	public:
		typedef ci::signals::Signal<void(DraggableViewRef&)> DragAndDropSignal;

		static DraggableViewRef create();

		void setup() override;

		DragAndDropSignal &getSignalDragBegan()		{ return mSignalDragBegan; };
		DragAndDropSignal &getSignalDragged()		{ return mSignalDragged; };
		DragAndDropSignal &getSignalDragEnded()		{ return mSignalDragEnded; };

	protected:
		DraggableView();

	private:

		void mouseEventHandler(po::scene::MouseEvent &event);

		bool mIsDragging;
		ci::vec2 mDragOffset;

		// Signals
		DragAndDropSignal mSignalDragBegan, mSignalDragged, mSignalDragEnded;
	};
}}