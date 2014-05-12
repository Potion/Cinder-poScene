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

#include "cinder/Camera.h"

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
        static SceneRef create(NodeContainerRef rootNode);
        ~Scene();
        
        ci::CameraOrtho& getCamera() { return mCamera; }
        void setAutoCam(bool autoCam) { mAutoCam = autoCam; }
        bool getAutoCam() { return mAutoCam; };
        
        virtual void update();
        virtual void draw();
        
        //Probably should not call this if you're not doing a custom draw tree!
        uint32_t getNextDrawOrder();
        
        //Root Node
        NodeContainerRef getRootNode() { return rootNode; };
        void setRootNode(NodeContainerRef node);
        
    protected:
        Scene(NodeContainerRef rootNode);
        
        //Root node of scene
        NodeContainerRef rootNode;
        
        //Our Event Center (each scene has their own)
        EventCenterRef eventCenter;
        
        //Reference to all our our children
        void trackChildNode(NodeRef node);
        void untrackChildNode(NodeRef node);
        std::vector<NodeRef> allChildren;
        
    private:
        //Each object get's its own draw order every frame.
        //This lets us sort objects for hit testing
        uint32_t drawOrderCounter;
        
        ci::CameraOrtho mCamera;
        bool mAutoCam;
        
        //Queue to track/untrack children on the next frame
        //This lets us pass our children to the event center by reference but not risk
        //Modifying the vector during iteration (i.e. add/remove child during event callback)
        struct TrackedNode {
            TrackedNode(NodeRef &node, bool bTrack) : node(node), bTrack(bTrack) {};
            NodeRef node;
            bool bTrack;
        };
        
        void processTrackingQueue();
        std::vector<TrackedNode> mTrackingQueue;
    };
}
