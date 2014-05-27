//
//  poTextBox.cpp
//  BasicScene
//
//  Created by Stephen Varga on 3/29/14.
//
//

#include "poTextBox.h"

namespace po {
    TextBoxRef TextBox::create()
    {
        return TextBoxRef(new TextBox());
    }
    
    TextBox::TextBox()
    : mUseTextBounds(false)
    {
        
    }
    
    void TextBox::draw()
    {
        if(mTexture) {
            #pragma message "This needs to be looked at, might affect other stuff"
            if(getAppliedAlpha() == 1) {
                ci::gl::enableAlphaBlending(true);
            } else {
                ci::gl::enableAdditiveBlending();
            }
            
            ci::gl::color(1,1,1, getAppliedAlpha());
            ci::gl::draw(mTexture);
        }
    }
    
    ci::Surface TextBox::render()
    {
        ci::TextBox::setPremultiplied(true);
        ci::Surface surface = ci::TextBox::render();
        mTexture            = ci::gl::Texture::create(surface);
        
        return surface;
    }
    
    ci::Rectf TextBox::getBounds()
    {
        if(mUseTextBounds) {
            float xPos;
            switch (mAlign) {
                case ci::TextBox::Alignment::LEFT:
                    xPos = 0; break;
                case ci::TextBox::Alignment::CENTER:
                    xPos = getSize().x/2 - measure().x/2; break;
                case ci::TextBox::Alignment::RIGHT:
                    xPos = getSize().x - measure().x; break;
            }
            
            return ci::Rectf(0,0, measure().x + xPos, measure().y);
        }
        else
            return ci::Rectf(0,0,getSize().x, getSize().y);
    }
}