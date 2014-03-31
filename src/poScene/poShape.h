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
#include "poUtils.h"

namespace po {
    
    //Create ShapeRef typedef
    class Shape;
    typedef std::shared_ptr<Shape> ShapeRef;
    
    class Shape
    :   public Node
    {
    public:
        static ShapeRef create();
        static ShapeRef create(ci::gl::TextureRef texture) { return createRect(texture); }
        
        static ShapeRef createRect(float width, float height);
        static ShapeRef createRect(float size); //Square
        
        static ShapeRef createRect(ci::gl::TextureRef texture);
        
        static ShapeRef createEllipse(float width, float height);
        static ShapeRef createEllipse(float size); //Circle
        
        ~Shape();
        
        virtual void draw();
        
        //Set/Return the backing ci::Shape2d
        //This should be used for modifying or changing the shape
        ci::Shape2d& getCiShape2d()     { return mCiShape2d; };
        ci::Shape2d getCiShape2dCopy() { return mCiShape2d; };
        void setCiShape2d(ci::Shape2d shape);
        
        //Bounds
        virtual ci::Rectf getBounds();
        
        //Hit testing
        bool pointInside(const ci::Vec2f &point);
        
        //Caching to VBO
        void render();
        
        //Texture
        void setTexture(ci::gl::TextureRef texture, TextureFit fit = TextureFit::NONE, Alignment alignment = Alignment::TOP_LEFT);
        ci::gl::TextureRef getTexture() { return mTexture; }
        
        //------------------
        //ATTRIBUTES
        
        //Fill
        Shape& fillColor(ci::Color color)               { setFillColor(color); return *this; }
        Shape& fillColor(float r, float g, float b)     { setFillColor(r,g,b); return *this; }
        void setFillColor(ci::Color color)              { mFillColor = color; }
        void setFillColor(float r, float g, float b)    { mFillColor.set(r,g,b); }
        
        Shape& fillEnabled(bool enabled)    { setFillEnabled(enabled); return *this; }
        void setFillEnabled(bool enabled)   { mFillEnabled = enabled; };
        
        bool getFillEnabled() { return mFillEnabled; }
        ci::Color getFillColor() { return mFillColor; }
        
        //Stroke
        Shape& strokeColor(ci::Color color)               { setStrokeColor(color); return *this; }
        Shape& strokeColor(float r, float g, float b)     { setStrokeColor(r,g,b); return *this; }
        void setStrokeColor(ci::Color color)              { mStrokeColor = color; }
        void setStrokeColor(float r, float g, float b)    { mStrokeColor.set(r,g,b); }
        
        Shape& strokeEnabled(bool enabled)      { setStrokeEnabled(enabled); return *this; }
        void setStrokeEnabled(bool enabled)     { mStrokeEnabled = enabled; };
        
        bool getStrokeEnabled()     { return mStrokeEnabled; }
        ci::Color getStrokeColor()  { return mStrokeColor; }
        
        //Precision (for rendering)
        Shape& precision(int precision) { setPrecision(precision); return *this; }
        int getPrecision() { return mPrecision; }
        void setPrecision(int precision) { mPrecision = precision; }
        
    protected:
        Shape();
        
    private:
        //Our Vbo
        ci::gl::VboMeshRef mVboMesh;
        
        //Our underlying ci::Shape2d
        ci::Shape2d mCiShape2d;
        
        //Textures
        ci::gl::TextureRef mTexture;
        
        //Attributes
        ci::Color mFillColor;
        ci::Color mStrokeColor;
        
        TextureFit  mTextureFit;
        Alignment   mTextureAlignment;
        
        bool mFillEnabled, mStrokeEnabled;
        
        int mPrecision;
    };
}