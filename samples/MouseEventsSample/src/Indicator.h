#pragma once

/*	Created by bruce on 6/9/15.
 *	Copyright 2015 __MyCompanyName__. All rights reserved.
 */

#include "poNodeContainer.h"

class Indicator;
typedef std::shared_ptr<Indicator> IndicatorRef;

class Indicator
: public po::scene::NodeContainer
{
public:
	static IndicatorRef create();
	virtual ~Indicator();

	virtual void setup();

protected:
	Indicator();

private:
 
};