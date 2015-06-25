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

#define GLSL(src) "#version 110\n" #src

#if defined( CINDER_MSW )
	#include <windows.h>
	#undef min
	#undef max
	#include <gl/gl.h>
#elif defined( CINDER_COCOA_TOUCH )
	#include <OpenGLES/ES1/gl.h>
	#include <OpenGLES/ES1/glext.h>
#elif defined( CINDER_MAC )
	#include <OpenGL/gl.h>
#endif

#include "cinder/CinderMath.h"
#include "Resources.h"
#include "poNode.h"
#include "poNodeContainer.h"
#include "poShape.h"
#include "poScene.h"

namespace po { namespace scene {
    
    static uint32_t OBJECT_UID		= 0;
    static const int ORIGIN_SIZE	= 2;
    
    
    // Masking Shader
    static ci::gl::GlslProgRef mMaskShader = nullptr;
    
    static const char *maskVertShader = GLSL(
        void main()
        {
            gl_Position     = gl_ModelViewProjectionMatrix * gl_Vertex;
            gl_TexCoord[0]  = gl_MultiTexCoord0;
        }
    );
    
    static const char *maskFragShader = GLSL(
        uniform sampler2D tex;
        uniform sampler2D mask;

        void main(void)
        {
            vec2 c0 = vec2(gl_TexCoord[0].s, 1.0 - gl_TexCoord[0].t);
            
            vec4 rgbValue       = texture2D(tex, c0);
            vec4 alphaValue     = texture2D(mask, c0);
            
            gl_FragColor.rgb    = rgbValue.rgb;
            gl_FragColor.a = alphaValue.a;
            
            //gl_FragColor = alphaValue;
        }
    );

    Node::Node(std::string name)
    : mUid(OBJECT_UID++)
    , mName(name)
    , mDrawOrder(0)
    , mPosition(0.f, 0.f)
    , mScale(1.f, 1.f)
    , mRotation(0)
    , mOffset(0.f,0.f)
    , mAlpha(1.f)
    , mAppliedAlpha(1.f)
    , mPositionAnim(ci::Vec2f(0.f, 0.f))
    , mScaleAnim(ci::Vec2f(1.f, 1.f))
    , mRotationAnim(0)
    , mOffsetAnim(ci::Vec2f(0.f, 0.f))
    , mAlphaAnim(1.f)
    , mAlignment(Alignment::TOP_LEFT)
    , mMatrixOrder(MatrixOrder::TRS)
    , mFillColor(1.f, 1.f, 1.f)
    , mFillColorAnim(ci::Color(1.f, 1.f, 1.f))
    , mFillEnabled(true)
    , mStrokeColor(255, 255, 255)
    , mStrokeEnabled(false)
    , mUpdatePositionFromAnim(false)
    , mUpdateScaleFromAnim(false)
    , mUpdateRotationFromAnim(false)
    , mUpdateOffsetFromAnim(false)
    , mUpdateAlphaFromAnim(false)
    , mUpdateFillColorFromAnim(false)
    , mDrawBounds(false)
    , mBoundsColor(1.f, 0, 0)
    , mParentShouldIgnoreInBounds(false)
    , mBoundsDirty(true)
    , mFrameDirty(true)
    , mVisible(true)
    , mInteractionEnabled(true)
    , mHasScene(false)
    , mHasParent(false)
    , mIsMasked(false)
    , mMask(nullptr)
    {
        //	Initialize our animations
        initAttrAnimations();
    }
    
    Node::~Node()
	{
		//	Make sure to clear the fbo w/Cinder bug fix
        removeParent();
        removeScene();
        disconnectAllSignals();
    }
    
    
    //------------------------------------
    //	Update & Draw Trees
	//------------------------------------
    
    void Node::updateTree()
    {
        //	Update our tween tie-in animations
        updateAttributeAnimations();
        
        if (mIsMasked) mMask->updateTree();
        
        //	Call our update function
        update();
    }
	
    void Node::beginDrawTree()
    {
        //	Update our draw order
        if (hasScene()) mDrawOrder = mScene.lock()->getNextDrawOrder();
        
        //	Set applied alpha
		if (hasParent()) {
            mAppliedAlpha = getParent()->getAppliedAlpha() * mAlpha;
		} else {
            mAppliedAlpha = mAlpha;
		}
		
        //	Push our Matrix
        ci::gl::pushModelView();
        setTransformation();
    }
	
