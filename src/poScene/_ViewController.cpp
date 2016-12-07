#include "ViewController.h"

namespace po { namespace scene {
	ViewControllerRef ViewController::create() {
		return create(po::scene::View::create());
	}

	ViewControllerRef ViewController::create(ViewRef view) {
		ViewControllerRef ref(new ViewController(view));
		ref->setup();
		return ref;
	}

	ViewController::ViewController()
	: ViewController(po::scene::View::create()) 
	{}
	
	ViewController::ViewController(ViewRef view) 
	: mView(view) {
		// Subscribe to update loop
		mUpdateConnection = ci::app::App::get()->getSignalUpdate().connect(std::bind(&ViewController::update, this));
	}
}}