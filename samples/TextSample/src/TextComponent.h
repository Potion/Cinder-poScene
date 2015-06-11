#pragma once

#include "poNodeContainer.h"
#include "cinder/Text.h"
#include "poTextBox.h"

using namespace po::scene;

class TextComponent;
typedef std::shared_ptr<TextComponent> TextComponentRef;

class TextComponent
: public po::scene::NodeContainer
{
public:
	static TextComponentRef create();
	virtual ~TextComponent();

	virtual void setup();

protected:
	TextComponent();

private:
	ci::TextBox mCiTextBox;
	TextBoxRef mTextBox;
	
	std::string generateText();
 
};