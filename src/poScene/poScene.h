/*
 Copyright (c) 2015, Potion Design LLC
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 
 * Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.
 
 * Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 
 * Neither the name of copyright holder nor the names of its
 contributors may be used to endorse or promote products derived from
 this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include "poNodeContainer.h"
#include "poEventCenter.h"
#include "cinder/Camera.h"

namespace po { namespace scene {
	
    //	Forward declare node
    class Node;
    typedef std::shared_ptr<Node> NodeRef;
    
    //	Create SceneRef typedef
    class Scene;
    typedef std::shared_ptr<Scene> SceneRef;
    
    class Scene
    : public std::enable_shared_from_this<Scene>
    {
        friend class Node;
        friend class EventCenter; // So we can access the child nodes
        
    public:
        static SceneRef create();
        static SceneRef create(NodeContainerRef rootNode);
        ~Scene();
        
        ci::CameraOrtho &getCamera() { return mCamera; }
        void setAutoCam(bool autoCam) { mAutoCam = autoCam; }
        bool getAutoCam() { return mAutoCam; };
        
        virtual void update();
        virtual void draw();
        
        uint32_t getNextDrawOrder();
        
        //	Root Node
        NodeContainerRef getRootNode() { return mRootNode; };
        void setRootNode(NodeContainerRef node);
                
    protected:
        Scene(NodeContainerRef rootNode);
        
        //	Root node of scene
        NodeContainerRef mRootNode;
        
        //	Our Event Center (each scene has their own)
        EventCenterRef eventCenter;
        
        //	Reference to all our our children
        void trackChildNode(NodeRef node);
        void untrackChildNode(NodeRef node);
        std::vector<NodeRef> allChildren;
        
    private:
		//
        //	Each object get's its own draw order every frame.
        //	This lets us sort objects for hit testing
		//
        uint32_t drawOrderCounter;
        
        ci::CameraOrtho mCamera;
        bool mAutoCam;
		
		//
        //	Queue to track/untrack children on the next frame
        //	This lets us pass our children to the event center by reference (copying super slow)
        //	but not risk Modifying the vector during iteration (i.e. add/remove child during event callback)
		//
        void processTrackingQueue();
        std::map<NodeRef, bool> mTrackingQueue;
		
    };
	
} } //  namespace po::scene