//
//  poShape.h
//  BasicScene
//
//  Created by Stephen Varga on 3/19/14.
//
//

#pragma once
#include "poNode.h"

namespace po {
    //Base Class
    class Shape;
    typedef std::shared_ptr<Shape> ShapeRef;
    
    class Shape
    :   public Node
        #pragma message "Is this the best way to do this? Would be cool to make these compatible."
        #pragma message "Should we use Path2d instead?"
    ,   public ci::Shape2d
    {
    public:
        static ShapeRef create();
        
        ~Shape();
        
        virtual void draw();
        
        //Fill & Stroke
        void enableFill();
        void disableFill();
        void enableStroke();
        void disableStroke();
        
        //Bounds
        virtual ci::Rectf getBounds();
        
        //------------------
        //ATTRIBUTES
        ci::Color fillColor;
        ci::Color strokeColor;
        
    protected:
        Shape();
    private:
        bool fillEnabled, strokeEnabled;
    };
    
    
    //RECTANGLE
    class RectShape;
    typedef std::shared_ptr<RectShape> RectShapeRef;
    
    class RectShape
    :   public Shape
    {
    public:
        static RectShapeRef create();
        static RectShapeRef create(float width, float height);
        
        ~RectShape();
    protected:
        RectShape(float width, float height);
        
        void construct(float width, float height);
    };
}