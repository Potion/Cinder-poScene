#pragma once

#include "poNodeContainer.h"
#include "poShape.h"


class PlayerButton;
typedef std::shared_ptr<PlayerButton> PlayerButtonRef;

class PlayerButton
: public po::scene::NodeContainer
{
public:
    typedef boost::signals2::signal<void()> ButtonSignal;
    
    static PlayerButtonRef  create(po::scene::ShapeRef shape);
    void                    setup(po::scene::ShapeRef shape);
    ButtonSignal            &getButtonSignal() { return mButtonSignal; };

protected:
    PlayerButton();
    
private:
    bool                mPressed;
    std::string         mName;
    po::scene::ShapeRef mShape;
    
    void                onMouseEvent(po::scene::MouseEvent event);
    void                doAction();
    
    ButtonSignal        mButtonSignal;
    ci::Vec2f           mPressPosition;
    
};