//
//  poEvent.cpp
//  BasicScene
//
//  Created by Stephen Varga on 3/22/14.
//
//

#include "poEvents.h"

namespace po {

    #pragma mark - Mouse Event -
    //Brutal copy-constructor replication...def not fun
    MouseEvent::MouseEvent(ci::app::MouseEvent event, ci::Vec2f offset)
    : ci::app::MouseEvent(event.getWindow()
                          , (event.isLeft() ? ci::app::MouseEvent::LEFT_DOWN
                                            : event.isRight() ? ci::app::MouseEvent::RIGHT_DOWN
                                            : ci::app::MouseEvent::MIDDLE_DOWN)
                          , event.getX()
                          , event.getY()
                          , (0
                             | (event.isShiftDown()     ? ci::app::MouseEvent::SHIFT_DOWN   : 0)
                             | (event.isAltDown()       ? ci::app::MouseEvent::ALT_DOWN     : 0)
                             | (event.isControlDown()   ? ci::app::MouseEvent::CTRL_DOWN    : 0)
                             | (event.isMetaDown()      ? ci::app::MouseEvent::META_DOWN    : 0)
                             | (event.isAccelDown()     ? ci::app::MouseEvent::ACCEL_DOWN   : 0)
                             | (event.isLeft()          ? ci::app::MouseEvent::LEFT_DOWN    : 0)
                             | (event.isRight()         ? ci::app::MouseEvent::RIGHT_DOWN   : 0)
                             | (event.isMiddle()        ? ci::app::MouseEvent::MIDDLE_DOWN  : 0)
                            )
                          , event.getWheelIncrement()
                          , event.getNativeModifiers()
                        
                        ) {
        
        mWindowPos = event.getPos() + offset;
    }
    
    
    #pragma mark - Touch Event -
    TouchEvent::TouchEvent(ci::app::TouchEvent event)
    {
        for(const ci::app::TouchEvent::Touch touch : event.getTouches()) {
            mTouches.push_back(ci::app::TouchEvent::Touch(touch));
        }
    }
    
    TouchEvent::Touch::Touch(ci::app::TouchEvent::Touch touch)
    : mPos(touch.getPos())
	, mPrevPos(touch.getPrevPos())
    , mWindowPos(touch.getPos())
    , mId(touch.getId())
    , mTime(touch.getTime())
    , mNative((void *)touch.getNative())
    {
    }
    
    
    #pragma mark - Key Event -
    KeyEvent::KeyEvent(ci::app::KeyEvent event)
    : ci::app::KeyEvent(event.getWindow()
                       , event.getCode()
                       , event.getCharUtf32()
                       , event.getChar()
                       , (0
                            | (event.isShiftDown()     ? ci::app::MouseEvent::SHIFT_DOWN   : 0)
                            | (event.isAltDown()       ? ci::app::MouseEvent::ALT_DOWN     : 0)
                            | (event.isControlDown()   ? ci::app::MouseEvent::CTRL_DOWN    : 0)
                            | (event.isMetaDown()      ? ci::app::MouseEvent::META_DOWN    : 0)
                            | (event.isAccelDown()      ? ci::app::MouseEvent::ACCEL_DOWN  : 0)
                        )
                       , event.getNativeKeyCode()
                       )
    {
    }
}