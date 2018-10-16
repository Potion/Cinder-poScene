#include "MainViewController.h"

#include "cinder/app/App.h"

#include "poScene/ShapeView.h"
#include "poScene/TextView.h"

namespace sample
{
	ViewControllerRef ViewController::create()
	{
		return ViewControllerRef( new ViewController() );
	}

	ViewController::ViewController()
		: mPreviousSquare( nullptr )
	{
	}

	void ViewController::viewDidLoad()
	{
		//	Number of squares
		int numSquares = 5;

		//	Maximum square size
		float maxSize = 300.f;

		//	Create the container
		mContainer = View::create();
		getView()->addSubview( mContainer );

		//	Add the first square to the container
		//	Add subsequent squares to the previous square
		for( int i = 0; i < numSquares; i++ ) {
			int size = maxSize - ( i * ( maxSize / numSquares ) );
			SquareRef square = Square::create( size );

			if( mPreviousSquare != nullptr ) {
				mPreviousSquare->addSubview( square );
			}
			else {
				mContainer->addSubview( square );
			}

			mPreviousSquare = square;
		}

		//	Center everything
		mContainer->setPosition( ci::app::getWindowWidth() / 2, ci::app::getWindowHeight() / 2 );

		//	Text box with instructions
		ci::TextBox ciTextBox = ci::TextBox();
		ciTextBox.text( "Click and drag the squares to highlight subviews and alter their position within the hierarchy." );
		ciTextBox.size( 200, 200 );
		ciTextBox.font( ci::Font( "Arial", 12.f ) );
		ciTextBox.color( ci::Color( 1, 1, 1 ) );

		TextViewRef textBox = TextView::create( ciTextBox );
		textBox->setPosition( 20, 20 );
		getView()->addSubview( textBox );
	}
}
