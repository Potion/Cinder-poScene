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
		
	}
}}