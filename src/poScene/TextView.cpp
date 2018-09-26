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

#include "poScene/TextView.h"

#include "cinder/gl/gl.h"

namespace po
{
	namespace scene
	{

		TextViewRef TextView::create()
		{
			return TextViewRef( new TextView( ci::TextBox() ) );
		}

		TextViewRef TextView::create( ci::TextBox ciTextBox )
		{
			return TextViewRef( new TextView( ciTextBox ) );
		}

		TextViewRef TextView::create( ci::TextBox ciTextBox, ci::gl::Texture::Format format )
		{
			TextViewRef ref( new TextView( ciTextBox ) );
			ref->setFormat( format );
			return ref;
		}

		TextView::TextView( ci::TextBox ciTextBox )
			: mCiTextBox( ciTextBox )
			, mUseTextBounds( false )
			, mHasFormat( false )
		{
			render();
		}

		void TextView::draw()
		{
			if( mTexture ) {
				ci::gl::ScopedBlendAlpha alphaBlendScoped;
				ci::gl::ScopedColor fillColorScoped( ci::ColorA( getFillColor(), getAppliedAlpha() ) );
				ci::gl::draw( mTexture );
			}
		}

		void TextView::setCiTextBox( ci::TextBox& ciTextBox )
		{
			mCiTextBox = ciTextBox;
			render();

			mUseTextBounds = mCiTextBox.getSize().y == ci::TextBox::GROW ? true : false;
		}

		void TextView::render()
		{

			try {

				if( mHasFormat ) {
					mTexture = ci::gl::Texture::create( mCiTextBox.render(), mFormat );
				}
				else {
					mTexture = ci::gl::Texture::create( mCiTextBox.render() );
				}
			}
			catch( ... ) {
				ci::app::console() << "TextView: Failed to render string: \n" << mCiTextBox.getText() << std::endl;
			}
		}

		ci::Rectf TextView::getBounds()
		{
			//return ci::Rectf(ci::vec2(0), mCiTextBox.getSize());

			if( mTexture ) {
				return mTexture->getBounds();
			}
			else {
				return ci::Rectf();
			}
		}

		void TextView::setFormat( ci::gl::Texture::Format format )
		{
			mFormat = format;
			mHasFormat = true;
			render();
		}

	}
} //  namespace po::scene