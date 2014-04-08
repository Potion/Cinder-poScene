//
//  poMatrixSet.h
//  BasicScene
//
//  Created by Stephen Varga on 3/23/14.
//
//

#include "cinder/Area.h"
#include "cinder/Matrix.h"
#include "cinder/Camera.h"

namespace po {
    class MatrixSet
    {
    public:
        void set(ci::Matrix44f modelview, ci::Matrix44f projection, ci::Area viewport);
        
        ci::Vec2f globalToLocal(const ci::Vec2f &point);
        ci::Vec2f localToGlobal(const ci::CameraOrtho &camera, const ci::Vec2f &point);
        
    private:
        ci::Matrix44f mModelview, mProjection;
        ci::Area mViewport;
        
        ci::Vec3f project(const ci::Vec3f &point);
        ci::Vec3f unproject(const ci::Vec3f &point);
    };
}
