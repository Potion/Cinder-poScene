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

#include "poScene/Scene.h"
#include "poScene/View.h"

namespace po { namespace scene {

    SceneRef Scene::create()
    {
        return create(ViewController::create());
    }
	
    SceneRef Scene::create(ViewControllerRef rootViewController)
    {
        SceneRef scene(new Scene(rootViewController));
		scene->setRootViewController(rootViewController);
        return scene;
    }
    
    Scene::Scene(ViewControllerRef rootViewController)
    : mRootViewController(rootViewController)
    , mAutoCam(true)
    , mEventCenter(EventCenter::create())
    , mFbo(nullptr)
    , mMaskFbo(nullptr)
    {
        createFbos();
        mConnWindowResize = ci::app::getWindow()->getSignalResize().connect(std::bind(&Scene::createFbos, this));
    }
    
    Scene::~Scene()
    {
        resetFbos();
    }
    
    void Scene::update()
    {
        //	Send a copy of all over our children to be processed
        //processTrackingQueue();
        mEventCenter->processEvents(allViews);
        
        mRootViewController->getView()->updateTree();
        
        if (mAutoCam) mCamera.setOrtho(0, (float)ci::app::getWindowWidth(), (float)ci::app::getWindowHeight(), 0, -1, 1);
    }
    
    void Scene::draw()
    {
        drawOrderCounter = 0;
        if (mAutoCam) ci::gl::setMatricesWindow(ci::app::getWindowSize());
        mRootViewController->getView()->drawTree();
    }
    
    uint32_t Scene::getNextDrawOrder()
    {
        return drawOrderCounter++;
    }
    
    void Scene::setRootViewController(ViewControllerRef viewController)
    {
        if (viewController) {
            mRootViewController->getView()->removeScene();
            mRootViewController = viewController;
            mRootViewController->getView()->setScene(shared_from_this());
        }
    }
    
    //------------------------------------
    //	Child View Tracking
	//------------------------------------
    
    void Scene::trackView(ViewRef View) {
        if (View) {
            //mTrackingQueue[View] = true;
            
            std::vector<ViewRef>::iterator iter = std::find(allViews.begin(), allViews.end(), View);
            if (iter == allViews.end()) {
                allViews.push_back(View);
            }
        }
    }
    
    void Scene::untrackView(ViewRef View) {
        if (View) {
            //mTrackingQueue[View] = false;
            std::vector<ViewRef>::iterator iter = std::find(allViews.begin(), allViews.end(), View);
            if (iter != allViews.end()) {
                allViews.erase(iter);
            }
        }
    }
    
    void Scene::processTrackingQueue()
    {
        for (auto &kv : mTrackingQueue) {
            if (kv.first) {
                std::vector<ViewRef>::iterator iter = std::find(allViews.begin(), allViews.end(), kv.first);
                if ( kv.second && iter == allViews.end() ) {
                    allViews.push_back(kv.first);
                } else {
                    if (iter != allViews.end()) allViews.erase(iter);
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
		// If the window width or height is 0 (probably minimized) we can't create FBOs so return
		if ((ci::app::getWindowWidth() == 0) | (ci::app::getWindowHeight() == 0)) {
			return;
		}
		
		// If the size is the same as our FBO do nothing (i.e. open window from minimize)
		if (mFbo && ci::app::getWindowWidth() == mFbo->getWidth() && ci::app::getWindowHeight() == mFbo->getHeight()) {
			return;
		}

        //	Create the FBO
        ci::gl::Fbo::Format format;
        format.setSamples(1);
        format.enableDepthBuffer(false);
		format.setColorTextureFormat(ci::gl::Texture2d::Format().internalFormat(GL_RGBA32F));
        
        mFbo = ci::gl::Fbo::create(ci::app::getWindowWidth(), ci::app::getWindowHeight(), format);
        mMaskFbo = ci::gl::Fbo::create(ci::app::getWindowWidth(), ci::app::getWindowHeight(), format);
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