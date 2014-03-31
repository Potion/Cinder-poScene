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
        
        ci::Shape2d shape;
        
        shape.moveTo(0,0);
        shape.lineTo(width,0);
        shape.lineTo(width,height);
        shape.lineTo(0,height);
        shape.close();
        
        s->setCiShape2d(shape);
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
        
        ci::Shape2d shape;
        
        shape.moveTo(x, height/2);
        shape.curveTo(x, ym-oy, xm - ox, y, xm, y);
        shape.curveTo(xm + ox, y, xe, ym - oy, xe, ym);
        shape.curveTo(xe, ym + oy, xm + ox, ye, xm, ye);
        shape.curveTo(xm - ox, ye, x, ym + oy, x, ym);
        shape.close();
        
        s->setCiShape2d(shape);
        s->render();

        return s;
    }
    
    ShapeRef Shape::createEllipse(float size)
    {
        return createEllipse(size, size);
    }
    
    Shape::Shape()
    :   mFillColor(255,255,255)
    ,   mFillEnabled(true)
    ,   mStrokeColor(255,255,255)
    ,   mStrokeEnabled(false)
    ,   mPrecision(100)
    {
    }
    
    Shape::~Shape() {}
    
    bool Shape::pointInside(const ci::Vec2f &point)
    {
        return mCiShape2d.contains(globalToLocal(point));
    }
    
    void Shape::setCiShape2d(ci::Shape2d shape)
    {
        mCiShape2d = shape;
        render();
    }
    
    void Shape::draw()
    {
        //Draw fill
        if(mFillEnabled) {
            ci::gl::color(mFillColor);
            ci::gl::draw(mVboMesh);
            //ci::gl::drawSolid(ciShape2d);
        }
        
        //Draw stroke
        #pragma message "Need to implement better stroke stuff"
        if(mStrokeEnabled) {
            ci::gl::color(mStrokeColor);
            ci::gl::draw(mCiShape2d, ci::app::getWindowContentScale());
        }
    }
    
    void Shape::render()
    {
        ci::TriMesh2d mesh= ci::Triangulator( mCiShape2d, mPrecision).calcMesh( ci::Triangulator::WINDING_ODD);
        mVboMesh = ci::gl::VboMesh::create( mesh );
    }
    
    ci::Rectf Shape::getBounds()
    {
        if(mBoundsDirty) {
            mBounds       = mCiShape2d.calcPreciseBoundingBox();
            mBoundsDirty = false;
        }
        
        return mBounds;
    }
}