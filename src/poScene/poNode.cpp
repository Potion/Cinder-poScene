//
//  poNode.cpp
//  BasicTest
//
//  Created by Stephen Varga on 3/17/14.
//
//

#include "cinder/CinderMath.h"

#include "poNode.h"
#include "poNodeContainer.h"
#include "poScene.h"

namespace po {
    static uint OBJECT_UID  = 0;
    static const int ORIGIN_SIZE   = 2;
    
    NodeRef Node::create()
    {
        return std::shared_ptr<Node>(new Node());
    }
    
    Node::Node()
    :   mUid(OBJECT_UID++)
    ,   mPosition(0.f,0.f)
    ,   mScale(1.f,1.f)
    ,   mRotation(0)
    ,   mOffset(0.f,0.f)
    ,   mAlpha(1.f)
    ,   mPositionAnim(ci::Vec2f(0.f,0.f))
    ,   mScaleAnim(ci::Vec2f(1.f,1.f))
    ,   mRotationAnim(0)
    ,   mOffsetAnim(ci::Vec2f(0.f,0.f))
    ,   mAlphaAnim(1.f)
    ,   mUpdatePositionFromAnim(false)
    ,   mUpdateScaleFromAnim(false)
    ,   mUpdateRotationFromAnim(false)
    ,   mUpdateOffsetFromAnim(false)
    ,   mUpdateAlphaFromAnim(false)
    ,   mDrawBounds(false)
    ,   mBoundsDirty(true)
    ,   mFrameDirty(true)
    ,   bVisible(true)
    ,   bInteractionEnabled(true)
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
    
    void Node::drawTree()
    {
        //If we're invisible, nothing to do here
        if(!bVisible) return;
        
        //Update our draw order
        mDrawOrder = mScene.lock()->getNextDrawOrder();
        
        //Push our Matrix
        ci::gl::pushMatrices();
        setTransformation();
        
        //Draw this item
        draw();
        
        //Draw bounds if necessary
        if(mDrawBounds)
            drawBounds();
        
        //Pop our Matrix
        ci::gl::popMatrices();
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
        
        mOffset = mOffset-bounds.getUpperLeft();
    }
    
    
    //------------------------------------------------------
    #pragma mark - Transformation -
    
    void Node::setTransformation()
    {
        #pragma message "Need to implement matrix order"
        ci::gl::translate(mPosition);
        ci::gl::rotate(mRotation);
        ci::gl::scale(mScale);
        
        ci::gl::translate(mOffset);
    
        mMatrix.set(ci::gl::getModelView(), ci::gl::getProjection(), ci::gl::getViewport());
    }
    
    #pragma message "This is def not right"
    ci::Vec2f Node::sceneToLocal(ci::Vec2f scenePoint)
    {
        po::SceneRef scene = mScene.lock();
        if(scene) {
            return scene->getCamera().worldToScreen(ci::Vec3f(scenePoint, 0.f),
                                                ci::app::getWindow()->getWidth(),
                                                ci::app::getWindow()->getHeight());
        }
        
        return ci::Vec2f();
    }
    
    ci::Vec2f Node::globalToLocal(ci::Vec2f globalPoint)
    {
        return mMatrix.globalToLocal(globalPoint);
    }
    
    bool Node::pointInside(const ci::Vec2f &point)
    {
        return false;
    }
    
    
    
    
    //------------------------------------------------------
    #pragma mark - Parent & Scene -
    
    void Node::setScene(SceneRef sceneRef) {
        mScene = sceneRef;
        if(hasScene()) mScene.lock()->trackChildNode(shared_from_this());
    }
    
    SceneRef Node::getScene()
    {
        return mScene.lock();
    }
    
    bool Node::hasScene()
    {
        return (mScene.lock() ? true : false);
    }
    
