/*
 Copyright (c) 2015, Potion Design LLC
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 
 * Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.
 
 * Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 
 * Neither the name of copyright holder nor the names of its
 contributors may be used to endorse or promote products derived from
 this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "cinder/TriMesh.h"
#include "cinder/Triangulate.h"
#include "cinder/gl/Texture.h"

#include "poShape.h"

namespace po { namespace scene {
    #pragma mark - Create -
    
    ShapeRef Shape::create()
    {
        std::shared_ptr<Shape> s = std::shared_ptr<Shape>(new Shape());

        return s;
    }
    
    //Texture/Image
    ShapeRef Shape::create(ci::gl::TextureRef texture)
    {
        ShapeRef s = Shape::create();
        s->setTexture(texture);
        return s;
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
        
        return s;
    }
    
    ShapeRef Shape::createSquare(float size)
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

        return s;
    }
    
    ShapeRef Shape::createCircle(float size)
    {
        return createEllipse(size, size);
    }
    
    
    #pragma mark - Shape -
    Shape::Shape()
    :   mPrecision(100)
    ,   mTextureFitType(TextureFit::Type::NONE)
    ,   mTextureAlignment(Alignment::TOP_LEFT)
    ,   mTextureOffset(0,0)
    {
    }
    
    Shape::~Shape() {
    }
    
    #pragma mark - ci::Shape2d -
    void Shape::setCiShape2d(ci::Shape2d shape)
    {
        mCiShape2d = shape;
        render();
    }
    
    #pragma mark - Drawing -
    void Shape::draw()
    {
        //Draw fill
        if(getFillEnabled()) {
            ci::gl::enableAlphaBlending();
            ci::gl::color(ci::ColorA(getFillColor(), getAppliedAlpha()));
            if(mTexture) mTexture->enableAndBind();
            ci::gl::draw(mVboMesh);
            if(mTexture) mTexture->disable();
        }
        
        //Draw stroke
    }
    
    #pragma mark - Texture -
    void Shape::setTexture(ci::gl::TextureRef texture, TextureFit::Type fit, Alignment alignment) {
        mTexture            = texture;
        mTextureFitType     = fit;
        mTextureAlignment   = alignment;
        
        //If we don't have an underlying shape, set it from the texture
        if(!mCiShape2d.getNumContours()) {
            mCiShape2d.moveTo(0,0);
            mCiShape2d.lineTo(texture->getWidth(), 0);
            mCiShape2d.lineTo(texture->getWidth(), texture->getHeight());
            mCiShape2d.lineTo(0,texture->getHeight());
            mCiShape2d.close();
        }
        
        render();
    }
    
    void Shape::setTextureOffset(ci::Vec2f offset)
    {
        mTextureOffset = offset;
        render();
    }
    
    void Shape::removeTexture()
    {
        if(mTexture) {
            mTexture.reset();
            render();
        }
    }
    
    
    #pragma mark - Rendering -

    void Shape::render()
    {
        //Create Mesh
        ci::TriMesh2d mesh = ci::Triangulator(mCiShape2d, mPrecision).calcMesh(ci::Triangulator::WINDING_ODD);
        
        if(mTexture) {
            //Get the texture coords
            std::vector<ci::Vec2f> texCoords(mesh.getVertices().size());
            TextureFit::fitTexture(getBounds(), mTexture, mTextureFitType, mTextureAlignment, mesh.getVertices(), texCoords);
            
            //Check to see if texture is flipped, common if coming from FBO
            if(mTexture->isFlipped())
                std::reverse(texCoords.begin(), texCoords.end());
            
            if(mTextureOffset != ci::Vec2f(0,0)) {
                ci::Vec2f normalizedOffset = mTextureOffset/ci::Vec2f((float)mTexture->getWidth(), (float)mTexture->getHeight());
                ci::app::console() << normalizedOffset << std::endl;
                for(auto &coord : texCoords) {
                    coord -= normalizedOffset;
                }
            }
            
            
            
            //Add coords to TriMesh
            mesh.appendTexCoords(&texCoords[0], texCoords.size());
        }
        
        //Create VBO Mesh
        mVboMesh = ci::gl::VboMesh::create(mesh);
    }
    
    #pragma mark - Dimensions -
    bool Shape::pointInside(const ci::Vec2f &point, bool localize)
    {
        ci::Vec2f pos = localize ? windowToLocal(point) : point;
        return mCiShape2d.contains(pos);
    }
    
    ci::Rectf Shape::getBounds()
    {
        //if(mBoundsDirty) {
            mBounds       = mCiShape2d.calcBoundingBox();
            //mBoundsDirty = false;
        //}
        
        return mBounds;
    }
} } //  Namespace: po::scene