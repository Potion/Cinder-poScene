//
//  CinderCatch.h
//  Test
//
//  Created by Stephen Varga on 3/18/14.
//
//

#pragma once

#include "catch.hpp"
#include "cinder/app/AppBasic.h"


#if defined( CINDER_MAC )
    #define CINDER_APP_CATCH( APP, RENDERER )                                                                   \
    int main (int argc, char * const argv[]) {                                                                  \
        return Catch::Session().run( argc, argv );                                                              \
    }                                                                                                           \
#elif defined( CINDER_MSW )
    #include "Shellapi.h"
    #define CINDER_APP_CATCH( APP, RENDERER )                                                                   \
    int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow) {              \
        int argc;																								\
        wchar_t** argv = CommandLineToArgvW( GetCommandLineW(), &argc );										\
        return Catch::Session().run( argc, argv );																\
    }
#endif