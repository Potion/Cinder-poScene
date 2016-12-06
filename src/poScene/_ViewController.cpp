#include "ViewController.h"

namespace po { namespace scene {
	ViewControllerRef ViewController::create() {
		ViewControllerRef ref(new ViewController());
		ref->setup();
		return ref;
	}

	ViewController::ViewController()
	: mView(po::scene::View::create()) 
	{
		// Subscribe to update loop
		ci::app::App::get()->getSignalUpdate().connect(std::bind(&ViewController::update, this));
	}
}}