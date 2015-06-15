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
	//	Reference to ci textbox
	ci::TextBox mCiTextBox;
	
	//	Reference to the textbox node
	TextBoxRef mTextBox;
	
	//	Get the placeholder text
	std::string generateText();
 
};