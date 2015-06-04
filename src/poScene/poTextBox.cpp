//
//  poTextBox.cpp
//  BasicScene
//
//  Created by Stephen Varga on 3/29/14.
//
//

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
    }
} } //  Namespace: po::scene