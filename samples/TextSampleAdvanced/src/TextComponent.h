#pragma once

#include "cinder/Text.h"

#include "poScene/View.h"
#include "poScene/TextView.h"

using namespace po::scene;

class TextComponent;
typedef std::shared_ptr<TextComponent> TextComponentRef;

class TextComponent
: public po::scene::View
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
	TextViewRef mTextBox;
	
	//	Get the placeholder text
	std::string generateText();
 
};