#include "ViewController.h"

using namespace std::placeholders;

namespace po
{
	namespace scene
	{
		ViewControllerRef ViewController::create( )
		{
			return ViewControllerRef( new ViewController() );
		}

		ViewController::ViewController()
		{
			// Subscribe to update loop
			mUpdateConnection = ci::app::App::get()->getSignalUpdate().connect( std::bind( &ViewController::update, this ) );
		}

		ViewRef ViewController::getView()
		{
			if( mView == nullptr ) {
				loadView();
			}

			return mView;
		}

		void ViewController::loadView()
		{
			if( mView == nullptr ) {
				mView = po::scene::View::create();
			}

			connectViewEvents();
			viewDidLoad();
		}

		void ViewController::connectViewEvents()
		{
			mViewConnections.clear();

			mViewConnections += mView->getSignalWillLayoutSubviews().connect( [this]( ViewRef view ) {
				viewWillLayoutSubviews();
			} );

			mViewConnections += mView->getSignalDidLayoutSubviews().connect( [this]( ViewRef view ) {
				viewDidLayoutSubviews();
			} );

			mViewConnections += mView->getSignalWillAppear().connect( [this]( ViewRef view ) {
				viewWillAppear();
			} );
		}
	}
}