//
//  AppTest.h
//  Test
//
//  Created by Stephen Varga on 3/18/14.
//
//

#pragma once

#include "CinderCatchApp.h"

class AppTest : public testing::Test
{
public:
	void SetUp();
	void TearDown();
protected:
	CinderGTestApp* mApp;
};
