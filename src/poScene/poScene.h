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

namespace po { namespace scene {
    //Forward declare node
    class Node;
    typedef std::shared_ptr<Node> NodeRef;
    
    //Create SceneRef typedef
    class Scene;
    typedef std::shared_ptr<Scene> SceneRef;
    
    class Scene
    : public std::enable_shared_from_this<Scene>
    {
        friend class Node;
        friend class EventCenter; //So we can access the child nodes
        
    public:
        static SceneRef create();
        static SceneRef create(NodeContainerRef rootNode);
        ~Scene();
        
        ci::CameraOrtho& getCamera() { return mCamera; }
        void setAutoCam(bool autoCam) { mAutoCam = autoCam; }
        bool getAutoCam() { return mAutoCam; };
        
        virtual void update();
        virtual void draw();
        
        uint32_t getNextDrawOrder();
        
        //Root Node
        NodeContainerRef getRootNode() { return mRootNode; };
        void setRootNode(NodeContainerRef node);
                
    protected:
        Scene(NodeContainerRef rootNode);
        
        //Root node of scene
        NodeContainerRef mRootNode;
        
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
        //This lets us pass our children to the event center by reference (copying super slow)
        //but not risk Modifying the vector during iteration (i.e. add/remove child during event callback)
        
        void processTrackingQueue();
        std::map<NodeRef, bool> mTrackingQueue;
    };
} } //  Namespace: po::scene