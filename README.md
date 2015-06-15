# po::scene

**po::scene** is [Potion's](http://www.potiondesign.com) 2D Scene graph block for working with layout, animation and interaction of multiple objects and views within [Cinder](http://libcinder.org). It is at it's most useful when used with applications that contain a large amount of interactive objects with various states and attributes. po::scene is released under the [BSD New License](./LICENSE).

The main philosophy behind po::scene is to avoid replacing any of the built-in Cinder functionality at all costs and keep the library as simple as possible.

Some of the key features include:

+ Draw tree with automatic update and draw functionality
+ Per-node attributes for position, rotation, scale, offset, color, alpha, etc.
+ Cinder timeline animation attributes that automatically update Node attributes
+ Per-node origin points with support for dynamic alignment
+ Point-transform from screen space to anywhere in the scene
+ Node or group-based Masking
+ Full integration with Cinder::Shape2d for custom interactive shapes and texture mapping
+ Node Containers for leveled grouping of Nodes, allowing for group manipulation and hit-testing
+ Signal based events with type, source reference and propagation properties
+ Simple wrappers for Cinder's built-in Shapes, Textures, TextBoxes, Videos
+ Rendering to FBO/Texture for post-processing or caching
+ Smart-pointer based syntax with TypeDefs for built-in classes
+ Simple inheritance for custom Nodes: just define update(),draw() and getBounds()
 
## Usage

Using a tree metaphor, **po::scene** contains three main classes:

+ `po::scene::Scene` is the root system of the tree. It contains the trunk of the tree (the root node), which in turn contains all branches and leaves.
+ `po::scene::NodeContainer` represents a branch. It can contain any number of other branches, as well as leaves.
+ `po::Scene::Node` represents a leaf, or an end point in the scene-graph. These nodes are usually the point for any gl drawing.

Moving any branch of a tree will also move all of it's connected branches and leaves. In po::scene moving, scaling, rotating or manipulating a `po::scene::NodeContainer` will transform all of the child nodes within it.



![Bounds example](images/Bounds.png)
*Screenshot from the bounds example, describing the structure of a scene* 
## Nodes

Nodes are the basic building blocks of a scene. The po::scene::Node class is a base class that can not be used on it's own. To create a Node, extend this class and implement the following methods:

+ `update()` Called every frame, do any general code updates here before drawing.
+ `draw()` This draws the Node. Override this method and do any OpenGL drawing here. When this method is called all transforms have been applied to the node (i.e. position, scale, rotation), so everything is relative to the Node's 0,0 origin
+ `getBounds()` This is necessary for any hit-testing of an object. Return a ci::Rectf with the Top Left and Bottom Right of the content drawn (or a wider area if you'd like to expand the hit area). For example, the po::scene::Image class returns the top left as 0,0 (relative to the node's origin) and the width and height of the associated `ci::gl::TextureRef`.

		ci::Rectf Image::getBounds()
		{
			return ci::Rectf(0,0,mTexture->getWidth(),mTexture->getHeight());
		}
		
		IMAGE EXPLAINING BOUNDS HERE?
		
###Attributes
Nodes have a number of built in attributes that relate to how they appear in the scene:

+ `position` The position within the Node's parent (containing) node
+ `rotation` The rotation of the node along the z-axis (2d rotation). Expressed in degrees.
+ `scale` The x and y scale of the node
+ `fillColor` The color that is used for drawing the node. This color is used differently depending on the type of drawing the node does. Expressed as a ci::Color
+ `alpha` The transparency of the node. This is applied through the node tree, i.e. if a parent node has an alpha of 0.5 and the child has an alpha of 1.0, the child will draw at 0.5 alpha. Items with an alpha of 0.0 will not render but they will be considered for events.
+ `offset` The offset (relative to the node's 0,0 origin) that the node's content will be drawn at
+ `alignment` Built-in enums used to set the node's offset automatically, relative to it's bounds. I.E. CENTER_CENTER will set the node's offset to `getBounds().getSize() * - 0.5`

These attributes use setters and getters that allow the node to internally keep track of things.
	
	using namespace po::scene;
	SomeNodeClassRef node = SomeNodeClass::create();
	node->setPosition(ci::Vec2f(25.0f, 25.0f)
	node->setFillColor(ci::Color(1.0f, 0.0f, 0.0f);
	node->setAlpha(0.5f);
	node->setRotation(45.0f);
	node->setAlignment(Alignment::CENTER_CENTER);
	
###Chaining
All node attributes should return by reference, allowing for easy chaining when setting several parameters at once. Here is a more convenient way to write the above example:
	
	using namespace po::scene;
	SomeNodeClassRef node = SomeNodeClass::create();
	node->setPosition(ci::Vec2f(25.0f, 25.0f)
	.setFillColor(ci::Color(1.0f, 0.0f, 0.0f)
	.setAlpha(0.5f)
	.setRotation(45.0f)
	.setAlignment(Alignment::CENTER_CENTER);

	

###Animations
All node attributes have corresponding ci::Anim objects. These animations can be set and they will be automatically applied to the node's internal attributes. 

	// Animate a node from 0,0 to 50,50
	ci::app::timeline().apply(&node->getPositionAnim(), ci::Vec2f(50.0f, 50.0f), 1.0f);
	
Setting a node attribute at any time during an animation will cancel that animation.

###Transformations
Every `po::scene::node` has it's own coordinate space. All `po::scene::Node` members contain functions for translating back and forth between various coordinate spaces. The three main spaces are:

+ `window` The origin of the application's window, as used by Cinder for all screen-based events.
+ `scene` The origin and transformation of a node's Scene (as specified by the Scene's Root Node)
+ `local` The origin and transformation of the node.

There are several node functions that can be used to translate between coordinate spaces:

+ `windowToLocal`/`localToWindow` Translate from/to window space
+ `sceneToLocal`/`localToScene` Translate from/to scene space
+ `nodeToLocal`/`localToNode` Translate from one node's space into another node's space. Equivalent to `node1->windowToLocal(node2->localToWindow(...))`

###Events
All nodes can respond to mouse and touch interaction events. These events come through as `po::scene::MouseEvent` and `po::scene::TouchEvent` class members. 

These events inherit from `po::scene::Event`, which in turn wraps Cinder's built-in `ci::app::Event` events while adding po::scene specific information.

All `po::scene` events have the following information:

+ `ciEvent` A copy of the raw Cinder event that this event wraps
+ `source` The node that is subscribing to the event
+ `propagationEnabled` Controls whether or not this even should continue through the draw tree if it is handled by the source node
+ `windowPos` The position of the event in the window space
+ `scenePos` The position of the event in the source node's scene
+ `localPos` The position of the event within the source node

#####Cinder Events
`po::scene` wraps the following built-in global (window) Cinder events for convenience and consistency:

+ Mouse Down, Mouse Move, Mouse Drag, Mouse Up
+ Touch Began, Touch Moved, Touch Ended. *Note: Cinder responds with a group of TouchEvents, po::scene responds with individual TouchEvents*

These events will fire on all subscribing node's, in order of the draw tree (bottom-up), regardless of the node's bounds.

#####Node Events
`po::scene` adds additional events that add interativity to all nodes. These events are fired on the top-most node first, and will only pass through nodes if the `propagationEnabled` variable is set to `true`.

+ Mouse Down Inside, Mouse Move Inside, Mouse Drag Inside, Mouse Up Inside
+ Touch Began Inside, Touch Moved Inside, Touch Ended Inside

####Subscribing to events
`po::scene::node` events are signal-based, and can be retrieved using the `getSignal(SignalType)` syntax.
	
	using namespace po::scene;
	SomeNodeClassRef node = SomeNodeClass::create();
	// Connect node's Mouse Down Inside event to a member function of this class
	node->getSignal(MouseEvent::Type::DOWN_INSIDE).connect(std::bind(&ThisClass::mouseHandler, this, std::placeholders::_1);
	
	
###Visibility/Interaction
`po::scene::Node` contains two variables that affect interactivity. The `setVisible(bool enabled)` method allows the node to stay in the node hierarchy, but it will not render and it will not be considered in interaction events. It is a quick way to turn a node on/off without having to add and remove it. 

`setInteractionEnabled(bool enabled)` sets a node to be ignored for events, but still render to the screen.

To set a node to be considered for interactive events but not render to the screen (i.e. an invisible hit area), set the node's `alpha` to 0.

## Node Containers

Node Containers inherit from the Node class and can contain any number of Nodes or other Node Containers. These are the branches of the scene, and they allow for hierarchy, grouping, complex layouts. 

Adjusting any attributes of a node container also effects every node that it contains.

A node container's bounds are determined by it's child nodes.

Node container events are based on any of the container's child nodes being eligible for an event. 


## Scene

The `po::scene::Scene` class represents the main entry point for a scene-graph. Any program can contain multiple Scene graphs, which will exist entirely independent of each other.

The Scene class requires a Root Node, which inherits from `po::scene::NodeContainer`. This node represents the beginning of the draw tree, and is the origin for the scene.

To create a scene, you can either start with an empty (auto-created) root node

	using namespace po::scene;
	SceneRef myScene = Scene::create();
	
You could then add child nodes to this root node using the `getRootNode()` function of `po::scene::NodeContainer`.
	
	NodeRef myRootNode = MyRootNodeClass::create();
	myScene->getRootNode()->addChild(myRootNode);
	
 or (more common) extend `po::scene::NodeContainer` and provide that as the root node.
 
	using namespace po::scene;
	SceneRef myScene = Scene::create(MyRootNodeClass::create());

The Scene's `update()` and `draw()` functions need to be called each frame from Cinder's `update()` and `draw()` functions.

	void MyCinderApp::update()
	{
		myScene->update();
	}
	
	void MyCinderApp::draw()
	{
		myScene->draw();
	}
	

This in turn will traverse the Node Tree, automatically triggering the `update()` and `draw()` trees of every visible node in the scene.


## Cinder Nodes

po::scene provides a number of convenient wrappers around Cinder classes that allow users to attach various renderable objects rather than trying to extend these or reimpliment these classes. These allow renderable Cinder objects to gain hit-testing, matrix transforms, animations, and everything else provided by the `po::scene::Node` class.

### po::scene::Image

`po::scene::Image` provides a simple Node that displays a `ci::gl::TextureRef`.

### po::scene::Shape

`po::scene::Shape` is a more complex wrapper for both the `ci::Shape2d` class as well as the `ci::gl::TextureRef` class. 

The first way to use a `po::scene::Shape` is as a solid-filled shape. `po::scene::Shape` provides number of convenience methods for this purpose:

	using namespace po::scene;
	ShapeRef myRect = Shape::createRect(100,200);
	ShapeRef mySquare = Shape::createSquare(100);
	ShapeRef myEllipse = Shape::createEllipse(100,200);
	ShapeRef myCircle = Shape::createCircle(100);
	
**EXAMPLE PICTURE OF THIS**

These create and set the backing `ci::Shape2d`. This can be replaced at any time by the user.

This shape will be drawn using the `po::scene::Node` fillColor attribute, and hit-testing will be done using the built in `ci::Shape2d` contains method.

In addition, a `ci::gl::TextureRef` can be attached to it to any `po::scene::Shape` and mapped using a number of alignments.

**EXAMPLE OF TEXTURES AND ALIGNMENTS**

### po::scene::Video

### po::scene::TextBox

## History

[Potion](http://www.potiondesign.com) is an interactive design and development studio based in New York City. [Potion](http://www.potiondesign.com) was founded in 2005 with the idea of bringing interaction beyond the desktop and into the real world. At the time the tools for doing this type of work were few and far between, and most of our work involved writing many things from scratch. Examples include Vision Systems, custom touchscreens, and numerous OpenGL implementations.

As Potion began to grow, our OpenGL work slowly started to evolve into our internal framework [poCode](http://www.pocode.org/). We executed many projects and improved [poCode](http://www.pocode.org/) over the course of nearly 6 years, but supporting the basic infrastructure for a growing number of platforms, library updates and devices soon became a larger task than we could handle. 

Around 2010 a number of open-source C++ frameworks had began to emerge that covered a lot of the basic functionality (and beyond) of [poCode](http://www.pocode.org/), and they also began to ammas a large base of users. At the same time, the core feature of [poCode](http://www.pocode.org/) that we used for every project was the Scene Graph, and there did not seem to be a clear implementation of this in any other frameworks.

[Potion](http://www.potiondesign.com) made the decision in 2014 to move to [Cinder](http://libcinder.org/), and at the same time rewrite our Scene Graph to work on top of all of the great work that the [Cinder](http://libcinder.org/) team had done and continued to do. Since then we have planned, written, tested, re-planned, and rewritten our scene graph while using it to complete 2 large-scale interactive installations.

We now feel ready to release po::scene, a 2D scene graph built on top of [Cinder](http://libcinder.org/). It is a tool that we find extremely useful, and we're hoping others do as well. It is not right for every project, but we are hopeful that there are enough people out there doing similar work that it will help save a few people from rewriting scene graphs from scratch for each project. We would love it if others gave it a try and let us know what they think. We're happy to answer any questions (within reason) and respond to pull request, address bugs, etc. that anyone might find.

At the same time, regardless of the amount of people who decide to use it, this is the library that we will use for the foreseeable future for the majority of our installations, so it will be regularly tested, updated, fixed and improved for our own benefit.

We are happy to have access to the wonderful resource that is Cinder, so hopefully we can pay it forward by open sourcing this library.

## What is a Scene Graph

[Wikipedia](http://en.wikipedia.org/wiki/Scene_graph) is probably the best place for a definition that is far more succinct than we can offer.

> A scene graph is a collection of nodes in a graph or tree structure. A tree node (in the overall tree structure of the scene graph) may have many children but often only a single parent, with the effect of a parent applied to all its child nodes; an operation performed on a group automatically propagates its effect to all of its members. In many programs, associating a geometrical transformation matrix (see also transformation and matrix) at each group level and concatenating such matrices together is an efficient and natural way to process such operations. A common feature, for instance, is the ability to group related shapes/objects into a compound object that can then be moved, transformed, selected, etc. as easily as a single object.