//
//  poImage.h
//  TextBox
//
//  Created by Stephen Varga on 6/4/15.
//
//

#pragma once

#include "poNode.h"

namespace po { namespace scene {
    class ImageShape;
    typedef std::shared_ptr<ImageShape> ImageShapeRef;
    
    class ImageShape
    : public Node
    {
    public:
        static ImageShapeRef create(ci::gl::TextureRef texture);
        
        void draw();
        
    protected:
        ImageShape(ci::gl::TextureRef texture);
        
    private:
        ci::gl::TextureRef mTexture;
    };

} } // Namespace: po::scene