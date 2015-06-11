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
	//	Load the mask texture
	ci::gl::TextureRef maskTexture = ci::gl::Texture::create(ci::loadImage(ci::app::loadAsset("mask.jpg")));
	
	//	Create the mask shape
	mMask = Shape::create(maskTexture);
	
	//	Load the image texture
	ci::gl::TextureRef texture = ci::gl::Texture::create(ci::loadImage(ci::app::loadAsset("cat.jpg")));
	
	//	Create the image shape
	mImage = Image::create(texture);
	addChild(mImage);
	
	//	Set the image mask
	mImage->setMask(mMask);
	
	//	Connect mouse event
	getSignal(MouseEvent::MOVE_INSIDE).connect(std::bind(&MaskingSample::onMouseMove, this, std::placeholders::_1));
}

void MaskingSample::onMouseMove(po::scene::MouseEvent &event)
{
	// Calculate the mask position based on the mouse
	ci::Vec2f maskPos = ci::Vec2f(event.getWindowPos().x - (mMask->getWidth()/2), event.getWindowPos().y - (mMask->getHeight()/2));
	
	//	Set the mask position
	mMask->setPosition(maskPos);
}