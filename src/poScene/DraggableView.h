#pragma once

#include "poScene/View.h"

#include "cinder/Signals.h"

namespace po { namespace scene {
	typedef std::shared_ptr<class DraggableView> DraggableViewRef;

	class DraggableView : public po::scene::View {
	public:
		typedef ci::signals::Signal<void(const ViewRef&)> DragAndDropSignal;

		static DraggableViewRef create();

		void setup() override;

		const DragAndDropSignal &getSignalDragStart() { return mSignalDragStart; };
		const DragAndDropSignal &getSignalDrag() { return mSignalDrag; };
		const DragAndDropSignal &getSignalDrop() { return mSignalDrop; };

	private:
		DraggableView();

		void mouseEventHandler(po::scene::MouseEvent &event);

		bool mIsDragging;
		ci::vec2 mDragOffset;

		// Signals
		DragAndDropSignal mSignalDragStart, mSignalDrag, mSignalDrop;
	};
}}