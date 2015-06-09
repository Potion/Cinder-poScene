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
    float interval = ci::app::getWindowWidth() / 6.0f;
    
    //  add individual AnimationSquares; a mouse click on each one will call only one specific method
    
    //  create and add node that animates position
    mPositionSquare = AnimationSquare::create("PositionAnim", ci::Color(255.f/255, 147.f/255, 30.f/255));
    mPositionSquare->setPosition(ci::Vec2f(interval - 50.f, 240.f));
    mPositionSquare->getSignal(MouseEvent::DOWN_INSIDE).connect(std::bind(&AnimationSquare::doPositionAnimation, mPositionSquare));
    addChild(mPositionSquare);

    //  create and add node that animates scale
    mScaleSquare = AnimationSquare::create("ScaleAnim", ci::Color(122.f/255, 201.f/255, 67.f/255));
    mScaleSquare->setPosition(ci::Vec2f((interval * 2) - 50.f, 240.f));
    mScaleSquare->getSignal(MouseEvent::DOWN_INSIDE).connect(std::bind(&AnimationSquare::doScaleAnimation, mScaleSquare));
    addChild(mScaleSquare);
    
    //  create and add node that animates alpha
    mAlphaSquare = AnimationSquare::create("AlphaAnim", ci::Color(189.f/255, 204.f/255, 212.f/255));
    mAlphaSquare->setPosition(ci::Vec2f((interval * 3) - 50.f, 240.f));
    mAlphaSquare->getSignal(MouseEvent::DOWN_INSIDE).connect(std::bind(&AnimationSquare::doAlphaAnimation, mAlphaSquare));
    addChild(mAlphaSquare);

    //  create and add node that animates rotation
    mRotationSquare = AnimationSquare::create("RotationAnim", ci::Color(63.f/255, 169.f/255, 245.f/255));
    mRotationSquare->setPosition(ci::Vec2f((interval * 4) - 50.f, 240.f));
    mRotationSquare->getSignal(MouseEvent::DOWN_INSIDE).connect(std::bind(&AnimationSquare::doRotationAnimation, mRotationSquare));
    addChild(mRotationSquare);

    //  create and add node that animates offset
    mOffsetSquare = AnimationSquare::create("OffsetAnim", ci::Color(122.f/255, 201.f/255, 67.f/255));
    mOffsetSquare->setPosition(ci::Vec2f((interval * 5) - 50.f, 240.f));
    mOffsetSquare->getSignal(MouseEvent::DOWN_INSIDE).connect(std::bind(&AnimationSquare::doOffsetAnimation, mOffsetSquare));
    addChild(mOffsetSquare);

    //  create and add node that animates color
    mColorSquare = AnimationSquare::create("ColorAnim", ci::Color(255.f/255, 123.f/255, 172.f/255));
    mColorSquare->setPosition(ci::Vec2f((interval * 6) - 50.f, 240.f));
    mColorSquare->getSignal(MouseEvent::DOWN_INSIDE).connect(std::bind(&AnimationSquare::doColorAnimation, mColorSquare));
    addChild(mColorSquare);
}
