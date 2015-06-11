#include "VideoSample.h"
#include "poShape.h"

using namespace po::scene;

VideoSampleRef VideoSample::create() 
{
    VideoSampleRef node(new VideoSample());
    node->setup();
    return node;
}

void VideoSample::setup() 
{
    //  create and add the video player
    //  this custom node container contains basic video operation within it
    mVideoPlayer = PlayerNode::create();
    mVideoPlayer->setAlignment(po::scene::Alignment::CENTER_CENTER);
    mVideoPlayer->setPosition(ci::app::getWindowCenter());
    addChild(mVideoPlayer);
    
    //  create and add the rotation arrows
    //  load button images as textures
    ci::gl::TextureRef clockwiseTex = ci::gl::Texture::create(loadImage(ci::app::loadAsset("spinArrowsClkws.png")));
    ci::gl::TextureRef counterClockwiseTex = ci::gl::Texture::create(loadImage(ci::app::loadAsset("spinArrowsXClkws.png")));
    
    //  create Shapes nodes with button textures
    ShapeRef rightButton = Shape::create(clockwiseTex);
    ShapeRef leftButton = Shape::create(counterClockwiseTex);
    
    //  get signals from button nodes
    rightButton->getSignal(MouseEvent::Type::DOWN_INSIDE).connect(std::bind(&VideoSample::rotateVideoPlayerClockwise, this));
    leftButton->getSignal(MouseEvent::Type::DOWN_INSIDE).connect(std::bind(&VideoSample::rotateVideoPlayerCounterClockwise, this));
    
    rightButton->setAlignment(po::scene::Alignment::CENTER_CENTER);
    leftButton->setAlignment(po::scene::Alignment::CENTER_CENTER);
    
    rightButton->setPosition(ci::app::getWindowWidth() * 0.9f, ci::app::getWindowHeight() / 2);
    leftButton->setPosition(ci::app::getWindowWidth() * 0.1f, ci::app::getWindowHeight() / 2);
    
    addChild(rightButton);
    addChild(leftButton);
    
}

void VideoSample::rotateVideoPlayerClockwise()
{
    simplifyVideoRotationValue();
    
    //  rotate half-circle
    float targetValue;
    float currentRot = mVideoPlayer->getRotation();
    if (currentRot < 89.f) {
        targetValue = 90.f;
    } else if (currentRot < 179.f) {
        targetValue = 180.f;
    } else if (currentRot < 269.f) {
        targetValue = 270.f;
    } else {
        targetValue = 360.f;
    }
    
    ci::app::timeline().apply(&mVideoPlayer->getRotationAnim(), targetValue, 1.0f);
}

void VideoSample::rotateVideoPlayerCounterClockwise()
{
    simplifyVideoRotationValue();
    
    //  rotate half-circle
    float targetValue;
    float currentRot = mVideoPlayer->getRotation();
    if (currentRot < 1.f) {
        targetValue = -90.f;
    } else if (currentRot < 91.f) {
        targetValue = 0.f;
    } else if (currentRot < 181.f) {
        targetValue = 90.f;
    } else if (currentRot < 271.f) {
        targetValue = 180.f;
    } else {
        targetValue = 270.f;
    }
    ci::app::timeline().apply(&mVideoPlayer->getRotationAnim(), targetValue, 1.0f);
}

//  bring rotation to range between 0 and 360
void VideoSample::simplifyVideoRotationValue()
{
    float rtn = mVideoPlayer->getRotation();
    
    if (rtn > 359.0f) {
        while (rtn > 359.f) {
            rtn -= 360.f;
        }
    } else if (rtn < -1.f) {
        while (rtn < -1.f) {
            rtn += 360.f;
        }
    }
    mVideoPlayer->setRotation(rtn);
}