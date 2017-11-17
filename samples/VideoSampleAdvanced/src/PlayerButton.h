#pragma once

#include "cinder/Signals.h"

#include "poScene/View.h"
#include "poScene/ShapeView.h"


class PlayerButton;
typedef std::shared_ptr<PlayerButton> PlayerButtonRef;

class PlayerButton
: public po::scene::View
{
public:
    typedef ci::signals::Signal<void()> ButtonSignal;
    
    static PlayerButtonRef  create(po::scene::ShapeViewRef shape);
    void                    setup(po::scene::ShapeViewRef shape);
    ButtonSignal            &getButtonSignal() { return mButtonSignal; };

protected:
    PlayerButton();
    
private:
    bool                mIsPressed;
    std::string         mName;
    po::scene::ShapeViewRef mShape;
    
    void                onMouseEvent(po::scene::MouseEvent &event);
    void                doAction();
    
    ButtonSignal        mButtonSignal;
    ci::vec2            mPressPosition;
    
};