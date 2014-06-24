//
//  poNode.cpp
//  BasicTest
//
//  Created by Stephen Varga on 3/17/14.
//
//

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

namespace po {
    static uint32_t OBJECT_UID  = 0;
    static const int ORIGIN_SIZE   = 2;
    
    
    NodeRef Node::create(std::string name)
    {
        return std::shared_ptr<Node>(new Node(name));
    }
    
    Node::Node(std::string name)
    :   mUid(OBJECT_UID++)
    ,   mName(name)
    ,   mDrawOrder(0)
    ,   mPosition(0.f,0.f)
    ,   mScale(1.f,1.f)
    ,   mRotation(0)
    ,   mOffset(0.f,0.f)
    ,   mAlpha(1.f)
    ,   mAppliedAlpha(1.f)
    ,   mPositionAnim(ci::Vec2f(0.f,0.f))
    ,   mScaleAnim(ci::Vec2f(1.f,1.f))
    ,   mRotationAnim(0)
    ,   mOffsetAnim(ci::Vec2f(0.f,0.f))
    ,   mAlphaAnim(1.f)
    ,   mAlignment(Alignment::TOP_LEFT)
    ,   mMatrixOrder(MatrixOrder::TRS)
    ,   mFillColor(255,255,255)
    ,   mFillEnabled(true)
    ,   mStrokeColor(255,255,255)
    ,   mStrokeEnabled(false)
    ,   mUpdatePositionFromAnim(false)
    ,   mUpdateScaleFromAnim(false)
    ,   mUpdateRotationFromAnim(false)
    ,   mUpdateOffsetFromAnim(false)
    ,   mUpdateAlphaFromAnim(false)
    ,   mDrawBounds(false)
    ,   mBoundsColor(255,0,0)
    ,   mBoundsDirty(true)
    ,   mFrameDirty(true)
    ,   mVisible(true)
    ,   mInteractionEnabled(true)
    ,   mCacheToFbo(false)
    ,   mIsDrawingIntoFbo(false)
    ,   mIsMasked(false)
    ,   mHasScene(false)
    {
        //Initialize our animations
        initAttrAnimations();
    }
    
    Node::~Node() {
        
    }
    
    
    
    //------------------------------------------------------
    #pragma mark - Update & Draw Trees -
    
    void Node::updateTree()
    {
        //Update our tween tie-in animations
        updateAttributeAnimations();
        
        //Call our update function
        update();
    }
    
    void Node::beginDrawTree()
    {
        //Update our draw order
        if(hasScene()) mDrawOrder = mScene.lock()->getNextDrawOrder();
        
        //Set applied alpha
        if(hasParent())
            mAppliedAlpha = getParent()->getAppliedAlpha() * mAlpha;
        else
            mAppliedAlpha = mAlpha;
        
        //Push our Matrix
        ci::gl::pushMatrices();
        setTransformation();
    }
    
    void Node::drawTree()
    {
        //If we're invisible, nothing to do here
        if(!mVisible) return;
        
//        if (mCacheToFbo && !mIsDrawingIntoFbo) {
//            cacheToFbo();
//        }
        
        beginDrawTree();
        
        //Draw this item
        if(!mCacheToFbo || mIsDrawingIntoFbo) {
            draw();
        } else {
            drawFbo();
        }
        
        finishDrawTree();
    }
    
    void Node::finishDrawTree()
    {
        //Draw bounds if necessary
        if(mDrawBounds)
            drawBounds();
        
        //Pop our Matrix
        ci::gl::popMatrices();
    }
    
    
    
    
    //------------------------------------------------------
    #pragma mark  - Caching -
    
