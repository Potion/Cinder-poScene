#pragma once

#include "poView.h"

class ___FILEBASENAME___;
typedef std::shared_ptr<___FILEBASENAME___> ___FILEBASENAME___Ref;

class ___FILEBASENAME___
: public po::scene::View
{
public:
	static ___FILEBASENAME___Ref create();
	virtual ~___FILEBASENAME___();

	void setup() override;

protected:
	___FILEBASENAME___();

	void update() override;
    void draw() override;

private:

};