    void Node::drawTree()
    {
        if (mVisible) {
            
            //  Draw
            beginDrawTree();
            
            if (!mIsMasked) {
                draw();
                finishDrawTree();
            } else {
                captureMasked();
                finishDrawTree();
                drawMasked();
            }
        }
    }
	
    void Node::finishDrawTree()
    {
        //	Draw bounds if necessary
        if (mDrawBounds) drawBounds();
        
        //	Pop our Matrix
        ci::gl::popModelView();
    }
	
    void Node::matrixTree()
    {
        beginDrawTree();
        finishDrawTree();
    }
    
    
    //------------------------------------
    //	Fbo Drawing
	//------------------------------------
    
    
    void Node::captureMasked()
    {
        ci::gl::SaveFramebufferBinding binding;
        //	Save the window buffer
        {
            
            //  Draw ourself into FBO
            getScene()->getWindowFbo()->bindFramebuffer();
            ci::gl::clear();
            draw();
        }
        
        {
            //  Draw mask into Masking FBO (replace with stencil buffer in GLNext)
            getScene()->getStencilFbo()->bindFramebuffer();
            ci::gl::clear(ci::ColorA(0.0f, 0.0f, 0.0f, 0.0f));
            ci::gl::pushModelView();
            //ci::gl::translate(-getPosition());
            mMask->drawTree();
            ci::gl::popModelView();
        }
    }
    
    void Node::drawMasked()
    {
        ci::gl::enableAlphaBlending();
        
        getScene()->getWindowFbo()->getTexture().setFlipped(true);
        getScene()->getStencilFbo()->getTexture().setFlipped(true);
        
        //ci::gl::draw(getScene()->getWindowFbo()->getTexture());
//        ci::gl::draw(getScene()->getStencilFbo()->getTexture());
//        return;
        
        // Bind FBO textures
        getScene()->getWindowFbo()->getTexture().bind(0);
        getScene()->getStencilFbo()->getTexture().bind(1);

        //	Bind Shader
        mMaskShader->bind();

        //	Set uniforms
        mMaskShader->uniform("tex", 0);
        mMaskShader->uniform("mask", 1);
//        mMaskShader->uniform ( "contentScale", ci::Vec2f((float)tex.getWidth() / (float)mMask->getWidth(), (float)tex.getHeight() / (float)mMask->getHeight() ) );
//        //mMaskShader.uniform ( "maskPosition", ci::Vec2f(0.f, 0.f));
//        mMaskShader->uniform ( "maskPosition", mMask->getPosition()/ci::Vec2f(mFbo->getWidth(), mFbo->getHeight()) );

        //	Draw
        ci::gl::drawSolidRect(getScene()->getWindowFbo()->getBounds());

        //	Restore everything
        getScene()->getWindowFbo()->getTexture().unbind();
        getScene()->getStencilFbo()->getTexture().unbind();
        mMaskShader->unbind();
    }
    
    //
    //	Draw the fbo
	//
//    void Node::drawFbo()
//    {
//        //	The fbo has premultiplied alpha, so we draw at full color
//        ci::gl::enableAlphaBlending();
//        ci::gl::color(ci::ColorAf::white());
//        
//        //	Flip the FBO texture since it's coords are reversed
//        ci::gl::Texture tex = mFbo->getTexture();
//        tex.setFlipped(true);
//        
//        if (mIsMasked) {
//            //	Use masking shader to draw FBO with mask
//            //	Bind the fbo and mask texture
//            tex.bind(0);
//            mMask->getTexture()->bind(1);
//            
//            //	Bind Shader
//            mMaskShader->bind();
//            
//            //	Set uniforms
//            mMaskShader->uniform("tex", 0);
//            mMaskShader->uniform("mask", 1);
//            mMaskShader->uniform ( "contentScale", ci::Vec2f((float)tex.getWidth() / (float)mMask->getWidth(), (float)tex.getHeight() / (float)mMask->getHeight() ) );
//            //mMaskShader.uniform ( "maskPosition", ci::Vec2f(0.f, 0.f));
//            mMaskShader->uniform ( "maskPosition", mMask->getPosition()/ci::Vec2f(mFbo->getWidth(), mFbo->getHeight()) );
//            
//            //	Draw
//            ci::gl::drawSolidRect(mFbo->getBounds());
//            
//            //	Restore everything
//            tex.unbind();
//            mMask->getTexture()->unbind();
//            mMaskShader->unbind();
//        } else {
//            //	Just draw the fbo
//            ci::gl::draw(tex, mFbo->getBounds());
//        }
//    }
	
	
    //------------------------------------
    //	Masking
	//------------------------------------
	
