//
//  poShape.cpp
//  BasicScene
//
//  Created by Stephen Varga on 3/19/14.
//
//

#include "poShape.h"

namespace po {
    //BASE CLASS
    #pragma mark - Shape -
    
    ShapeRef Shape::create()
    {
        return std::shared_ptr<Shape>(new Shape());
    }
    
    Shape::Shape()
    :   fillColor(255,255,255)
    ,   fillEnabled(true)
    ,   strokeColor(255,255,255)
    ,   strokeEnabled(false)
    {
    }
    
    Shape::~Shape() {}
    
    void Shape::draw()
    {
        //Draw fill
        if(fillEnabled) {
            ci::gl::color(fillColor);
            ci::gl::drawSolid(*this);
        }
        
        //Draw stroke
        if(strokeEnabled) {
        }
    }
    
    ci::Rectf Shape::getBounds()
    {
        #pragma message "Not sure if this is gonna work"
        return calcBoundingBox();
    }
    
    
    
    //RECTANGLE
    #pragma mark - Rectangle -
    
    RectShapeRef RectShape::create()
    {
        return std::shared_ptr<RectShape>(new RectShape(100, 100));
    }
    
    RectShapeRef RectShape::create(float width, float height)
    {
        return std::shared_ptr<RectShape>(new RectShape(width, height));
    }
    
    RectShape::RectShape(float width, float height)
    {
        construct(width, height);
    }
    
    RectShape::~RectShape() {}
    
    void RectShape::construct(float width, float height) {
        moveTo(0,0);
        lineTo(width,0);
        lineTo(width,height);
        lineTo(0,height);
        close();
    }
    
}