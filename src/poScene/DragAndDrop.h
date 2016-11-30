#pragma once

#include "poScene/View.h"
#include "poScene/DraggableView.h"

namespace po { namespace scene {


typedef std::shared_ptr<class DragAndDropView> DragAndDropViewRef;
class DragAndDropView : public DraggableView {
public:
	static DragAndDropViewRef create();
	static DragAndDropViewRef create(ci::vec2 snapBackPosition);

	void snapBackToPosition();

protected:
	DragAndDropView();
	DragAndDropView(ci::vec2 snapBackPosition);

private:
	bool mSnapsBack;
	ci::vec2 mSnapPosition;
};


typedef std::shared_ptr<class DropZoneView> DropZoneViewRef;
class DropZoneView : public View {
public:
	static DropZoneViewRef create();

	virtual void setHighlighted(bool highlighted=true);

	virtual bool addView(DraggableViewRef view);

protected:

private:
	DropZoneView();
	void setup();

	bool mIsHighlighted;
	ViewRef mBackgroundView;
	ViewRef mDraggableViews;
};
}}


//
//namespace po { namespace scene {
//	typedef std::shared_ptr<class DragAndDropViewController> DragAndDropViewControllerRef;
//	typedef std::shared_ptr<class DropZoneView> DropZoneViewRef;
//
//	struct DragAndDropSet {
//		std::vector<DraggableViewRef> draggableViews;
//		std::vector<DropZoneViewRef> dropZoneViews;
//
//		std::map<ViewRef, std::vector<ci::signals::ScopedConnection> > connections;
//	};
//
//	class DragAndDropViewController : public ViewController {
//	public:
//		static DragAndDropViewControllerRef create();
//
//		void setup();
//		void reset();
//
//		void trackDraggableView(DraggableViewRef view, int setID=0);
//		void trackDropZoneView(DropZoneViewRef view, int setID=0);
//
//	protected:
//		void viewBeganDraggingHandler(DraggableViewRef &view);
//		void viewDraggedHandler(DraggableViewRef &view);
//		void viewFinishedDraggingHandler(DraggableViewRef &view);
//
//	private:
//		DragAndDropViewController();
//
//		std::map<int, DragAndDropSet> mSets;
//	};
//
//	typedef ci::signals::Signal<void(DraggableViewRef &)> DropZoneViewSignal;
//	class DropZoneView : public View {
//	public:
//		static DropZoneViewRef create();
//
//		bool hasView() { return mDraggableView != nullptr; };
//
//		bool hitTest(DraggableViewRef draggableView);
//		void addView(DraggableViewRef draggableView);
//
//		void showHighlighted(bool highlighted=true);
//
//	protected:
//
//	private:
//		DropZoneView();
//
//		void setup();
//
//		bool mIsHighlighted;
//
//		DraggableViewRef mDraggableView;
//	};
//}}