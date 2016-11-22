#pragma once

#include "poScene/ViewContainer.h"
#include "poScene/ShapeView.h"
#include "Indicator.h"

class AnimationSquare;
typedef std::shared_ptr<AnimationSquare> AnimationSquareRef;

class AnimationSquare
: public po::scene::ViewContainer
{
public:
    static AnimationSquareRef create(std::string name, ci::Color color);
    void setup(std::string name, ci::Color color);
    
    //  All AnimationSquares have all the following methods;
    //  the app will call only one for each individual AnimationSquare used.
    void doPositionAnimation();
    void doScaleAnimation();
    void doAlphaAnimation();
    void doRotationAnimation();
    void doOffsetAnimation();
    void doColorAnimation();
    
    //  Because certain animations, like rotation and scale, might
    //  look better with a center rotation point, we include two
    //  methods to adjust the alignment of just mBaseShape
    //  and reposition it accordingly.
    void useBaseShapeCenter();
    void useBaseShapeTopLeft();
    
protected:

private:
    po::scene::ViewRef mBaseShape;
    ci::vec2           mBasePosition;
    ci::Color           mBaseColor;
    
    IndicatorRef        mIndicator;
    void                showIndicator();
    
};