    bool Node::cacheToFbo()
    {
        //Save the window buffer
        ci::gl::SaveFramebufferBinding binding;
        
        //Create the FBO, set viewport and bind
        if(!mFbo) {
            try {
                GLint maxTextureSize;
                glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
                //            mFbo = ci::gl::Fbo(maxTextureSize/4, maxTextureSize/4);
                
                mFbo = ci::gl::Fbo(getWidth(), getHeight());
            } catch (ci::gl::FboException) {
                ci::app::console() << "po::Scene: Couldn't create FBO, make sure your node has children or content!" << std::endl;
                return false;
            }
        }
        
        ci::gl::setViewport(mFbo.getBounds());
        mFbo.bindFramebuffer();
        
        //Set Ortho camera to fbo bounds
        ci::CameraOrtho cam;
        cam.setOrtho( 0,getBounds().getWidth(), getBounds().getHeight(), 0, -1, 1 );
        ci::gl::setMatrices(cam);
        
        //Draw into the FBO
        ci::gl::pushMatrices();
        ci::gl::translate(-getFrame().getUpperLeft());
        mIsDrawingIntoFbo = true;
        drawTree();
        mIsDrawingIntoFbo = false;
        ci::gl::popMatrices();
        
        //Set the camera up for the window
        cam.setOrtho(0, ci::app::getWindowWidth(), ci::app::getWindowHeight(), 0, -1, 1);
        ci::gl::setMatrices(cam);
        
        //Return the viewport
        ci::gl::setViewport(ci::app::getWindowBounds());
        
        mCacheToFbo = true;
        
        return true;
    }
    
    void Node::drawFbo()
    {
        ci::gl::enableAlphaBlending();
        ci::gl::color(255,255,255);
        
        ci::gl::Texture tex = mFbo.getTexture();
        tex.setFlipped(true);
        
        if(mIsMasked) {
            tex.bind(0);
            mMask->getTexture()->bind(1);
            
            mMaskShader.bind();
            mMaskShader.uniform("tex", 0);
            mMaskShader.uniform("mask", 1);
            mMaskShader.uniform ( "contentScale", ci::Vec2f((float)tex.getWidth() / (float)mMask->getWidth(), (float)tex.getHeight() / (float)mMask->getHeight() ) );
            mMaskShader.uniform ( "maskPosition", mMask->getPosition()/ci::Vec2f(mFbo.getWidth(), mFbo.getHeight()) );

            ci::gl::drawSolidRect(mFbo.getBounds());
            
            tex.unbind();
            mMask->getTexture()->unbind();
            mMaskShader.unbind();
        }
        
        else {
            ci::gl::draw(tex, getBounds());
        }
    }
    
    
    //------------------------------------------------------
    #pragma mark  - Masking -
    
    void Node::setMask(po::ShapeRef mask)
    {
        //Try to cache to FBO
        if(cacheToFbo()) {
            //If successful, try to build the shader
            if(!mMaskShader)
            {
                try {
                    mMaskShader = ci::gl::GlslProg ( ci::app::loadResource(RES_GLSL_PO_MASK_VERT), ci::app::loadResource( RES_GLSL_PO_MASK_FRAG));
                } catch (ci::gl::GlslProgCompileExc e) {
                    ci::app::console() << "Could not load shader: " << e.what() << std::endl;
                    return;
                }
            }
            
            //Set our vars
            mMask       = mask;
            mIsMasked   = true;
            mCacheToFbo = true;
        }
    }
    
    po::ShapeRef Node::removeMask(bool andStopCaching)
    {
        mIsMasked = false;
        mCacheToFbo = !andStopCaching;
        po::ShapeRef mask = mMask;
        mMask.reset();
        return mask;
    }
    
    
    
    
    //------------------------------------------------------
    #pragma mark - Attributes -
    
    void Node::setPosition(float x, float y)
    {
        mPositionAnim.stop();
        mUpdatePositionFromAnim = false;
        mPosition.set(x, y);
        mPositionAnim.ptr()->set(mPosition);
        mFrameDirty = true;
    }
    
    void Node::setScale(float x, float y)
    {
        mScaleAnim.stop();
        mUpdateScaleFromAnim = false;
        mScale.set(x, y);
        mScaleAnim.ptr()->set(mScale);
        
        mFrameDirty     = true;
        mBoundsDirty    = true;
    }
    
