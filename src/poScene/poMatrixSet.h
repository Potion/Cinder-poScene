//
//  poMatrixSet.h
//  BasicScene
//
//  Created by Stephen Varga on 3/23/14.
//
//

#include "cinder/Matrix.h"

namespace po {
    class MatrixSet
    {
    public:
        void set(ci::Matrix44f modelview, ci::Matrix44f projection, ci::Area viewport);
        
        ci::Vec2f globalToLocal(ci::Vec2f point);
        
    private:
        ci::Matrix44f modelview, projection;
        ci::Area viewport;
        
        ci::Vec3f unproject(const ci::Vec3f &pt);
    };
}
