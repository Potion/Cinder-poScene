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

#include "ViewContainer.h"
#include "Scene.h"

namespace po { namespace scene {
	
    ViewContainerRef ViewContainer::create(std::string name)
    {
        return std::shared_ptr<ViewContainer>(new ViewContainer(name));
    }

    ViewContainer::ViewContainer(std::string name)
    : View(name)
    {
    }
    
    ViewContainer::ViewContainer()
    : View("")
    {
    }
    
    ViewContainer::~ViewContainer()
    {
        removeAllChildren();
    }
    
	
    //------------------------------------
    //  Scene
	//------------------------------------
    
    void ViewContainer::setScene(SceneRef scene)
    {
        View::setScene(scene);
        for (ViewRef &childView : mChildren) {
            childView->setScene(scene);
        }
    }
    
    void ViewContainer::removeScene()
    {
        View::removeScene();
        for (ViewRef &childView : mChildren) {
            childView->removeScene();
        }
    }
    
    
    //------------------------------------
    //  Children
	//------------------------------------
	
	//
    //  Add Children
	//
	
    ViewContainer &ViewContainer::addChild(ViewRef View)
    {
        setParentAndScene(View);
        mChildren.push_back(View);
        setAlignment(getAlignment());
        calculateMatrices();
        
        return *this;
    }
    
    ViewContainer &ViewContainer::addChildren(std::vector<ViewRef> Views) {
        for (auto &View : Views) {
            setParentAndScene(View);
            mChildren.push_back(View);
        }
        
        setAlignment(getAlignment());
        calculateMatrices();
        
        return *this;
    }
    
    ViewContainer &ViewContainer::addChildAt(int index, ViewRef View)
    {
        setParentAndScene(View);
        mChildren.insert(mChildren.begin() + index, View);
        setAlignment(getAlignment());
        calculateMatrices();
        
        return *this;
    }
    
    ViewContainer &ViewContainer::addChildBefore(ViewRef before, ViewRef View)
    {
        setParentAndScene(View);
        mChildren.insert(mChildren.begin() + getChildIndex(before), View);
        setAlignment(getAlignment());
        calculateMatrices();
        
        return *this;
    }
    
    ViewContainer &ViewContainer::addChildAfter(ViewRef after, ViewRef View)
    {
        setParentAndScene(View);
        mChildren.insert(mChildren.begin() + getChildIndex(after) + 1, View);
        setAlignment(getAlignment());
        calculateMatrices();
        
        return *this;
    }
	
	//
    //  Get Children
    //
	
    std::deque<ViewRef> ViewContainer::getChildren()
    {
        return mChildren;
    };
    
    std::deque<ViewRef> &ViewContainer::getChildrenByReference()
    {
        return mChildren;
    }
    
    bool ViewContainer::hasChildren()
    {
        return (mChildren.size() != 0);
    }
    
    int ViewContainer::getChildIndex(const ViewRef &child)
    {
        std::deque<ViewRef>::iterator iter = std::find(mChildren.begin(), mChildren.end(), child);
        if (iter != mChildren.end()) return (int)std::distance(mChildren.begin(), iter);
        return INVALID_INDEX;
    }
    
    ViewRef ViewContainer::getChildByIndex(int index)
    {
        if ( index < 0 || index >= mChildren.size() ) return ViewRef();
        return *(mChildren.begin() + index);
    }
    
    ViewRef ViewContainer::getChildByUID(uint32_t uid)
    {
        //	Go through our tree to find any View with UID
        for (ViewRef &View : mChildren) {
            ViewContainerRef container = std::dynamic_pointer_cast<ViewContainer>(View);
            if (container) {
                ViewRef foundView = container->getChildByUID(uid);
                if (foundView) return foundView;
            } else {
                if (View->getUID() == uid) return View;
            }
        }
        
        //	See if it is us
        if (mUid == uid) return shared_from_this();
            
        //	Not found
        return ViewRef();
    }
    
    ViewRef ViewContainer::getChildByName(const std::string &name)
    {
        for (ViewRef& View : mChildren) {
            if (View->getName() == name) return View;
        }
        
        return ViewRef();
    }
    
    ViewRef ViewContainer::getFirstChild()
    {
        if (mChildren.empty()) return ViewRef();
        return mChildren.front();
    }
    
    ViewRef ViewContainer::getLastChild()
    {
        if (mChildren.empty()) return ViewRef();
        return mChildren.back();
    }
    
    //
    //  Remove Children
	//
    
