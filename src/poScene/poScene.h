//
//  poScene.h
//  BasicTest
//
//  Created by Stephen Varga on 3/17/14.
//
//

#include "poNode.h"

#pragma once

namespace po {
    class Scene;
    typedef std::shared_ptr<Scene> SceneRef;
    
    class Scene
    {
    public:
        static SceneRef create();
        
        Scene();
        ~Scene();
        
        void update();
        void draw();
        
        //Root Node
        NodeRef getRootNode();
        
    protected:
        //Root node of scene
        po::NodeRef rootNode;
    };
}
