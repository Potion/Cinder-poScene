//
//  AnimationSquare.cpp
//  AnimationSample
//
//  Created by Jennifer Presto on 6/9/15.
//
//


#include "AnimationSquare.h"
#include "poShape.h"

using namespace po::scene;

AnimationSquareRef AnimationSquare::create(std::string name, ci::Color color)
{
    AnimationSquareRef ref (new AnimationSquare());
    ref->setup(name, color);
    return ref;
}

void AnimationSquare::setup(std::string name, ci::Color color)
{

    //  create and add the shape to the node container
    mBaseShape = Shape::createRect(100, 100);
    mBaseColor = color;
    mBaseShape->setFillColor(color);
    addChild(mBaseShape);

    //  create and add the label
    mIndicator = Indicator::create(name, color);
    mIndicator->setPosition(ci::Vec2f(0, 125));
    addChild(mIndicator);
    
    //  add a signal to all mouse clicks to activate label
    getSignal(MouseEvent::DOWN_INSIDE).connect(std::bind(&AnimationSquare::showIndicator, this));
    
}

void AnimationSquare::doPositionAnimation()
{
    ci::Vec2f topPos = ci::Vec2f(0.0f, - 100.f);
    ci::Vec2f bottomPos = ci::Vec2f(0.0f, 100.f);
    
    ci::app::timeline().apply(&mBaseShape->getPositionAnim(), topPos, 0.5);
    ci::app::timeline().appendTo(&mBaseShape->getPositionAnim(), bottomPos, 0.5);
    ci::app::timeline().appendTo(&mBaseShape->getPositionAnim(), ci::Vec2f(0.0f, 0.0f), 0.5);
}

void AnimationSquare::doScaleAnimation()
{
    ci::app::timeline().apply(&mBaseShape->getScaleAnim(), ci::Vec2f(1.5f, 1.5f), 0.5);
    ci::app::timeline().appendTo(&mBaseShape->getScaleAnim(), ci::Vec2f(0.5f, 0.5f), 0.5);
    ci::app::timeline().appendTo(&mBaseShape->getScaleAnim(), ci::Vec2f(1.0f, 1.0f), 0.5);
}

void AnimationSquare::doAlphaAnimation()
{
    ci::app::timeline().apply(&mBaseShape->getAlphaAnim(), 0.0f, 0.5);
    ci::app::timeline().appendTo(&mBaseShape->getAlphaAnim(), 1.0f, 0.5);
}

void AnimationSquare::doRotationAnimation()
{
    mBaseShape->setRotation(0.0f);
    ci::app::timeline().apply(&mBaseShape->getRotationAnim(), 360.f, 1.5);
}

void AnimationSquare::doOffsetAnimation()
{
    ci::app::timeline().apply(&mBaseShape->getOffsetAnim(), ci::Vec2f(25.0f, 25.0f), 0.5);
    ci::app::timeline().appendTo(&mBaseShape->getOffsetAnim(), ci::Vec2f(-25.0f, -25.0f), 0.5);
    ci::app::timeline().appendTo(&mBaseShape->getOffsetAnim(), ci::Vec2f(0.0f, 0.0f), 0.5);
}

void AnimationSquare::doColorAnimation()
{
    ci::Color orange = ci::Color(255.f/255, 147.f/255, 30.f/255);
    ci::Color blue = ci::Color(63.f/255, 169.f/255, 245.f/255);
    
    ci::app::timeline().apply(&mBaseShape->getFillColorAnim(), orange, 0.5);
    ci::app::timeline().appendTo(&mBaseShape->getFillColorAnim(), blue, 0.5);
    ci::app::timeline().appendTo(&mBaseShape->getFillColorAnim(), mBaseColor, 0.5);
}

void AnimationSquare::useBaseShapeCenter()
{
    mBaseShape->setAlignment(po::scene::Alignment::CENTER_CENTER);
    mBaseShape->setPosition(50.f, 50.f);
}

void AnimationSquare::useBaseShapeTopLeft()
{
    mBaseShape->setAlignment(po::scene::Alignment::TOP_LEFT);
    mBaseShape->setPosition(0.0f, 0.0f);
}

void AnimationSquare::showIndicator()
{
    mIndicator->showHighlighted();
}