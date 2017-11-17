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

#include "cinder/app/App.h"
#include "poScene.h"
#include "poNodeContainer.h"

namespace po { namespace scene {

    SceneRef Scene::create()
    {
        return create(NodeContainer::create());
    }
	
    SceneRef Scene::create(NodeContainerRef rootNode)
    {
        SceneRef scene(new Scene(rootNode));
        scene->getRootNode()->setScene(scene);
        return scene;
    }
    
    Scene::Scene(NodeContainerRef rootNode)
    : mRootNode(rootNode)
    , mAutoCam(true)
    , eventCenter(EventCenter::create())
    , mFbo(nullptr)
    , mMaskFbo(nullptr)
    {
        createFbos();
        ci::app::getWindow()->getSignalResize().connect(std::bind(&Scene::createFbos, this));
    }
    
    Scene::~Scene()
    {
        mRootNode->removeAllChildren();
        mRootNode = nullptr;
        resetFbos();
    }
    
    void Scene::update()
    {
        //	Send a copy of all over our children to be processed
        //processTrackingQueue();
        eventCenter->processEvents(allChildren);
        
        mRootNode->updateTree();
        
        if (mAutoCam) mCamera.setOrtho(0, (float)ci::app::getWindowWidth(), (float)ci::app::getWindowHeight(), 0, -1, 1);
    }
    
    void Scene::draw()
    {
        drawOrderCounter = 0;
        if (mAutoCam) ci::gl::setMatricesWindow(ci::app::getWindowSize());
        mRootNode->drawTree();
    }
    
    uint32_t Scene::getNextDrawOrder()
    {
        return drawOrderCounter++;
    }
    
    void Scene::setRootNode(NodeContainerRef node)
    {
        if (node) {
            mRootNode->removeScene();
            mRootNode = node;
            mRootNode->setScene(shared_from_this());
        }
    }
    
    
    //------------------------------------
    //	Child Node Tracking
	//------------------------------------
    
    void Scene::trackChildNode(NodeRef node) {
        if (node) {
            //mTrackingQueue[node] = true;
            
            std::vector<NodeRef>::iterator iter = std::find(allChildren.begin(), allChildren.end(), node);
            if (iter == allChildren.end()) {
                allChildren.push_back(node);
            }
        }
    }
    
    void Scene::untrackChildNode(NodeRef node) {
        if (node) {
            //mTrackingQueue[node] = false;
            std::vector<NodeRef>::iterator iter = std::find(allChildren.begin(), allChildren.end(), node);
            if (iter != allChildren.end()) {
                allChildren.erase(iter);
            }
        }
    }
    
    void Scene::processTrackingQueue()
    {
        for (auto &kv : mTrackingQueue) {
            if (kv.first) {
                std::vector<NodeRef>::iterator iter = std::find(allChildren.begin(), allChildren.end(), kv.first);
                if ( kv.second && iter == allChildren.end() ) {
                    allChildren.push_back(kv.first);
                } else {
                    if (iter != allChildren.end()) allChildren.erase(iter);
                }
            }
        }
        
        mTrackingQueue.clear();
    }
    
    
    
    //------------------------------------
    //	Fbos
    //------------------------------------
    
    void Scene::createFbos()
    {
        //	Create the FBO
        ci::gl::Fbo::Format format;
        format.setSamples(1);
        format.enableDepthBuffer(false);
        
        mFbo = ci::gl::Fbo::create(ci::app::getWindowWidth(), ci::app::getWindowHeight(), format);
        mMaskFbo = ci::gl::Fbo::create(ci::app::getWindowWidth(), ci::app::getWindowHeight(), format);
        
//        mFbo->getT
//        mFbo->getColorTexture().setFlipped(true);
//        mMaskFbo->getColorTexture().setFlipped(true);
    }
    
    
    //
    //	Reset the FBO with Cinder bug fix
    //	see https://forum.libcinder.org/topic/constantly-changing-fbo-s-size-without-leak
    //
    void Scene::resetFbos()
    {
        // Reset the FBO
        mFbo.reset();
        mMaskFbo.reset();

    }
    
    
	
} } //  namespace po::scene