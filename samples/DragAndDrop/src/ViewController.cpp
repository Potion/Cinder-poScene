#include "ViewController.h"

#include "cinder/app/App.h"

namespace sample {
	ViewControllerRef ViewController::create() {
		ViewControllerRef ref(new ViewController());
		ref->setup();
		return ref;
	}

	ViewController::ViewController()
	{
	}

	void ViewController::setup() {
		mDragAndDropVC = po::scene::DragAndDropViewController::create();

		mDragAndDropVC->getSignalViewAddedToDropZone().connect(std::bind(&ViewController::onViewAddedToDragZone, this, std::placeholders::_1, std::placeholders::_2));
		
		for(int i=0; i<5; i++) {
			
			po::scene::DropZoneViewRef dropZoneView = po::scene::DropZoneView::create();
			dropZoneView->setPosition(ci::app::getWindowWidth() * 0.75, 75 + i * 75);
			mDragAndDropVC->trackDropZoneView(dropZoneView);
			
			ci::vec2 pos(ci::app::getWindowWidth() * 0.25, 75 + i * 75);
			po::scene::DraggableViewRef dragView = po::scene::DraggableView::create(pos);
			mDragAndDropVC->trackDraggableView(dragView);
		}

		getView()->addChild( mDragAndDropVC->getView() );
		
		//ci::app::console() << "\n---------------\nDropZoneViews: \n";
		for(auto &view : mDragAndDropVC->getDropZoneViews()) {
			mDragAndDropVC->getView()->addChild(view);
			po::scene::ShapeViewRef shapeDrop = po::scene::ShapeView::createRect(30,30);
			shapeDrop->setAlignment(po::scene::Alignment::CENTER_CENTER);
			shapeDrop->setFillColor(ci::Color(1,0,0.2));;
			view->getBackgroundView()->addChild( shapeDrop );
			view->setBoundsColor(ci::Color(1,1,1));
			view->setDrawBounds(true);
			//ci::app::console() << "frame: " << view->getFrame() <<" bounds: " << view->getBounds() << " " << view->isInteractionEnabled()<< std::endl;
		}
		
		//ci::app::console() << "\n---------------\nDraggableViews: \n";
		for(auto &view : mDragAndDropVC->getDraggableViews() ) {
			mDragAndDropVC->getView()->addChild(view);
			po::scene::ShapeViewRef shapeDrag = po::scene::ShapeView::createRect(20,20);
			shapeDrag->setAlignment(po::scene::Alignment::CENTER_CENTER);
			shapeDrag->setFillColor(ci::ColorA(0,1,1,.5));
			view->addChild( shapeDrag );
			
			view->setBoundsColor(ci::Color(1,0,0));
			view->setDrawBounds(true);
			//ci::app::console() << "frame: " << view->getFrame() <<" bounds: " << view->getBounds() << std::endl;
		}
		
	}
	
	void ViewController::onViewAddedToDragZone( po::scene::DropZoneViewRef dropRef, po::scene::DraggableViewRef dragRef )
	{
		dragRef->setPosition(ci::vec2());
	}
	
}
