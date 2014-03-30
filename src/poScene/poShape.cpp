//
//  poShape.cpp
//  BasicScene
//
//  Created by Stephen Varga on 3/19/14.
//
//
#include "cinder/TriMesh.h"
#include "cinder/Triangulate.h"

#include "poShape.h"

namespace po {
    //BASE CLASS
    #pragma mark - Shape -
    
    ShapeRef Shape::create()
    {
        return std::shared_ptr<Shape>(new Shape());
    }
    
    
    //Rect
    ShapeRef Shape::createRect(float width, float height)
    {
        std::shared_ptr<Shape> s = std::shared_ptr<Shape>(new Shape());
        
        s->ciShape2d.moveTo(0,0);
        s->ciShape2d.lineTo(width,0);
        s->ciShape2d.lineTo(width,height);
        s->ciShape2d.lineTo(0,height);
        s->ciShape2d.close();
        
        s->render();
        
        return s;
    }
    
    ShapeRef Shape::createRect(float size)
    {
        return createRect(size, size);
    }
    
    //Ellipse
    ShapeRef Shape::createEllipse(float width, float height)
    {
        std::shared_ptr<Shape> s = std::shared_ptr<Shape>(new Shape());
        
        //Adapted from http://stackoverflow.com/questions/2172798/how-to-draw-an-oval-in-html5-canvas
        float kappa = .5522848;
        float x = 0;
        float y = 0;
        float ox = (width / 2) * kappa; // control point offset horizontal
        float oy = (height / 2) * kappa; // control point offset vertical
        float xe = x + width;           // x-end
        float ye = y + height;           // y-end
        float xm = x + width / 2;       // x-middle
        float ym = y + height / 2;       // y-middle
        
        s->ciShape2d.moveTo(x, height/2);
        s->ciShape2d.curveTo(x, ym-oy, xm - ox, y, xm, y);
        s->ciShape2d.curveTo(xm + ox, y, xe, ym - oy, xe, ym);
        s->ciShape2d.curveTo(xe, ym + oy, xm + ox, ye, xm, ye);
        s->ciShape2d.curveTo(xm - ox, ye, x, ym + oy, x, ym);
        s->ciShape2d.close();
        
        s->render();

        return s;
    }
    
    ShapeRef Shape::createEllipse(float size)
    {
        return createEllipse(size, size);
    }
    
    Shape::Shape()
    :   fillColor(255,255,255)
    ,   fillEnabled(true)
    ,   strokeColor(255,255,255)
    ,   strokeEnabled(false)
    ,   precision(100)
    {
    }
    
    Shape::~Shape() {}
    
    bool Shape::pointInside(const ci::Vec2f &point)
    {
        return ciShape2d.contains(globalToLocal(point));
    }
    
    void Shape::setCiShape2d(ci::Shape2d shape)
    {
        ciShape2d = shape;
        render();
    }
    
    void Shape::draw()
    {
        //Draw fill
        if(fillEnabled) {
            ci::gl::color(fillColor);
            ci::gl::draw(vboMesh);
            //ci::gl::drawSolid(ciShape2d);
        }
        
        //Draw stroke
        #pragma message "Need to implement better stroke stuff"
        if(strokeEnabled) {
            ci::gl::color(strokeColor);
            ci::gl::draw(ciShape2d, ci::app::getWindowContentScale());
        }
    }
    
    void Shape::render()
    {
        ci::TriMesh2d mesh= ci::Triangulator( ciShape2d, precision).calcMesh( ci::Triangulator::WINDING_ODD);
        vboMesh = ci::gl::VboMesh::create( mesh );
    }
    
    ci::Rectf Shape::getBounds()
    {
        if(mBoundsDirty) {
            mBounds       = ciShape2d.calcPreciseBoundingBox();
            mBoundsDirty = false;
        }
        
        return mBounds;
    }
}