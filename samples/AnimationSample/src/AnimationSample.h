#pragma once

#include "poScene/View.h"
#include "AnimationSquare.h"

class AnimationSample;
typedef std::shared_ptr<AnimationSample> AnimationSampleRef;

class AnimationSample
: public po::scene::View
{
public:
    static AnimationSampleRef create();
    
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
