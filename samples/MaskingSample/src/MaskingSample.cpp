#include "MaskingSample.h"

//	photo credit: <a href="http://www.flickr.com/photos/37539977@N00/600757415">slowly getting into the box</a> via <a href="http://photopin.com">photopin</a> <a href="https://creativecommons.org/licenses/by/2.0/">(license)</a>

MaskingSampleRef MaskingSample::create() 
{
    MaskingSampleRef node(new MaskingSample());
    node->setup();
    return node;
}

MaskingSample::MaskingSample()
{
}

void MaskingSample::setup() 
{
	ci::app::getWindow()->connectKeyUp(&MaskingSample::keyUp, this);
	
	//	Load the mask texture
	ci::gl::TextureRef maskTexture = ci::gl::Texture::create(ci::loadImage(ci::app::loadAsset("circle_mask_blurred.jpg")));
	
	//	Create the mask shape
	//mMask = Shape::create(maskTexture);
    mMask = Shape::createRect(100, 100);
    mMask->setAlignment(Alignment::CENTER_CENTER);
    mMask->setPosition(ci::app::getWindowWidth()/2, ci::app::getWindowHeight()/2);
    ci::app::timeline().apply(&mMask->getRotationAnim(), 0.0f, 360.0f, 1.0f).loop();
    ci::app::timeline().apply(&mMask->getScaleAnim(), ci::Vec2f(1.0f, 1.0f), ci::Vec2f(4.0f, 4.0f), 1.0f).loop().pingPong();
    ci::app::timeline().apply(&mMask->getAlphaAnim(), 0.5f, 1.0f, 5.0f).loop().pingPong();
    
	//	Load the image texture
	ci::gl::TextureRef texture = ci::gl::Texture::create(ci::loadImage(ci::app::loadAsset("cat.jpg")));
	
	//	Create the image shape
	mImage = Image::create(texture);
	addChild(mImage);
	
	//	Set the image mask
	setMask(mMask);
	
	//	Connect mouse event
	getSignal(MouseEvent::MOVE).connect(std::bind(&MaskingSample::onMouseMove, this, std::placeholders::_1));
}

void MaskingSample::onMouseMove(po::scene::MouseEvent &event)
{
	// Calculate the mask position based on the mouse
	ci::Vec2f maskPos = event.getWindowPos();
	
	//	Set the mask position
	//mMask->setPosition(maskPos);
    
    std::cout << maskPos << std::endl;
}

void MaskingSample::keyUp(ci::app::KeyEvent &event)
{
	ci::app::console() << event.getChar() << std::endl;
}