#pragma once

/*	Created by bruce on 6/11/15.
 *	Copyright 2015 __MyCompanyName__. All rights reserved.
 */

#include "poNodeContainer.h"

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
 
};