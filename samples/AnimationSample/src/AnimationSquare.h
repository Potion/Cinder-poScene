//
//  AnimationSquare.h
//  AnimationSample
//
//  Created by Jennifer Presto on 6/9/15.
//
//

#pragma once

#include "poNodeContainer.h"
#include "poShape.h"
#include "Indicator.h"

class AnimationSquare;
typedef std::shared_ptr<AnimationSquare> AnimationSquareRef;

class AnimationSquare
: public po::scene::NodeContainer
{
public:
    static AnimationSquareRef create(std::string name, ci::Color color);
    void setup(std::string name, ci::Color color);
    
    void doPositionAnimation();
    void doScaleAnimation();
    void doAlphaAnimation();
    void doRotationAnimation();
    void doOffsetAnimation();
    void doColorAnimation();
    
protected:

private:
    po::scene::ShapeRef mBaseShape;
    ci::Vec2f           mBasePosition;
    ci::Color           mBaseColor;
    
    IndicatorRef        mIndicator;
    void                showIndicator();
    
};