#pragma once

#include "poScene/ViewController.h"
#include "AnimationSquare.h"

class AnimationSampleViewController;
typedef std::shared_ptr<AnimationSampleViewController> AnimationSampleViewControllerRef;

class AnimationSampleViewController
: public po::scene::ViewController
{
public:
    static AnimationSampleViewControllerRef create();
    
    void setup();
    
protected:
	
private:
    AnimationSquareRef mPositionSquare;
    AnimationSquareRef mScaleSquare;
    AnimationSquareRef mRotationSquare;
    AnimationSquareRef mAlphaSquare;
    AnimationSquareRef mOffsetSquare;
    AnimationSquareRef mColorSquare;
    
};
