#include "AnimationSample.h"

using namespace po::scene;

AnimationSampleRef AnimationSample::create() 
{
    AnimationSampleRef node(new AnimationSample());
    node->setup();
    return node;
}

void AnimationSample::setup() 
{
    // for offsets, use dimensions of AnimationSquares, which are 100 x 100
    float xInterval = ci::app::getWindowWidth() / 6.0f;
    float xOffset = (xInterval - 100.f) / 2;
    float yOffset = (ci::app::getWindowHeight() / 2.0f) - 50.f;
    
    //  add individual AnimationSquares; a mouse click on each one will call only one specific method
    
    //  create and add node that animates position
    mPositionSquare = AnimationSquare::create("PositionAnim", ci::Color(255.f/255, 147.f/255, 30.f/255));
    mPositionSquare->setPosition(ci::vec2(xOffset, yOffset));
    mPositionSquare->getSignal(MouseEvent::DOWN_INSIDE).connect(std::bind(&AnimationSquare::doPositionAnimation, mPositionSquare));
    addChild(mPositionSquare);

    //  create and add node that animates scale
    mScaleSquare = AnimationSquare::create("ScaleAnim", ci::Color(122.f/255, 201.f/255, 67.f/255));
    mScaleSquare->setPosition(ci::vec2(xInterval + xOffset, yOffset));
    mScaleSquare->getSignal(MouseEvent::DOWN_INSIDE).connect(std::bind(&AnimationSquare::doScaleAnimation, mScaleSquare));
    mScaleSquare->useBaseShapeCenter();
    addChild(mScaleSquare);
    
    //  create and add node that animates alpha
    mAlphaSquare = AnimationSquare::create("AlphaAnim", ci::Color(189.f/255, 204.f/255, 212.f/255));
    mAlphaSquare->setPosition(ci::vec2((xInterval * 2) + xOffset, yOffset));
    mAlphaSquare->getSignal(MouseEvent::DOWN_INSIDE).connect(std::bind(&AnimationSquare::doAlphaAnimation, mAlphaSquare));
    addChild(mAlphaSquare);

    //  create and add node that animates rotation
    mRotationSquare = AnimationSquare::create("RotationAnim", ci::Color(63.f/255, 169.f/255, 245.f/255));
    mRotationSquare->setPosition(ci::vec2((xInterval * 3) + xOffset, yOffset));
    mRotationSquare->getSignal(MouseEvent::DOWN_INSIDE).connect(std::bind(&AnimationSquare::doRotationAnimation, mRotationSquare));
    mRotationSquare->useBaseShapeCenter();
    addChild(mRotationSquare);

    //  create and add node that animates offset
    mOffsetSquare = AnimationSquare::create("OffsetAnim", ci::Color(122.f/255, 201.f/255, 67.f/255));
    mOffsetSquare->setPosition(ci::vec2((xInterval * 4) + xOffset, yOffset));
    mOffsetSquare->getSignal(MouseEvent::DOWN_INSIDE).connect(std::bind(&AnimationSquare::doOffsetAnimation, mOffsetSquare));
    addChild(mOffsetSquare);

    //  create and add node that animates color
    mColorSquare = AnimationSquare::create("ColorAnim", ci::Color(255.f/255, 123.f/255, 172.f/255));
    mColorSquare->setPosition(ci::vec2((xInterval * 5) + xOffset, yOffset));
    mColorSquare->getSignal(MouseEvent::DOWN_INSIDE).connect(std::bind(&AnimationSquare::doColorAnimation, mColorSquare));
    addChild(mColorSquare);
}
