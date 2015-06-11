#pragma once

#include "poNodeContainer.h"
#include "poShape.h"
#include "poTextBox.h"


class TextureSample;
typedef std::shared_ptr<TextureSample> TextureSampleRef;

class TextureSample
: public po::scene::NodeContainer
{
public:
    static TextureSampleRef create();
    
    void setup();
    
protected:
	TextureSample();

private:
    po::scene::ShapeRef     mPlainShape;    // plain rectangle for reference
    po::scene::ShapeRef     mTexShape;      // ellipse that will have texture
    ci::gl::TextureRef      mTexture;       // image texture
    
    po::scene::TextBoxRef   mTexturefitLabel;
    
    void keyDown(ci::app::KeyEvent &event);
};
