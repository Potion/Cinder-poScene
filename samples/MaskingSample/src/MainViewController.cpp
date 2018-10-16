#include "MainViewController.h"

#include "cinder/app/App.h"
#include "cinder/ImageIo.h"

//	photo credit: <a href="http://www.flickr.com/photos/37539977@N00/600757415">slowly getting into the box</a> via <a href="http://photopin.com">photopin</a> <a href="https://creativecommons.org/licenses/by/2.0/">(license)</a>

using namespace po::scene;

namespace sample
{
	ViewControllerRef ViewController::create()
	{
		return ViewControllerRef( new ViewController() );
	}

	ViewController::ViewController()
	{
	}

	void ViewController::viewDidLoad()
	{
		//	Load the mask texture
		ci::gl::TextureRef maskTexture = ci::gl::Texture::create( ci::loadImage( ci::app::loadAsset( "circle_mask_blurred.png" ) ) );

		//	Create the mask shape
		//mMask = Shape::create(maskTexture);
		mMask = ShapeView::createRect( 100, 100 );
		mMask->setAlignment( Alignment::CENTER_CENTER );
		mMask->setPosition( ci::app::getWindowWidth() / 2, ci::app::getWindowHeight() / 2 );
		ci::app::timeline().apply( &mMask->getRotationAnim(), 0.0f, ci::toRadians( 360.0f ), 1.0f ).loop();
		ci::app::timeline().apply( &mMask->getScaleAnim(), ci::vec2( 1.0f, 1.0f ), ci::vec2( 4.0f, 4.0f ), 1.0f ).loop().pingPong();

		//	Load the image texture
		ci::gl::TextureRef texture = ci::gl::Texture::create( ci::loadImage( ci::app::loadAsset( "cat.jpg" ) ) );

		//	Create the image shape
		mImage = ImageView::create( texture );
		getView()->addSubview( mImage );

		//	Set the image mask
		getView()->setMask( mMask );

		// CB - test masking with alpha
		//setAlpha(0.5f);
		//mMask->setAlpha(0.5f);

		//	Connect mouse event
		getView()->getSignal( MouseEvent::MOVE ).connect( std::bind( &ViewController::onMouseMove, this, std::placeholders::_1 ) );
	}

	void ViewController::onMouseMove( po::scene::MouseEvent& event )
	{
		// Calculate the mask position based on the mouse
		ci::vec2 maskPos = event.getWindowPos();

		//	Set the mask position
		mMask->setPosition( maskPos );
	}
}
