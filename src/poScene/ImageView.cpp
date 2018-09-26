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

#include "poScene/ImageView.h"

#include "cinder/gl/gl.h"

namespace po
{
	namespace scene
	{
		ci::gl::BatchRef ImageView::mTextureBatch = nullptr;

		ImageViewRef ImageView::create()
		{
			return create( nullptr );
		}

		ImageViewRef ImageView::create( ci::gl::TextureRef texture )
		{
			ImageViewRef ref( new ImageView( texture ) );
			return ref;
		}

		ImageView::ImageView( ci::gl::TextureRef texture )
			: mTexture( texture )
		{
			if( mTextureBatch == nullptr ) {
				ci::gl::GlslProgRef textureShader = ci::gl::getStockShader( ci::gl::ShaderDef().texture().color() );
				mTextureBatch = ci::gl::Batch::create( ci::geom::Rect( ci::Rectf( 0, 0, 1, 1 ) ), textureShader );
			}

			//auto mesh = ci::geom::Rect( ci::Rectf( 0, 0, 1, 1 ) ) >> ci::geom::AttribFn<ci::vec2, ci::vec2>( ci::geom::TEX_COORD_0, ci::geom::TEX_COORD_0, []( const ci::vec2 & uv ) {
			//	return uv * 2.0f;
			//} );
		}

		void ImageView::draw()
		{
			if( mTexture ) {
				ci::gl::ScopedBlendAlpha alphaBlendScoped;
				ci::gl::ScopedColor fillColorScoped( ci::ColorA( getFillColor(), getAppliedAlpha() ) );

				ci::gl::ScopedTextureBind texBind( mTexture );
				ci::gl::ScopedModelMatrix mModelView;
				ci::gl::scale( ci::vec2( mTexture->getWidth(), mTexture->getHeight() ) );
				mTextureBatch->draw();
			}
		}

		ci::Rectf ImageView::getBounds()
		{
			return mTexture->getBounds();
		}

	}
} //  Namespace: po::scene