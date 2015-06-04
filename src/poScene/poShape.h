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
#include "poNode.h"
#include "poTextureFit.h"

namespace po {
	namespace scene {
    
		//Create ShapeRef typedef
		class Shape;
		typedef std::shared_ptr<Shape> ShapeRef;
		
		class Shape
		: public Node
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
			ci::gl::VboMeshRef getVbo() { return mVboMesh; };
			
			//Texture
			void setTexture(ci::gl::TextureRef texture, TextureFit::Type fit = TextureFit::Type::NONE, Alignment alignment = Alignment::TOP_LEFT);
			void setTextureOffset(ci::Vec2f offset);
			ci::gl::TextureRef getTexture() { return mTexture; }
			
			void removeTexture();
			
			//Precision (for rendering)
			Shape &precision(int precision) { setPrecision(precision); return *this; }
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
			TextureFit::Type mTextureFitType;
			Alignment mTextureAlignment;
			ci::Vec2f mTextureOffset;
			
			int mPrecision;
			
		};
		
	}
} //  Namespace: po::scene