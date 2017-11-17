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

#pragma once

#include "cinder/Shape2d.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/VboMesh.h"
#include "cinder/gl/draw.h"
#include "cinder/gl/Shader.h"
#include "cinder/gl/scoped.h"

#include "poScene/View.h"
#include "poScene/TextureFit.h"

namespace po
{
	namespace scene
	{
		// ShapeView2d is a class that converts any Cinder ShapeView2d to a VBO Mesh
		// and uses it as the drawable object.
		//
		// It supports textures, which can be mapped using the po::Scene::TextureFit::Type enums.
		//
		// Interaction is supported through the ci::Shape2d contains method, which is localized
		// to the position and the offset/alignment of the View.
		//
		// Setting the ShapeView, Texture or TextureFit will invalidate the VBOMesh and re-render the ShapeView.

		//	Create ShapeViewRef typedef
		typedef std::shared_ptr<class ShapeView> ShapeViewRef;

		class ShapeView
			: public View
		{
			public:
				//! Create an empy ShapeView
				static ShapeViewRef create();

				//! Create a ShapeView with a texture (defaults to a rect ShapeView)
				static ShapeViewRef create( ci::gl::TextureRef texture );

				//! Create a rectangular ShapeView
				static ShapeViewRef createRect( float width, float height, float rad = 0.f );

				//! Create a rounded rect
				static ShapeViewRef createRoundedRect( float width, float height, float rad );

				//! Create an square ShapeView
				static ShapeViewRef createSquare( float size );

				//! Create an elliptical ShapeView
				static ShapeViewRef createEllipse( float width, float height );

				//! Create an circle
				static ShapeViewRef createCircle( float size );

				~ShapeView();

				//! Draw the ShapeView's VBOMesh and attach texture if set
				virtual void draw();

				//!	Set/Return the backing ci::Shape2d
				/**	This should be used for modifying or changing the ShapeView **/
				ci::Shape2d getCiShapeView2dCopy() { return mCiShape2d; };

				/**	This should be used for modifying or changing the ShapeView **/
				void setCiShape2d( ci::Shape2d ShapeView );

				//! Get the bounds
				/** The bounds of a ShapeView are determined by the bounds of the ci::Shape2d
				    plus any offsets/alignments**/
				virtual ci::Rectf getBounds();

				//! Determine if a point is inside the ShapeView by localizing it and checking if it intersects
				bool pointInside( const ci::vec2& point, bool localize = true );

				//	Caching to VBO
				//! Triangulate the ci::Shape2d and push to a VBOMesh with correct texture coords
				void render();
				//! Get the VBOMeshRef
				ci::gl::VboMeshRef getVbo() { return mVboMesh; };

				//! Set the texture with fit and alignment
				void setTexture( ci::gl::TextureRef texture, TextureFit::Type fit = TextureFit::Type::NONE, Alignment alignment = Alignment::TOP_LEFT );
				//! Set an offset for the texture
				void setTextureOffset( ci::vec2 offset );
				//! Get the texture
				ci::gl::TextureRef getTexture() { return mTexture; }
				//! Remove the texture and return to using fill color to render
				void removeTexture();

				//! Get the internal precision
				int getPrecision() { return mPrecision; }
				//! Set the internal precision to use when triangulating the ci::Shape2d
				ShapeView& setPrecision( int precision ) { mPrecision = precision; return *this;}

			protected:
				ShapeView();

			private:
				//	Our Vbo
				ci::gl::VboMeshRef mVboMesh;

				//	Our underlying ci::Shape2d
				ci::Shape2d mCiShape2d;

				//	Textures
				ci::gl::TextureRef mTexture;
				TextureFit::Type mTextureFitType;
				Alignment mTextureAlignment;
				ci::vec2 mTextureOffset;

				int mPrecision;

		};

	}
} //  namespace po::scene