    void Node::setRotation(float rotation)
    {
        mRotationAnim.stop();
        mUpdateRotationFromAnim = false;
        mRotation = rotation;
        mRotationAnim = rotation;
        
        mFrameDirty     = true;
        mBoundsDirty    = true;
    }
    
    void Node::setAlpha(float alpha)
    {
        mAlphaAnim.stop();
        mUpdateAlphaFromAnim = false;
        mAlpha = ci::math<float>::clamp(alpha, 0.f, 1.f);
        mAlphaAnim = mAlpha;
    }
    
    void Node::setOffset(float x, float y) {
        mOffsetAnim.stop();
        mUpdateOffsetFromAnim = false;
        mOffset.set(x, y);
        mOffsetAnim.ptr()->set(mOffset);
        mFrameDirty = true;
        
        //If we are manually setting the offset, we can't have alignment
        setAlignment(Alignment::NONE);
    }
    
    bool Node::isVisible()
    {
        if(!mVisible) return false;
        
        NodeRef parent = getParent();
        while(parent) {
            if(!parent->mVisible)
                return false;
        
            parent = parent->getParent();
        }
        
        return true;
    }
    
    void Node::initAttrAnimations()
    {
        //Initialize the isComplete() method of each tween, a bit annoying
        mPositionAnim.stop();
        mScaleAnim.stop();
        mRotationAnim.stop();
        mOffsetAnim.stop();
        mAlphaAnim.stop();
    }
    
    void Node::updateAttributeAnimations()
    {
        //See if a tween is in progress, if so we want to use that value
        //Setting an attribute calls stop(), so that will override this
        if(!mPositionAnim.isComplete())  mUpdatePositionFromAnim = true;
        if(!mScaleAnim.isComplete())     mUpdateScaleFromAnim    = true;
        if(!mRotationAnim.isComplete())  mUpdateRotationFromAnim = true;
        if(!mAlphaAnim.isComplete())     mUpdateAlphaFromAnim    = true;
        if(!mOffsetAnim.isComplete())    mUpdateOffsetFromAnim   = true;
        
        //Update Anims if we care
        if(mUpdatePositionFromAnim)     mPosition    = mPositionAnim;
        if(mUpdateScaleFromAnim)        mScale       = mScaleAnim;
        if(mUpdateRotationFromAnim)     mRotation    = mRotationAnim;
        if(mUpdateAlphaFromAnim)        mAlpha       = mAlphaAnim;
        if(mUpdateOffsetFromAnim)       mOffset      = mOffsetAnim;
    }
    
    //------------------------------------------------------
    #pragma mark  - Alignment -
    
    void Node::setAlignment(Alignment alignment)
    {
        mAlignment = alignment;
        
        if(alignment == Alignment::NONE) return;
        
        ci::Rectf bounds = getBounds();
        
        switch (mAlignment) {
            case Alignment::TOP_LEFT:
                mOffset.set(0,0); break;
            case Alignment::TOP_CENTER:
                mOffset.set(-bounds.getWidth()/2.f,0); break;
            case Alignment::TOP_RIGHT:
                mOffset.set(-bounds.getWidth(),0); break;
            case Alignment::CENTER_LEFT:
                mOffset.set(0,-bounds.getHeight()/2.f); break;
            case Alignment::CENTER_CENTER:
                mOffset.set(-bounds.getWidth()/2.f,-bounds.getHeight()/2.f); break;
            case Alignment::CENTER_RIGHT:
                mOffset.set(-bounds.getWidth(),-bounds.getHeight()/2.f); break;
            case Alignment::BOTTOM_LEFT:
                mOffset.set(0,-bounds.getHeight()); break;
            case Alignment::BOTTOM_CENTER:
                mOffset.set(-bounds.getWidth()/2.f,-bounds.getHeight()); break;
            case Alignment::BOTTOM_RIGHT:
                mOffset.set(-bounds.getWidth(),-bounds.getHeight()); break;
        }
        
        //mOffset = mOffset-bounds.getUpperLeft();
    }
    
    
    //------------------------------------------------------
    #pragma mark - Transformation -
    
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
    