    ViewContainer &ViewContainer::removeChild(ViewRef View)
    {
        std::deque<ViewRef>::iterator iter = std::find(mChildren.begin(), mChildren.end(), View);
        if (iter != mChildren.end()) {
            (*iter)->removeParent();
            (*iter)->removeScene();
            
            mChildren.erase(iter);
            
            setAlignment(getAlignment());
            
            return *this;
        } else {
            ViewContainerInvalidChildException exc;
            throw(exc);
        }
    }
    
    ViewContainer &ViewContainer::removeChildAt(int index)
    {
        if ( index <= 0 || index >= mChildren.size() ) {
            ViewContainerInvalidChildException exc;
            throw(exc);
        } else {
            mChildren[index]->removeParent();
            mChildren[index]->removeScene();
            
            mChildren.erase(mChildren.begin() + index);
            
            setAlignment(getAlignment());
            
            return *this;
        }
    }
    
    ViewContainer &ViewContainer::removeAllChildren()
    {
        for (ViewRef &View : mChildren) {
            View->removeParent();
            View->removeScene();
        }
        
        setAlignment(getAlignment());
        
        mChildren.clear();
        
        return *this;
    }
    
    //
    //  Move Children
	//
    
    ViewContainer &ViewContainer::moveChildToFront(ViewRef View)
    {
        auto ViewIter = std::find(mChildren.begin(), mChildren.end(), View);
        if (ViewIter != mChildren.end()) {
            mChildren.erase(ViewIter);
            mChildren.push_back(View);
        }
        
        return *this;
    }
    
    ViewContainer &ViewContainer::moveChildForward(ViewRef View)
    {
        auto ViewIter = std::find(mChildren.begin(), mChildren.end(), View);
        if ( ViewIter != mChildren.end() && *ViewIter != mChildren.back() ) {
            std::iter_swap(ViewIter, ++ViewIter);
        }
        
        return *this;
    }
    
    ViewContainer &ViewContainer::moveChildToBack(ViewRef View)
    {
        auto ViewIter = std::find(mChildren.begin(), mChildren.end(), View);
        if (ViewIter != mChildren.end()) {
            mChildren.erase(ViewIter);
            mChildren.push_front(View);
        }
        
        return *this;
    }
    
    ViewContainer &ViewContainer::moveChildBackward(ViewRef View)
    {
        auto ViewIter = std::find(mChildren.begin(), mChildren.end(), View);
        if ( ViewIter != mChildren.end() && *ViewIter != mChildren.front() ) {
            std::iter_swap(ViewIter, --ViewIter);
        }
        
        return *this;
    }
    
    void ViewContainer::setParentAndScene(ViewRef View)
    {
        //	See if the View is already a child of another View.
		if (View->getParent() != nullptr) {
			View->getParent()->removeChild(View);
		}
        
        //	Assign ourselves as the parent
        View->setParent(std::static_pointer_cast<ViewContainer>(shared_from_this()));
        View->setScene(mScene.lock());
    }
    
    
    //------------------------------------
    //  Update + Draw
	//------------------------------------

    void ViewContainer::updateTree()
    {
        View::updateTree();
        
        //	We have to copy the children, because if any of the update loops remove children
        //	The vector is screwed (invalidated)
        std::deque<ViewRef> children(mChildren);
        
		for (ViewRef &childView : children) {
            if (childView->mVisible && childView->hasParent()) childView->updateTree();
		}
    }

    
    void ViewContainer::draw()
    {
        for (ViewRef &childView : mChildren) {
            childView->drawTree();
        }
    }

	
    //------------------------------------
    //  Dimensions
	//------------------------------------

    ci::Rectf ViewContainer::getBounds()
    {
        //	Reset Bounds
		ci::Rectf bounds = ci::Rectf(0, 0, 0, 0);
		
        for (ViewRef &childView : mChildren) {
			if (childView->mVisible && !childView->getParentShouldIgnoreInBounds()) bounds.include(childView->getFrame());
		}
            
        return bounds;
    }
	
    
    //------------------------------------
    //  Interaction
	//------------------------------------
	
    bool ViewContainer::pointInside(const ci::vec2 &windowPoint)
    {
        for (const ViewRef &View : mChildren) {
            if (!View->mVisible) continue;
            if (View->pointInside(windowPoint)) return true;
        }
        
        return false;
    }
	
    
    //------------------------------------
    //  Matrices/Dimensions
	//------------------------------------
    
    void ViewContainer::matrixTree() {
        beginDrawTree();
        
        for (ViewRef &childView : mChildren) {
            childView->matrixTree();
        }
        
        finishDrawTree();
    }
    
    void ViewContainer::calculateMatrices() {
        if (hasParent()) {
            getParent()->calculateMatrices();
        } else {
            matrixTree();
        }
    }
	
} } //  namespace po::scene