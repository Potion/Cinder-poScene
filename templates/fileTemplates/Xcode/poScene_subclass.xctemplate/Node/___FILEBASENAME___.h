#pragma once

/*	Created by ___FULLUSERNAME___ on ___DATE___.
 *	Copyright ___YEAR___ ___ORGANIZATIONNAME___. All rights reserved.
 */

#include "poNode.h"

class ___FILEBASENAME___;
typedef std::shared_ptr<___FILEBASENAME___> ___FILEBASENAME___Ref;

class ___FILEBASENAME___
: public po::scene::Node
{
public:
	static ___FILEBASENAME___Ref create();
	virtual ~___FILEBASENAME___();

	virtual void setup();

protected:
	___FILEBASENAME___();
	
	virtual void update();
    virtual void draw();

private:

};