    void Node::removeScene()
    {
        if(hasScene()) mScene.lock()->untrackChildNode(shared_from_this());
        mScene.reset();
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
        ci::gl::color(255,0,0);
        
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
    
    
    #pragma mark Mouse Events
    
    //Signals
    void Node::trackConnection(MouseEvent::Type type, Node *listener, scoped_connection *connection) {
        mouseConnections[type][listener] = std::unique_ptr<scoped_connection>(connection);
    };
    
    void Node::disconnect(MouseEvent::Type type, Node *listener) {
        mouseConnections[type][listener]->disconnect();
    }
    
    
    #pragma mark -
    //Mouse Down Inside
    void Node::connectMouseDownInside(Node* listener)
    {
        scoped_connection *connection = new scoped_connection(mSignalMouseDownInside.connect(std::bind( &Node::mouseDownInside, listener, std::_1 )));
        trackConnection(po::MouseEvent::Type::DOWN_INSIDE, listener, connection);
    }
    
    void Node::disconnectMouseDownInside(Node* listener)
    {
        disconnect(po::MouseEvent::Type::DOWN_INSIDE, listener);
    };
    
    void Node::emitMouseDownInside(po::MouseEvent &event)
    {
        mSignalMouseDownInside(event);
    };
    
    
    #pragma mark -
    //Mouse Move Inside
    void Node::connectMouseMoveInside(Node* listener)
    {
        scoped_connection *connection = new scoped_connection(mSignalMouseMoveInside.connect(std::bind( &Node::mouseMoveInside, listener, std::_1 )));
        trackConnection(po::MouseEvent::Type::MOVE_INSIDE, listener, connection);
    }
    
    void Node::disconnectMouseMoveInside(Node* listener)
    {
        disconnect(po::MouseEvent::Type::MOVE_INSIDE, listener);
    };
    
    void Node::emitMouseMoveInside(po::MouseEvent &event)
    {
        mSignalMouseMoveInside(event);
    };
    
    
    #pragma mark -
    //Mouse Drag Inside
    void Node::connectMouseDragInside(Node* listener)
    {
        scoped_connection *connection = new scoped_connection(mSignalMouseDragInside.connect(std::bind( &Node::mouseDragInside, listener, std::_1 )));
        trackConnection(po::MouseEvent::Type::DRAG_INSIDE, listener, connection);
    }
    
    void Node::disconnectMouseDragInside(Node* listener)
    {
        disconnect(po::MouseEvent::Type::DRAG_INSIDE, listener);
    };
    
    void Node::emitMouseDragInside(po::MouseEvent &event)
    {
        mSignalMouseMoveInside(event);
    };
    
    
    #pragma mark -
    //Mouse Up Inside
    void Node::connectMouseUpInside(Node* listener)
    {
        scoped_connection *connection = new scoped_connection(mSignalMouseUpInside.connect(std::bind( &Node::mouseUpInside, listener, std::_1 )));
        trackConnection(po::MouseEvent::Type::UP_INSIDE, listener, connection);
    }
    
    void Node::disconnectMouseUpInside(Node* listener)
    {
        disconnect(po::MouseEvent::Type::UP_INSIDE, listener);
    };
    
    void Node::emitMouseUpInside(po::MouseEvent &event)
    {
        mSignalMouseUpInside(event);
    };

    
    #pragma mark -
    //Global Mouse Events
    void Node::notifyGlobal(po::MouseEvent &event) {
        //Setup event
        event.mSource    = shared_from_this();
        event.mPos       = globalToLocal(event.getWindowPos());
        event.mScenePos  = getScene()->getRootNode()->globalToLocal(event.getWindowPos());
        
        switch (event.getType()) {
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
        if(!mouseConnections[type].size()) {
           return false;
        }
        else {
            std::map<po::Node*, std::unique_ptr<scoped_connection> >::iterator iter = mouseConnections[type].begin();
            for(; iter!=mouseConnections[type].end(); ++iter) {
                if(iter->second.get()->connected()) return true;
            }
        }
        
        return false;
    }
    
    //For the given event, notify everyone that we have as a subscriber
    void Node::emitEvent(po::MouseEvent &event)
    {
        //Setup event
        event.mSource    = shared_from_this();
        event.mPos       = globalToLocal(event.getWindowPos());
        event.mScenePos  = getScene()->getRootNode()->globalToLocal(event.getWindowPos());
        
        //Emit the Event
        switch (event.getType()) {
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
    
    #pragma mark -
    //Global Mouse Events
    void Node::notifyGlobal(po::KeyEvent &event) {
        switch (event.getType()) {
            case po::KeyEvent::Type::DOWN:
                keyDown(event);
                break;
            case po::KeyEvent::Type::UP:
                keyUp(event);
                break;
        }
    }
}
