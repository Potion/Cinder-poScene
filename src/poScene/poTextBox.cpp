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
    {
        
    }
    
    void TextBox::draw()
    {
        if(mTexture) {
            ci::gl::enableAlphaBlending(true);
            ci::gl::color(1,1,1);
            ci::gl::draw(mTexture);
        }
    }
    
    ci::Surface TextBox::render()
    {
        ci::TextBox::setPremultiplied(true);
        ci::Surface surface = ci::TextBox::render();
        mTexture = ci::gl::Texture::create(surface);
        return surface;
    }
    
    ci::Rectf TextBox::getBounds()
    {
        return ci::Rectf(0,0,mCalculatedSize.x, mCalculatedSize.y);
    }
}