	//
	//	Apply the mask (as a shaperef)
	//
    void Node::setMask(ShapeRef mask)
    {
        //	If successful, try to build the shader
        if (!mMaskShader) {
            try {
                mMaskShader = ci::gl::GlslProg::create( maskVertShader, maskFragShader);
            } catch (ci::gl::GlslProgCompileExc e) {
                ci::app::console() << "Could not load shader: " << e.what() << std::endl;
                return;
            }
        }
        
        //	Set our vars
        mMask       = mask;
        mIsMasked   = true;
    }
	
	//
    //	Remove the mask, and stop caching to FBO unless requested
	//
    NodeRef Node::removeMask()
    {
        mIsMasked = false;
        
        NodeRef mask = mMask;
        mMask.reset();
        return mask;
    }

    
    //------------------------------------
    //	Attributes
	//------------------------------------
	
	//
	//	Set the position
	//
    Node &Node::setPosition(float x, float y)
    {
        mPositionAnim.stop();
        mUpdatePositionFromAnim = false;
        mPosition.set(x, y);
        mPositionAnim.ptr()->set(mPosition);
        mFrameDirty = true;
        return *this;
    }
	
	//
    //	Set the scale
	//
    Node &Node::setScale(float x, float y)
    {
        mScaleAnim.stop();
        mUpdateScaleFromAnim = false;
        mScale.set(x, y);
        mScaleAnim.ptr()->set(mScale);
        mFrameDirty = true;
        mBoundsDirty = true;
        return *this;
    }
    
    //
    //	Set the rotation
	//
    Node &Node::setRotation(float rotation)
    {
        if(rotation >= 360.0f) {
            rotation = fmodf(rotation, 360.0f);
        }
        
        mRotationAnim.stop();
        mUpdateRotationFromAnim = false;
        mRotation = rotation;
        mRotationAnim = rotation;
        mFrameDirty = true;
        mBoundsDirty = true;
        return *this;
    }
    
    //
    //	Set the alpha
	//
    Node &Node::setAlpha(float alpha)
    {
        mAlphaAnim.stop();
        mUpdateAlphaFromAnim = false;
        mAlpha = ci::math<float>::clamp(alpha, 0.f, 1.f);
        mAlphaAnim = mAlpha;
        return *this;
    }
    
    //
    // Set the fill color
    Node &Node::setFillColor(ci::Color color)
    {
        mFillColor = color;
        mFillColorAnim.ptr()->set(mFillColor);
        return *this;
    }
    
    //
    //	Offset the whole node from the origin
	//
    Node &Node::setOffset(float x, float y) {
        mOffsetAnim.stop();
        mUpdateOffsetFromAnim = false;
        mOffset.set(x, y);
        mOffsetAnim.ptr()->set(mOffset);
        mFrameDirty = true;
        
		//	If we are manually setting the offset, we can't have alignment
        setAlignment(Alignment::NONE);
        
        return *this;
    }
    
    //
    //	Check if we are visible, and up the scene graph
	//	Somewhat slow, could be better implementation (i.e. parents set a var on their children like "parentIsVisible")
	//
    bool Node::isVisible()
    {
        if (!mVisible) return false;
        
        NodeRef parent = getParent();
        while (parent) {
            if (!parent->mVisible) return false;
            parent = parent->getParent();
        }
        
        return true;
    }
    
	
    //------------------------------------
    //  Animation
	//------------------------------------
    
    void Node::initAttrAnimations()
    {
        //	Initialize the isComplete() method of each tween
        mPositionAnim.stop();
        mScaleAnim.stop();
        mRotationAnim.stop();
        mOffsetAnim.stop();
        mAlphaAnim.stop();
        mFillColorAnim.stop();
    }
	
	//
	//	TODO: Find a better way to do this, probably some sort of map of active properties.
	//
    void Node::updateAttributeAnimations()
	{
        //	See if a tween is in progress, if so we want to use that value
        //	Setting an attribute calls stop(), so that will override this
        if (!mPositionAnim.isComplete()) mUpdatePositionFromAnim = true;
        if (!mScaleAnim.isComplete()) mUpdateScaleFromAnim = true;
        if (!mRotationAnim.isComplete()) mUpdateRotationFromAnim = true;
        if (!mAlphaAnim.isComplete()) mUpdateAlphaFromAnim = true;
        if (!mOffsetAnim.isComplete()) mUpdateOffsetFromAnim = true;
        if (!mFillColorAnim.isComplete()) mUpdateFillColorFromAnim = true;
        
        //	Update Anims if we care
        if (mUpdatePositionFromAnim) mPosition = mPositionAnim;
        if (mUpdateScaleFromAnim) mScale = mScaleAnim;
        if (mUpdateRotationFromAnim) mRotation = mRotationAnim;
        if (mUpdateAlphaFromAnim) mAlpha = mAlphaAnim;
        if (mUpdateOffsetFromAnim) mOffset = mOffsetAnim;
        if (mUpdateFillColorFromAnim) mFillColor = mFillColorAnim;
    }
    
	
    //------------------------------------
    //  Alignment
	//------------------------------------
    
