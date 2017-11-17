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
    
    ShapeRef Shape::create()
    {
        std::shared_ptr<Shape> s = std::shared_ptr<Shape>(new Shape());
        return s;
    }
	
	//
    //	Texture/Image
	//
    ShapeRef Shape::create(ci::gl::TextureRef texture)
    {
        ShapeRef s = Shape::create();
        s->setTexture(texture);
        return s;
    }
    
    //
    //	Rect
	//
    ShapeRef Shape::createRect(float width, float height, float rad )
    {
		
		if ( rad > 0.f )
			return createRoundedRect( width, height, rad );
		
        std::shared_ptr<Shape> s = std::shared_ptr<Shape>(new Shape());
        ci::Shape2d shape;
        shape.moveTo(0, 0);
        shape.lineTo(width, 0);
        shape.lineTo(width, height);
        shape.lineTo(0, height);
        shape.close();
		
        s->setCiShape2d(shape);
        return s;
    }
	
	//
	//	Rounded Rect
	//
	ShapeRef Shape::createRoundedRect(float width, float height, float rad)
	{
		std::shared_ptr<Shape> s = std::shared_ptr<Shape>(new Shape());
		
		// points
		ci::vec2 p1( rad,0 );
		ci::vec2 p2( width-rad,0 );
		ci::vec2 p3( width,rad );
		ci::vec2 p4( width,height-rad );
		ci::vec2 p5( width-rad,height );
		ci::vec2 p6( rad,height );
		ci::vec2 p7( 0,height-rad );
		ci::vec2 p8( 0,rad );

		// tangents
		ci::vec2 t1( width,0 );
		ci::vec2 t2( width,height );
		ci::vec2 t3( 0,height );
		ci::vec2 t4( 0,0 );
		
		ci::Shape2d shape;
		shape.moveTo( p1 );
		shape.lineTo( p2 );
		shape.arcTo( p3,t1,rad );
		shape.lineTo( p4 );
		shape.arcTo( p5,t2,rad );
		shape.lineTo( p6 );
		shape.arcTo( p7,t3,rad );
		shape.lineTo( p8 );
		shape.arcTo( p1,t4,rad );
		shape.close();
		
		s->setCiShape2d(shape);

		return s;
		
	}
	
	
    ShapeRef Shape::createSquare(float size)
    {
        return createRect(size, size);
    }
	
	//
    //	Ellipse
	//
    ShapeRef Shape::createEllipse(float width, float height)
    {
        std::shared_ptr<Shape> s = std::shared_ptr<Shape>(new Shape());
        
        //	Adapted from http://stackoverflow.com/questions/2172798/how-to-draw-an-oval-in-html5-canvas
        float kappa = .5522848f;
        float x = 0;
        float y = 0;
        float ox = (width / 2) * kappa;		//	control point offset horizontal
        float oy = (height / 2) * kappa;	//	control point offset vertical
        float xe = x + width;				//	x-end
        float ye = y + height;				//	y-end
        float xm = x + width / 2;			//	x-middle
        float ym = y + height / 2;			//	y-middle
        
        ci::Shape2d shape;
        shape.moveTo(x, height/2);
        shape.curveTo(x, ym - oy, xm - ox, y, xm, y);
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
	
    Shape::Shape()
    : mPrecision(100)
    , mTextureFitType(TextureFit::Type::NONE)
    , mTextureAlignment(Alignment::TOP_LEFT)
    , mTextureOffset(0, 0)
    {
    }
    
    Shape::~Shape()
	{
    }
    
    void Shape::setCiShape2d(ci::Shape2d shape)
    {
        mCiShape2d = shape;
        render();
    }
    
    void Shape::draw()
    {
        //Draw fill
        if (getFillEnabled()) {
			ci::gl::ScopedBlendAlpha alphaBlendScoped;
			ci::gl::ScopedColor fillColorScoped(ci::ColorA(getFillColor(), getAppliedAlpha()));
            
            if (mTexture) {
                ci::gl::ScopedGlslProg shaderScp( ci::gl::getStockShader( ci::gl::ShaderDef().texture().color()));
                ci::gl::ScopedTextureBind texBind(mTexture);
                ci::gl::draw(mVboMesh);
            } else {
                ci::gl::ScopedGlslProg shaderScp( ci::gl::getStockShader( ci::gl::ShaderDef().color()));
                ci::gl::draw(mVboMesh);
            }
        }
        
        //	TODO: Draw stroke
    }
    
    
    //------------------------------------
    //	Texture
	//------------------------------------
    
    void Shape::setTexture(ci::gl::TextureRef texture, TextureFit::Type fit, Alignment alignment)
	{
        mTexture            = texture;
        mTextureFitType     = fit;
        mTextureAlignment   = alignment;
        
        //	If we don't have an underlying shape, set it from the texture
        if (!mCiShape2d.getNumContours()) {
            mCiShape2d.moveTo(0, 0);
            mCiShape2d.lineTo((float)texture->getWidth(), 0);
            mCiShape2d.lineTo((float)texture->getWidth(), (float)texture->getHeight());
            mCiShape2d.lineTo(0, (float)texture->getHeight());
            mCiShape2d.close();
        }
        
        render();
    }
    
    void Shape::setTextureOffset(ci::vec2 offset)
    {
        mTextureOffset = offset;
        render();
    }
    
    void Shape::removeTexture()
    {
        if (mTexture) {
            mTexture.reset();
            render();
        }
    }
    
    
    //------------------------------------
    //	Rendering
	//------------------------------------

    void Shape::render()
    {
        //Create a TriMesh from our shape
        ci::TriMesh::Format format = ci::TriMesh::Format();
        format.mTexCoords0Dims      = 2;
        format.mPositionsDims       = 2;
        format.mNormalsDims         = 3;
        ci::TriMeshRef triMesh = ci::TriMesh::create( ci::Triangulator(mCiShape2d, (float)mPrecision).calcMesh(ci::Triangulator::WINDING_POSITIVE), format );
        
        if( mTexture )
        {
            //  Allocate
            std::vector< ci::vec2 > texCoords(triMesh->getNumVertices());
            std::vector< ci::vec2 > vertices;
            
            //  Grab the pointer to the mesh vertices
            const ci::vec2* meshVertices = triMesh->getPositions<2>();
            
            //  Grab the actual vertices
            for ( int i=0; i<triMesh->getNumVertices(); i++ )  {
                vertices.push_back( meshVertices[i] );
            }
            
            //  Calculate the texture coords
            TextureFit::fitTexture(getBounds(), mTexture, mTextureFitType, mTextureAlignment, vertices, texCoords);
            
            //  Check to see if texture is flipped, common if coming from FBO
            if(!mTexture->isTopDown()) {
                for(auto &coord : texCoords) {
                    coord.y = 1 - coord.y;
                }
            }
            
            //  Set the offset
            if(mTextureOffset != ci::vec2(0,0)) {
                ci::vec2 normalizedOffset = mTextureOffset/ci::vec2((float)mTexture->getWidth(), (float)mTexture->getHeight());
                for(auto &coord : texCoords) {
                    coord -= normalizedOffset;
                }
            }
            
            // appending texture coordinates to the mesh ref
            triMesh->appendTexCoords0(&texCoords[0], texCoords.size());
        }
        
        
        //Create VBO mesh
        mVboMesh = ci::gl::VboMesh::create(*triMesh);
    }
    
    
    //------------------------------------
    //	Dimensions
	//------------------------------------
    
    bool Shape::pointInside(const ci::vec2 &point, bool localize)
    {
        ci::vec2 pos = localize ? windowToLocal(point) : point;
        return mCiShape2d.contains(pos);
    }
    
    ci::Rectf Shape::getBounds()
    {
        //if (mBoundsDirty) {
            mBounds = mCiShape2d.calcBoundingBox();
            //mBoundsDirty = false;
        //}
        
        return mBounds;
    }
	
} } //  namespace po::scene