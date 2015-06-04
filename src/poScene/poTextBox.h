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


namespace po { namespace scene {
    class TextBox;
    typedef std::shared_ptr<TextBox> TextBoxRef;
    
    class TextBox
    : public Node
    {
    public:
        static TextBoxRef create(std::shared_ptr<ci::TextBox> ciTextBox);
        static TextBoxRef create();
        
        
        void render();
        void draw();
        
        ci::Rectf getBounds();
        
        void setCiTextBox(std::shared_ptr<ci::TextBox> ciTextBox);
        std::shared_ptr<ci::TextBox> getCiTextBox();
        
    protected:
        TextBox();
        TextBox(std::shared_ptr<ci::TextBox> ciTextBox);
        
    private:
        std::shared_ptr<ci::TextBox> mCiTextBox;
        ci::gl::TextureRef mTexture;
        bool mUseTextBounds;
    };
    
    
//    class TextBox
//    : public Node
//    , public ci::TextBox
//    {
//    public:
//        static TextBoxRef create();
//        
//        virtual void draw();
//        
//        void clear();
//        
//        //Renders to an internal TextureRef for drawing
//        ci::Surface render();
//        ci::gl::TextureRef getTexure()  { return mTexture; };
//        
//        ci::Rectf getBounds();
//        void        setSize(ci::Vec2i size);
//        
//        //Override ci::TextBox Functions so we can return the po::Textbox instead of base class
//        TextBox& size(ci::Vec2i sz) { ci::TextBox::setSize(sz);                                        return *this; }
//        TextBox& text(const std::string &t) { ci::TextBox::setText(t);                                 return *this; }
//        TextBox& font(const ci::Font &f) { setFont(f);                                                 return *this; }
//        TextBox& textAlignment(ci::TextBox::Alignment align) { ci::TextBox::setAlignment( align );     return *this; }
//        TextBox& premultiplied(bool premult = true) { ci::TextBox::setPremultiplied( premult );        return *this; }
//        TextBox& ligate(bool ligateText = true) { ci::TextBox::setLigate( ligateText );                return *this; }
//        TextBox& color(ci::Color color) { setColor(color);                                             return *this; };
//        TextBox& color(float r, float g, float b) { setColor(ci::Color(r,g,b));                        return *this; };
//        
//        TextBox& useTextBounds(bool useTextBounds) { setUseTextBounds(useTextBounds); return *this; };
//        void setUseTextBounds(bool useTextBounds) { mUseTextBounds = useTextBounds; };
//    protected:
//        TextBox();
//        
//    private:
//        ci::gl::TextureRef mTexture;
//        bool mUseTextBounds;
//    };
} } //  Namespace: po::scene