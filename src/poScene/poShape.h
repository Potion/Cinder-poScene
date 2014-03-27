//
//  poShape.h
//  BasicScene
//
//  Created by Stephen Varga on 3/19/14.
//
//

#pragma once
#include "cinder/Shape2d.h"
#include "cinder/gl/Vbo.h"

#include "poNode.h"

namespace po {
    //Create ShapeRef typedef
    class Shape;
    typedef std::shared_ptr<Shape> ShapeRef;
    
    class Shape
    :   public Node
    {
    public:
        static ShapeRef create();
        static ShapeRef createRect(float width, float height);
        static ShapeRef createRect(float size); //Square
        static ShapeRef createEllipse(float width, float height);
        static ShapeRef createEllipse(float size); //Circle
        
        ~Shape();
        
        virtual void draw();
        
        //Return the backing ci::Shape2d
        ci::Shape2d getCiShape2dCopy() { return ciShape2d; };
        void setCiShape2d(ci::Shape2d shape);
        
        //Set precision for triangulation/rendering
        int getPrecision() { return precision; }
        void setPrecision(int precision) { this->precision = precision; }
        
        //Fill & Stroke
        void setFillEnabled(bool enabled)   { fillEnabled = enabled; };
        void setStrokeEnabled(bool enabled) { strokeEnabled = enabled; };
        
        //Bounds
        virtual ci::Rectf getBounds();
        
        //Hit testing
        bool pointInside(const ci::Vec2f &point);
        
        //Caching to VBO
        void render();
        
        //------------------
        //ATTRIBUTES
        ci::Color fillColor;
        ci::Color strokeColor;
        
    protected:
        Shape();
        
    private:
        virtual void addChild() {};
        virtual void removeChild() {};
        
        int precision;
        ci::gl::VboMeshRef vboMesh;
        
        ci::Shape2d ciShape2d;
        bool fillEnabled, strokeEnabled;
    };
}