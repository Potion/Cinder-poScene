//
//  poMatrixSet.cpp
//  BasicScene
//
//  Created by Stephen Varga on 3/23/14.
//
//

#include "poMatrixSet.h"

#if defined( CINDER_MSW )
#include <windows.h>
#undef min
#undef max
#include <gl/gl.h>
#elif defined( CINDER_COCOA_TOUCH )
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#elif defined( CINDER_MAC )
#include <OpenGL/gl.h>
#endif

namespace po {
    void MatrixSet::set(ci::Matrix44f modelview, ci::Matrix44f projection, ci::Area viewport)
    {
        this->modelview     = modelview;
        this->projection    = projection;
        this->viewport      = viewport;
    }
    
    ci::Vec2f MatrixSet::globalToLocal(ci::Vec2f point) {
        ci::Vec3f p(point.x, viewport.getHeight() - point.y, 0.f);
        ci::Vec3f r = unproject(p);
        return ci::Vec2f(r.x, r.y);
    }
    
    
    //Adapted from code by Paul Houx https://forum.libcinder.org/topic/glu-s-gluunproject-substitute
    ci::Vec3f MatrixSet::unproject(const ci::Vec3f &pt)
    {
        /* find the inverse modelview-projection-matrix */
        ci::Matrix44f a = projection * modelview;
        a.invert(0.0f);
        
        /* transform to normalized coordinates in the range [-1, 1] */
        ci::Vec4f in;
        in.x = (pt.x - viewport.getX1())/viewport.getWidth()*2.0f-1.0f;
        in.y = (pt.y - viewport.getY1())/viewport.getHeight()*2.0f-1.0f;
        in.z = 2.0f * pt.z - 1.0f;
        in.w = 1.0f;
        
        /* find the object's coordinates */
        ci::Vec4f out = a * in;
        if(out.w != 0.0f) out.w = 1.0f / out.w;
        
        /* calculate output */
        ci::Vec3f result;
        result.x = out.x * out.w;
        result.y = out.y * out.w;
        result.z = out.z * out.w;
        
        return result;
    }

}