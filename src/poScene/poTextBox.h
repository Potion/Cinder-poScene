//
//  poTextBox.h
//  BasicScene
//
//  Created by Stephen Varga on 3/29/14.
//
//

#pragma once

#include "cinder/Text.h"
#include "cinder/gl/Texture.h"

#include "poNode.h"


namespace po {
    class TextBox;
    typedef std::shared_ptr<TextBox> TextBoxRef;
    
    class TextBox
    : public Node
    , public ci::TextBox
    {
    public:
        static TextBoxRef create();
        
        virtual void draw();
        
        void clear();
        
        //Renders to an internal TextureRef for drawing
        ci::Surface render();
        ci::gl::TextureRef getTexure()  { return mTexture; };
        
        ci::Rectf getBounds();
        void        setSize(ci::Vec2i size);
        
        //Override ci::TextBox Functions so we can return the po::Textbox instead of base class
        po::TextBox& size(ci::Vec2i sz) { ci::TextBox::setSize(sz);                                        return *this; }
        po::TextBox& text(const std::string &t) { ci::TextBox::setText(t);                                 return *this; }
        po::TextBox& font(const ci::Font &f) { setFont(f);                                                 return *this; }
        po::TextBox& textAlignment(ci::TextBox::Alignment align) { ci::TextBox::setAlignment( align );     return *this; }
        po::TextBox& premultiplied(bool premult = true) { ci::TextBox::setPremultiplied( premult );        return *this; }
        po::TextBox& ligate(bool ligateText = true) { ci::TextBox::setLigate( ligateText );                return *this; }
        po::TextBox& color(ci::Color color) { setColor(color);                                             return *this; };
        po::TextBox& color(float r, float g, float b) { setColor(ci::Color(r,g,b));                        return *this; };
        
        po::TextBox& useTextBounds(bool useTextBounds) { setUseTextBounds(useTextBounds); return *this; };
        void setUseTextBounds(bool useTextBounds) { mUseTextBounds = useTextBounds; };
    protected:
        TextBox();
        
    private:
        ci::gl::TextureRef mTexture;
        bool mUseTextBounds;
    };
};