    #pragma message "Do we need this?"
    ci::Vec2f Node::sceneToLocal(const ci::Vec2f &scenePoint)
    {
        return ci::Vec2f();
    }
    
    ci::Vec2f Node::globalToLocal(const ci::Vec2f &globalPoint)
    {
        return mMatrix.globalToLocal(globalPoint);
    }
    
    ci::Vec2f Node::localToGlobal(const ci::Vec2f &scenePoint)
    {
        po::SceneRef scene = mScene.lock();
        if(scene) {
            return mMatrix.localToGlobal(scenePoint);
        }
        
        return ci::Vec2f();
    }
    
    bool Node::pointInside(const ci::Vec2f &point, bool localize)
    {
        ci::Vec2f pos = localize ? globalToLocal(point) : point;
        return getBounds().contains(pos);
    }
    
    
    
    //------------------------------------------------------
    #pragma mark - Parent & Scene -
    
    void Node::setScene(SceneRef sceneRef) {
        mScene = sceneRef;
        mHasScene = mScene.lock() ? true : false;
        
        if(hasScene()) mScene.lock()->trackChildNode(shared_from_this());
    }
    
    SceneRef Node::getScene()
    {
        return mScene.lock();
    }
    
    bool Node::hasScene()
    {
        return mHasScene;
    }
    
    void Node::removeScene()
    {
        if(hasScene()) mScene.lock()->untrackChildNode(shared_from_this());
        mScene.reset();
        mHasScene = false;
    }
    
    void Node::setParent(NodeContainerRef containerNode)
    {
        mParent = containerNode;
    }
    
    NodeContainerRef Node::getParent() const {
        return mParent.lock();
    }
        
    bool Node::hasParent()
    {
        return (mParent.lock() ? true : false);
    }
    
    void Node::removeParent() {
        mParent.reset();
    }
    
    
    
    //------------------------------------------------------
    #pragma mark  - Dimensions -
    
    ci::Rectf Node::getBounds()
    {
        //Reset Bounds
        ci::Rectf bounds = ci::Rectf(0,0,0,0);
        return bounds;
    }
    
    void Node::drawBounds()
    {
        ci::gl::color(mBoundsColor);
        
        //Draw bounding box
        ci::gl::drawStrokedRect(getBounds());
        
        //Draw origin
        ci::gl::pushMatrices();
        ci::gl::translate(-mOffset);
        ci::gl::scale(ci::Vec2f(1.f,1.f)/mScale);
        ci::gl::drawSolidRect(ci::Rectf(-ORIGIN_SIZE/2, -ORIGIN_SIZE/2, ORIGIN_SIZE, ORIGIN_SIZE));
        ci::gl::popMatrices();
    }
    
