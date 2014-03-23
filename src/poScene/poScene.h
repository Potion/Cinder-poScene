//
//  poScene.h
//  BasicTest
//
//  Created by Stephen Varga on 3/17/14.
//
//

#pragma once

#include "poNodeContainer.h"
#include "poEventCenter.h"

namespace po {
    //Forward declare node
    class Node;
    typedef std::shared_ptr<Node> NodeRef;
    
    //Create SceneRef typedef
    class Scene;
    typedef std::shared_ptr<Scene> SceneRef;
    
    class Scene
    : public std::enable_shared_from_this<Scene>
    {
        friend class po::Node;
        friend class po::EventCenter; //So we can access the child nodes
        
    public:
        static SceneRef create();
        ~Scene();
        
        virtual void update();
        virtual void draw();
        
        //Root Node
        NodeContainerRef getRootNode();
        
        //Each object get's its own draw order every frame.
        //This lets us sort objects for hit testing
        uint getNextDrawOrder();
        
    protected:
        Scene();
        
        //Root node of scene
        NodeContainerRef rootNode;
        
        //Our Event Center (each scene has their own)
        EventCenterRef eventCenter;
        
        //Reference to all our our children
        void trackChildNode(NodeRef node);
        void untrackChildNode(NodeRef node);
        std::vector<NodeRef> allChildren;
        
        //Item draw order
        uint drawOrderCounter;
    private:
    };
}
