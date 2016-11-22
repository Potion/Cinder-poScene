#include "PlayerButton.h"

using namespace po::scene;

PlayerButtonRef PlayerButton::create(po::scene::ShapeViewRef shape)
{
    PlayerButtonRef ref (new PlayerButton());
    ref->setup(shape);
    return ref;
}

PlayerButton::PlayerButton()
: mIsPressed(false)
, mPressPosition(ci::vec2(0.f, 0.f))
{}

void PlayerButton::setup(po::scene::ShapeViewRef shape)
{
    mShape = shape;
    
    addChild(mShape);
    
    getSignal(MouseEvent::Type::DOWN_INSIDE).connect(std::bind(&PlayerButton::onMouseEvent, this, std::placeholders::_1));
    getSignal(MouseEvent::Type::UP_INSIDE).connect(std::bind(&PlayerButton::onMouseEvent, this, std::placeholders::_1));
    getSignal(MouseEvent::Type::MOVE).connect(std::bind(&PlayerButton::onMouseEvent, this, std::placeholders::_1));
    getSignal(MouseEvent::Type::DRAG).connect(std::bind(&PlayerButton::onMouseEvent, this, std::placeholders::_1));
}

void PlayerButton::onMouseEvent(MouseEvent &event)
{
    switch (event.getType()) {
        case MouseEvent::Type::DOWN_INSIDE:
            mIsPressed = true;
            mPressPosition = event.getLocalPos();
            mShape->setFillColor(.4f, .4f, .4f);
            break;
            
        case MouseEvent::Type::UP_INSIDE:
            if (mIsPressed) doAction();
            mShape->setFillColor(1, 1, 1);
            mIsPressed = false;
            break;
            
        case MouseEvent::Type::MOVE:
        case MouseEvent::Type::DRAG:
            if (mIsPressed) {
                ci::vec2 dragVec = mPressPosition - event.getLocalPos();
                float dragDist = length(dragVec);
                if (dragDist > 10.f) {
                    mShape->setFillColor(1, 1, 1);
                    mIsPressed = false;
                }
            }
            break;
            
        default:
            break;
    }
}

void PlayerButton::doAction()
{
    mButtonSignal.emit();
}