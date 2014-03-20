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
    
    void Shape::draw()
    {
        //Draw fill
        if(fillEnabled) {
            ci::Color(fillColor);
            ci::gl::draw(*this);
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
    
}