//
//  poScene.h
//  BasicTest
//
//  Created by Stephen Varga on 3/17/14.
//
//

#pragma once

#include "poNode.h"

namespace po {
    //Forward declare node
    class Node;
    typedef std::shared_ptr<Node> NodeRef;
    
    class Scene;
    typedef std::shared_ptr<Scene> SceneRef;
    
    class Scene
    {
    public:
        static SceneRef create();
        
        Scene();
        ~Scene();
        
        virtual void update();
        virtual void draw();
        
        //Root Node
        NodeRef getRootNode();
        
        //Each object get's its own draw order every frame.
        //This lets us sort objects for hit testing
        uint getNextDrawOrder();
        
    protected:
        //Root node of scene
        NodeRef rootNode;
        
        uint drawOrderCounter;
    };
}
