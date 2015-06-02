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
#include "poTextureFit.h"

namespace po {
    
    //Create ShapeRef typedef
    class Shape;
    typedef std::shared_ptr<Shape> ShapeRef;
    
    class Shape
    :   public Node
    {
    public:
        static ShapeRef create();
        static ShapeRef create(ci::gl::TextureRef texture);
        
        static ShapeRef createRect(float width, float height);
        static ShapeRef createSquare(float size);
        
        static ShapeRef createEllipse(float width, float height);
        static ShapeRef createCircle(float size);
        
        ~Shape();
        
        virtual void draw();
        
        //Set/Return the backing ci::Shape2d
        //This should be used for modifying or changing the shape
        ci::Shape2d getCiShape2dCopy() { return mCiShape2d; };
        void setCiShape2d(ci::Shape2d shape);
        
        //Bounds
        virtual ci::Rectf getBounds();
        
        //Hit testing
        bool pointInside(const ci::Vec2f &point, bool localize = true);
        
        //Caching to VBO
        void render();
        void setUseVBO(bool useVBO);
        bool getUseVBO() { return mUseVBO; };
        ci::gl::VboMeshRef getVbo() { return mVboMesh; };
        
        //Texture
        void setTexture(ci::gl::TextureRef texture, TextureFit::Type fit = TextureFit::Type::NONE, Alignment alignment = Alignment::TOP_LEFT);
        void setTextureOffset(ci::Vec2f offset);
        ci::gl::TextureRef getTexture() { return mTexture; }
        
        void removeTexture();
        
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
        TextureFit::Type  mTextureFitType;
        Alignment   mTextureAlignment;
        ci::Vec2f mTextureOffset;
        
        int mPrecision;
    };
}