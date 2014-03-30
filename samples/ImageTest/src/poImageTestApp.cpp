//
//  poImageTestApp.cpp
//  ImageTest
//
//  Created by Stephen Varga on 3/30/14.
//
//

#include "poImageTestApp.h"

poImageTestAppRef poImageTestApp::create() {
    poImageTestAppRef app(new poImageTestApp());
    app->setup();
    return app;
}


void poImageTestApp::setup() {
    
}