    ci::Rectf Node::getFrame()
    {
//        if(bFrameDirty) {
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
    
    
    //------------------------------------------------------
    #pragma mark  - Events -
    
    #pragma mark General
    
    
    #pragma mark - Mouse Events
    
    #pragma mark -
    //Global Mouse Events
    void Node::notifyGlobal(po::MouseEvent &event, const po::MouseEvent::Type &type) {
        //Setup event
        event.mSource    = shared_from_this();
        event.mPos       = globalToLocal(event.getWindowPos());
        event.mScenePos  = getScene()->getRootNode()->globalToLocal(event.getWindowPos());
        
        switch (type) {
            case po::MouseEvent::Type::DOWN:
                mouseDown(event); break;
            case po::MouseEvent::Type::MOVE:
                mouseMove(event); break;
            case po::MouseEvent::Type::DRAG:
                mouseDrag(event); break;
            case po::MouseEvent::Type::UP:
                mouseUp(event); break;
            case po::MouseEvent::Type::WHEEL:
                mouseWheel(event); break;
        }
    }
    
    #pragma mark -
    //SceneGraph Specific events
    
    //See if we care about an event
    bool Node::hasConnection(po::MouseEvent::Type type)
    {
        switch (type) {
            case po::MouseEvent::Type::DOWN_INSIDE:
                return mSignalMouseDownInside.num_slots();
            case po::MouseEvent::Type::MOVE_INSIDE:
                return mSignalMouseMoveInside.num_slots();
            case po::MouseEvent::Type::DRAG_INSIDE:
                return mSignalMouseDragInside.num_slots();
            case po::MouseEvent::Type::UP_INSIDE:
                return mSignalMouseUpInside.num_slots();
        }
        
        return false;
    }
    
    //For the given event, notify everyone that we have as a subscriber
    void Node::emitEvent(po::MouseEvent &event, const po::MouseEvent::Type &type)
    {
        //Setup event
        event.mSource    = shared_from_this();
        event.mPos       = globalToLocal(event.getWindowPos());
        event.mScenePos  = getScene()->getRootNode()->globalToLocal(event.getWindowPos());
        
        //Emit the Event
        switch (type) {
            case po::MouseEvent::Type::DOWN_INSIDE:
                mSignalMouseDownInside(event); break;
            case po::MouseEvent::Type::MOVE_INSIDE:
                mSignalMouseMoveInside(event); break;
            case po::MouseEvent::Type::DRAG_INSIDE:
                mSignalMouseDragInside(event); break;
            case po::MouseEvent::Type::UP_INSIDE:
                mSignalMouseUpInside(event); break;
        }
    }
    
    #pragma mark - Touch Events -
    //Global Mouse Events
    void Node::notifyGlobal(po::TouchEvent &event, const po::TouchEvent::Type &type) {
        for(po::TouchEvent::Touch &touch : event.getTouches()) {
            //Setup event
            touch.mSource    = shared_from_this();
            touch.mPos       = globalToLocal(touch.getWindowPos());
            touch.mScenePos  = getScene()->getRootNode()->globalToLocal(touch.getWindowPos());
        }
        
        switch (type) {
            case po::TouchEvent::Type::BEGAN:
                touchesBegan(event); break;
            case po::TouchEvent::Type::MOVED:
                touchesMoved(event); break;
            case po::TouchEvent::Type::ENDED:
                touchesEnded(event); break;
        }
    }
    
    //For the given event, notify everyone that we have as a subscriber
    void Node::emitEvent(po::TouchEvent &event, const po::TouchEvent::Type &type)
    {
        //Setup event
        event.mSource    = shared_from_this();
        for(po::TouchEvent::Touch &touch :event.getTouches()) {
            touch.mPos       = globalToLocal(touch.getWindowPos());
            touch.mScenePos  = getScene()->getRootNode()->globalToLocal(touch.getWindowPos());
			touch.mSource	 = shared_from_this();
        }
        
        //Emit the Event
        switch (type) {
            case po::TouchEvent::Type::BEGAN_INSIDE:
                mSignalTouchesBeganInside(event); break;
            case po::TouchEvent::Type::MOVED_INSIDE:
                mSignalTouchesMovedInside(event); break;
            case po::TouchEvent::Type::ENDED_INSIDE:
                mSignalTouchesEndedInside(event); break;
            default:
                std::cout << "Touch event type " << type << " not found." << std::endl;
        }
    }
    
    //See if we care about an event
    bool Node::hasConnection(po::TouchEvent::Type type)
    {
        switch (type) {
            case po::TouchEvent::Type::BEGAN_INSIDE:
                return mSignalTouchesBeganInside.num_slots();
            case po::TouchEvent::Type::MOVED_INSIDE:
                return mSignalTouchesMovedInside.num_slots();
            case po::TouchEvent::Type::ENDED_INSIDE:
                return mSignalTouchesEndedInside.num_slots();
        }
        
        return false;
    }
    
    #pragma mark - Key Events -
    //Global Mouse Events
    void Node::notifyGlobal(po::KeyEvent &event, const po::KeyEvent::Type &type) {
        switch (type) {
            case po::KeyEvent::Type::DOWN:
                keyDown(event);
                break;
            case po::KeyEvent::Type::UP:
                keyUp(event);
                break;
        }
    }
}
