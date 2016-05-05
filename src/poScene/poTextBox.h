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

#include "cinder/Text.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/draw.h"
#include "poNode.h"

namespace po { namespace scene {
    // TextBoxes are Node wrappers for Cinder's ci::TextBox class.
    // All text settings should be done on a ci::TextBox object,
    // which can then be attached to this object for rendering as a texture
    // and hit testing.
	
	class TextBox;
	typedef std::shared_ptr<TextBox> TextBoxRef;
	
	class TextBox
	: public Node
	{
	public:
        //! Create with a ci::TextBox
		static TextBoxRef create(ci::TextBox ciTextBox);
        //! Create with a default (blank) ci::TextBox
		static TextBoxRef create();
		//! Create with a ci::TextBox and Texture Format for rendering
		static TextBoxRef create(ci::TextBox ciTextBox, ci::gl::Texture::Format format);
		
        //! Draw the texture generated from the ci::TextBox
		void draw();
		
        //! Return the bounds based on the ci::TextBox's bounds + offset/alignment
		ci::Rectf getBounds();
        
        //! Set the ci::TextBox object (by copying) and render to a texture
        void setCiTextBox(ci::TextBox &ciTextBox);
		
        //! Get a copy of the backing ci::TextBox for manipulation
        ci::TextBox getCiTextBoxCopy() { return mCiTextBox; };

		// CB - for calling functions like measure()
        const ci::TextBox& getCiTextBox() const { return mCiTextBox; };


		//! Set the texture format to use when rendering to texture
		void setFormat(ci::gl::Texture::Format format);
		
	protected:
		TextBox(ci::TextBox ciTextBox);
		
	private:
        void render();
		ci::TextBox mCiTextBox;
		ci::gl::TextureRef mTexture;
		bool mUseTextBounds;
		ci::gl::Texture::Format mFormat;
		bool mHasFormat;
		
	};
		
} } //  namespace po::scene