    Node &Node::setAlignment(Alignment alignment)
    {
        mAlignment = alignment;
        
        if (mAlignment == Alignment::NONE) return *this;
        if (mAlignment == Alignment::TOP_LEFT)  {
            mOffset.set(0, 0);
            return *this;
        }
        
        ci::Rectf bounds = getBounds();
        
        switch (mAlignment) {
            case Alignment::NONE:
            case Alignment::TOP_LEFT:
                break;
            case Alignment::TOP_CENTER:
                mOffset.set(-bounds.getWidth() / 2.f, 0);
				break;
            case Alignment::TOP_RIGHT:
                mOffset.set(-bounds.getWidth(), 0);
				break;
            case Alignment::CENTER_LEFT:
                mOffset.set(0, -bounds.getHeight() / 2.f);
				break;
            case Alignment::CENTER_CENTER:
                mOffset.set(-bounds.getWidth() / 2.f, -bounds.getHeight() / 2.f);
				break;
            case Alignment::CENTER_RIGHT:
                mOffset.set(-bounds.getWidth(), -bounds.getHeight() / 2.f);
				break;
            case Alignment::BOTTOM_LEFT:
                mOffset.set(0, -bounds.getHeight());
				break;
            case Alignment::BOTTOM_CENTER:
                mOffset.set(-bounds.getWidth() / 2.f, -bounds.getHeight());
				break;
            case Alignment::BOTTOM_RIGHT:
                mOffset.set(-bounds.getWidth(), -bounds.getHeight());
				break;
        }
        
        mOffsetAnim.ptr()->set(mOffset);
        
        return *this;
    }
    
	
    //------------------------------------
    //  Transformation
	//------------------------------------
    
    void Node::setTransformation()
    {
        switch(mMatrixOrder) {
            case MatrixOrder::TRS:
                ci::gl::translate(mPosition);
                ci::gl::rotate(mRotation);
                ci::gl::scale(mScale);
                break;
            case MatrixOrder::RST:
                ci::gl::rotate(mRotation);
                ci::gl::scale(mScale);
                ci::gl::translate(mPosition);
                break;
        }
        
        ci::gl::translate(mOffset);
        mMatrix.set(ci::gl::getModelView(), ci::gl::getProjection(), ci::gl::getViewport());
    }
    

    ci::Vec2f Node::nodeToLocal(const ci::Vec2f &point, NodeRef node) {
		return windowToLocal(node->localToWindow(point));
	}
    
    ci::Vec2f Node::localToNode(const ci::Vec2f &point, NodeRef node) {
		return node->windowToLocal(localToWindow(point));
	}
	
    ci::Vec2f Node::sceneToLocal(const ci::Vec2f &scenePoint)
    {
        SceneRef scene = getScene();
        if (scene != nullptr) return scene->getRootNode()->localToNode(scenePoint, shared_from_this());
        return ci::Vec2f();
    }
    
    ci::Vec2f Node::localToScene(const ci::Vec2f &point)
    {
        SceneRef scene = getScene();
        if (scene != nullptr) return localToNode(point, scene->getRootNode());
		return ci::Vec2f();
    }
    
    ci::Vec2f Node::sceneToWindow(const ci::Vec2f &point)
    {
        SceneRef scene = getScene();
        if (scene != nullptr) return scene->getRootNode()->localToWindow(point);
        return point;
    }
    
    ci::Vec2f Node::windowToScene(const ci::Vec2f &point)
    {
        SceneRef scene = getScene();
        if (scene != nullptr) return scene->getRootNode()->windowToLocal(point);
        return ci::Vec2f();
    }
	
    ci::Vec2f Node::windowToLocal(const ci::Vec2f &windowPoint) {
		return mMatrix.globalToLocal(windowPoint);
	}
	
    ci::Vec2f Node::localToWindow(const ci::Vec2f &scenePoint)
    {
        if (mHasScene) return mMatrix.localToGlobal(scenePoint);
        return ci::Vec2f();
    }
    
