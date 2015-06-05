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

#include "poTextBox.h"

namespace po { namespace scene {
    
    TextBoxRef TextBox::create()
    {
        return TextBoxRef(new TextBox(std::shared_ptr<ci::TextBox>(new ci::TextBox())));
    }
    
    TextBoxRef TextBox::create(std::shared_ptr<ci::TextBox> ciTextBox)
    {
        return TextBoxRef(new TextBox(ciTextBox));
    }
    
    TextBox::TextBox(std::shared_ptr<ci::TextBox> ciTextBox)
    : mCiTextBox(ciTextBox)
    , mUseTextBounds(false)
    {
        render();
    }
    
    void TextBox::draw()
    {
        if(mTexture) {
            if(getAppliedAlpha() == 1) {
                ci::gl::enableAlphaBlending(true);
            } else {
                ci::gl::enableAdditiveBlending();
            }
            
            ci::gl::color(1,1,1, getAppliedAlpha());
            ci::gl::draw(mTexture);
        }
    }
    
    void TextBox::render()
    {
        mCiTextBox->setPremultiplied(true);
        mTexture = ci::gl::Texture::create(mCiTextBox->render());
    }
    
    ci::Rectf TextBox::getBounds()
    {
        if(mUseTextBounds) {
            float xPos = 0.0f;
            switch (mCiTextBox->getAlignment()) {
                case ci::TextBox::Alignment::LEFT:
                    xPos = 0;
                    break;
                    
                case ci::TextBox::Alignment::CENTER:
                    xPos = mCiTextBox->getSize().x/2 - mCiTextBox->measure().x/2;
                    break;
                    
                case ci::TextBox::Alignment::RIGHT:
                    xPos = mCiTextBox->getSize().x - mCiTextBox->measure().x;
                    break;
            }
            
            return ci::Rectf(0,0, mCiTextBox->measure().x + xPos, mCiTextBox->measure().y);
        }
        else {
            return ci::Rectf(0,0, mCiTextBox->getSize().x, mCiTextBox->getSize().y);
        }
    }
	
} } //  namespace po::scene