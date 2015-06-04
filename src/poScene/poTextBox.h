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
        TextBox(std::shared_ptr<ci::TextBox> ciTextBox);
        
    private:
        std::shared_ptr<ci::TextBox> mCiTextBox;
        ci::gl::TextureRef mTexture;
        bool mUseTextBounds;
    };
} } //  Namespace: po::scene