    //
    //  This is used for hit-testing all Nodes
    //  Override this function to do any type of custom
    //
    bool Node::pointInside(const ci::Vec2f &point)
    {
        ci::Vec2f pos = windowToLocal(point);
        return getBounds().contains(pos);
    }

    
    //------------------------------------
    //  Parent + Scene
	//------------------------------------
    
    void Node::setScene(SceneRef sceneRef) {
        mScene = sceneRef;
        mHasScene = mScene.lock() ? true : false;
        if (hasScene()) mScene.lock()->trackChildNode(shared_from_this());
    }
    
    SceneRef Node::getScene() { return mScene.lock(); }
	
    bool Node::hasScene() { return mHasScene; }
	
    void Node::removeScene()
    {
        SceneRef scene = mScene.lock();
        if (scene) scene->untrackChildNode(shared_from_this());
        mScene.reset();
        mHasScene = false;
    }
	
    void Node::setParent(NodeContainerRef containerNode)
    {
        mParent = containerNode;
        mHasParent = mParent.lock() ? true : false;
    }
	
    NodeContainerRef Node::getParent() const {
		return mParent.lock();
	}
	
    bool Node::hasParent() { return mHasParent; }
	
    void Node::removeParent() {
        mParent.reset();
        mHasParent = false;
    }
	
    
    //------------------------------------
    //  Dimensions
	//------------------------------------
    
    ci::Rectf Node::getBounds()
    {
        //	Reset Bounds
        ci::Rectf bounds = ci::Rectf(0, 0, 0, 0);
        return bounds;
    }
	
    void Node::drawBounds()
    {
        ci::gl::color(mBoundsColor);
        
        //	Draw bounding box
        ci::gl::drawStrokedRect(getBounds());
        
        //	Draw origin
        ci::gl::pushModelView();
        ci::gl::translate(-mOffset);
        ci::gl::scale(ci::Vec2f(1.f, 1.f) / mScale);
        ci::gl::drawSolidRect(ci::Rectf(-ORIGIN_SIZE / 2, -ORIGIN_SIZE / 2, ORIGIN_SIZE, ORIGIN_SIZE));
        ci::gl::popModelView();
    }
	
    ci::Rectf Node::getFrame()
    {
//        if (bFrameDirty) {
            ci::Rectf r = getBounds();
            
            ci::MatrixAffine2f m;
            m.translate(mPosition);
            m.rotate(ci::toRadians(getRotation()));
            m.scale(mScale);
            m.translate(mOffset);
            
            mFrame = r.transformCopy(m);
            mFrameDirty = false;
//        }
        return mFrame;
    }
	
    
    //------------------------------------
    //  Events
	//------------------------------------
	
	//
    //  General
	//
    
    bool Node::isEligibleForInteractionEvents()
    {
        if ( !hasScene() || !isInteractionEnabled() || !isVisible() ) return false;
        return true;
    }
    
    void Node::disconnectAllSignals()
    {
        for (auto &signal : mMouseEventSignals) {
            signal.second.disconnect_all_slots();
        }
        
        for (auto &signal : mTouchEventSignals) {
            signal.second.disconnect_all_slots();
        }
    }
	
	
	//------------------------------------
    //  Mouse Events
	//------------------------------------
	
	//
    //	See if we care about an event
	//
    bool Node::isEligibleForInteractionEvent(const MouseEvent::Type &type)
    {
        if ((mMouseEventSignals.count(type) && mMouseEventSignals[type].num_slots() != 0)) {
            return isEligibleForInteractionEvents();
        }
        return false;
    }
    
    //
    //	For the given event, notify everyone that we have as a subscriber
	//
    void Node::emitEvent(MouseEvent &event)
    {
        event.setSource(shared_from_this());
        mMouseEventSignals[event.getType()](event);
    }
    
	
	//------------------------------------
    //  Touch Events
	//------------------------------------
	
	//
    //	For the given event, notify everyone that we have as a subscriber
	//
    void Node::emitEvent(TouchEvent &event)
    {
        event.setSource(shared_from_this());
        mTouchEventSignals[event.getType()](event);
    }
	
	//
    //	See if we care about an event
	//
    bool Node::isEligibleForInteractionEvent(const TouchEvent::Type &type)
    {
        if ((mTouchEventSignals.count(type) && mTouchEventSignals[type].num_slots() != 0)) {
            return isEligibleForInteractionEvents();
        }
        return false;
    }
	
} } //  namespace po::scene