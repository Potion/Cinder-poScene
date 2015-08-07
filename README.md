!["po::Scene"](images/logo.jpg)


**po::scene** is [Potion's](http://www.potiondesign.com) 2D [Scene Graph](#scenegraph) block for working with layout, animation and interaction of multiple objects and views within [Cinder](http://libcinder.org). It is at its most useful when used with applications that contain a large amount of interactive objects with various states and attributes. po::scene is released under the [BSD New License](./LICENSE).

The main philosophy behind po::scene is to avoid replacing any of the built-in Cinder functionality at all costs and keep the library as simple as possible.

Some of the key features include:

+ Draw tree with automatic update and draw functionality
+ Per-node attributes for position, rotation, scale, offset, color, alpha, etc.
+ Cinder timeline animation attributes that automatically update node attributes
+ Per-node origin points with support for dynamic alignment
+ Point-transform from screen space to anywhere in the scene
+ Node or group-based Masking
+ Full integration with Cinder::Shape2d for custom interactive shapes and texture mapping
+ Node containers for leveled grouping of nodes, allowing for group manipulation and hit-testing
+ Signal based events with type, source reference and propagation properties
+ Simple wrappers for Cinder's built-in Shapes, Textures, TextBoxes, Videos
+ Rendering to FBO/Texture for post-processing or caching
+ Smart-pointer based syntax with TypeDefs for built-in classes
+ Simple inheritance for custom nodes: just define update(),draw() and getBounds()

<a name="scenegraph"></a>
## What is a Scene Graph

[Wikipedia](http://en.wikipedia.org/wiki/Scene_graph) is probably the best place for a definition that is far more succinct than we can offer.

> A scene graph is a collection of nodes in a graph or tree structure. A tree node (in the overall tree structure of the scene graph) may have many children but often only a single parent, with the effect of a parent applied to all its child nodes; an operation performed on a group automatically propagates its effect to all of its members. In many programs, associating a geometrical transformation matrix (see also transformation and matrix) at each group level and concatenating such matrices together is an efficient and natural way to process such operations. A common feature, for instance, is the ability to group related shapes/objects into a compound object that can then be moved, transformed, selected, etc. as easily as a single object.
 
## Structure

Using a tree metaphor, **po::scene** contains three main classes:

+ `po::scene::Scene` is the root system of the tree. It contains the trunk of the tree (the root Node), which in turn contains all branches and leaves.
+ `po::scene::NodeContainer` represents a branch. It can contain any number of other branches, as well as leaves.
+ `po::Scene::Node` represents a leaf, or an end point in the scene-graph. These Nodes are usually the point for any gl drawing.

![Structure diagram](images/diagram.jpg)

*Diagram of po::Scene structure*


Moving any branch of a tree will also move all of its connected branches and leaves. In po::scene moving, scaling, rotating or manipulating a `po::scene::NodeContainer` will transform all of the child Nodes within it.
![Bounds example](images/Bounds.png)
*Screenshot from the bounds example, describing the structure of a scene* 
## Nodes

Nodes are the basic building blocks of a scene. The po::scene::Node class is a base class that can not be used on its own, but is instead extended by a variety of inheriting classes that make up po::scene.
		
###Attributes
Nodes have a number of built in attributes that relate to how they appear in the scene:

+ `position` The coordinate of the Node's origin within it's parent or the scene. This is where 0,0 will be within the node's own coordinate space.
+ `rotation` The rotation of the Node along the z-axis (2d rotation). Expressed in degrees.
+ `scale` The x and y scale of the Node.
+ `fillColor` The color that is used for drawing the Node. This color is used differently depending on the type of drawing the Node does. Expressed as a ci::Color.
+ `alpha` The transparency of the Node. This is applied through the node tree. For example, if a parent Node has an alpha of 0.5 and the child has an alpha of 1.0, the child will draw at 0.5 alpha. Items with an alpha of 0.0 will not render but they will be considered for events.
+ `offset` The offset, relative to the Node's (0,0) origin, at which the Node's content will be drawn.
+ `alignment` Built-in enums used to set the Node's offset automatically, relative to its bounds. For example, CENTER_CENTER will set the Node's offset to `getBounds().getSize() * - 0.5`.

These attributes use setters and getters that allow the Node to internally keep track of things.
	
	using namespace po::scene;
	SomeNodeClassRef node = SomeNodeClass::create();
	node->setPosition(ci::Vec2f(25.0f, 25.0f)
	node->setFillColor(ci::Color(1.0f, 0.0f, 0.0f);
	node->setAlpha(0.5f);
	node->setRotation(45.0f);
	node->setAlignment(Alignment::CENTER_CENTER);
	
###Chaining
All Node attributes should return by reference, allowing for easy chaining when setting several parameters at once. Here is a more convenient way to write the above example:
	
	using namespace po::scene;
	SomeNodeClassRef node = SomeNodeClass::create();
	node->setPosition(ci::Vec2f(25.0f, 25.0f)
	.setFillColor(ci::Color(1.0f, 0.0f, 0.0f)
	.setAlpha(0.5f)
	.setRotation(45.0f)
	.setAlignment(Alignment::CENTER_CENTER);

	

###Animations
All Node attributes have corresponding ci::Anim objects. These animations can be set and they will be automatically applied to the Node's internal attributes. 

	// Animate a node from 0,0 to 50,50
	ci::app::timeline().apply(&node->getPositionAnim(), ci::Vec2f(50.0f, 50.0f), 1.0f);
	
Setting any Node attributes at any time during an animation will cancel the animation for that attribute.

	//Stop animation and retain current position
	node->setPosition(node->getPosition());

###Transformations
Every `po::scene::Node` has its own coordinate space. All `po::scene::Node` members contain functions for translating back and forth between various coordinate spaces. There are three main spaces:

+ `window` The origin of the application's window, as used by Cinder for all screen-based events.
+ `scene` The origin and transformation of a Node's Scene (as specified by the Scene's root Node).
+ `local` The origin and transformation of the Node.

There are several Node functions that can be used to translate between coordinate spaces:

+ `windowToLocal`/`localToWindow` Translate from/to window space.
+ `sceneToLocal`/`localToScene` Translate from/to scene space.
+ `nodeToLocal`/`localToNode` Translate from one Node's space into another Node's space. Equivalent to `node1->windowToLocal(node2->localToWindow(...))`.

###Events
All Nodes can respond to mouse and touch interaction events. These events come through as `po::scene::MouseEvent` and `po::scene::TouchEvent` class members. 

These events inherit from `po::scene::Event`, which in turn wraps Cinder's built-in `ci::app::Event` events while adding po::scene specific information.

All `po::scene` Events have the following information:

+ `ciEvent` A copy of the raw Cinder Event that this Event wraps.
+ `source` The Node that is subscribing to the Event.
+ `propagationEnabled` Controls whether or not this Event should continue through the draw tree if it is handled by the source Node.
+ `windowPos` The position of the Event in the window space.
+ `scenePos` The position of the Event in the source Node's Scene.
+ `localPos` The position of the Event within the source Node.

#####Cinder Events
`po::scene` wraps the following built-in global (window) Cinder Events for convenience and consistency:

+ Mouse Down, Mouse Move, Mouse Drag, Mouse Up
+ Touch Began, Touch Moved, Touch Ended. *Note: Cinder responds with a group of TouchEvents, po::scene responds with individual TouchEvents*

These Events will fire on all subscribing Nodes, in the order of the draw tree (bottom-up), regardless of the Node's bounds.

#####Node Events
`po::scene` adds additional Events that add interactivity to all Nodes. These Events are fired on the top-most Node first, and will only pass through Nodes if the `propagationEnabled` variable is set to `true`.

+ Mouse Down Inside, Mouse Move Inside, Mouse Drag Inside, Mouse Up Inside
+ Touch Began Inside, Touch Moved Inside, Touch Ended Inside

####Subscribing to events
`po::scene::Node` Events are signal-based, and can be retrieved using the `getSignal(SignalType)` syntax:
	
	using namespace po::scene;
	SomeNodeClassRef node = SomeNodeClass::create();
	// Connect node's Mouse Down Inside event to a member function of this class
	node->getSignal(MouseEvent::Type::DOWN_INSIDE).connect(std::bind(&ThisClass::mouseHandler, this, std::placeholders::_1);
	
	
###Visibility/Interaction
`po::scene::Node` contains two variables that affect interactivity. The `setVisible(bool enabled)` method allows the Node to stay in the Node hierarchy, but it will not render and it will not be considered in interaction events. It is a quick way to turn a Node on/off without having to add and remove it. 

`setInteractionEnabled(bool enabled)` sets a Node to be ignored for events, but still render to the screen.

On the other hand, to set a Node to be considered for interactive events but not render to the screen (e.g., to create an invisible hit area), set the Node's `alpha` to 0.

## NodeContainers

NodeContainers inherit from the Node class and can contain any number of Nodes or other NodeContainers. These are the branches of the scene, and they allow for hierarchy, grouping, and complex layouts. 

Adjusting any attributes of a NodeContainer also affects every Node that it contains.

A NodeContainer's bounds are determined by its child Nodes.

NodeContainer Events are based on all of the container's child Nodes. For example, if a NodeContainer subscribes to the `MouseEvent::Type::DOWN_INSIDE` event and the mouse is pressed, all of it's child nodes will be checked. If the mouse is down inside any of them, the event will fire.


## Scene

The `po::scene::Scene` class represents the main entry point for a scene graph. Any program can contain multiple scene graphs, which will exist entirely independent of each other.

The Scene class requires a root Node, which inherits from `po::scene::NodeContainer`. This Node represents the beginning of the draw tree, and is the origin for the scene.

To create a Scene, you can either start with an empty (auto-created) root Node

	using namespace po::scene;
	SceneRef myScene = Scene::create();
	
You could then add child Nodes to this root Node using the `getRootNode()` function of `po::scene::NodeContainer`.
	
	NodeRef myRootNode = MyRootNodeClass::create();
	myScene->getRootNode()->addChild(myRootNode);
	
 or (more common) extend `po::scene::NodeContainer` and provide that as the root Node.
 
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
	

This in turn will traverse the node tree, automatically triggering the `update()` and `draw()` trees of every visible Node in the scene.


## Cinder Nodes

po::scene provides a number of convenient wrappers around Cinder classes that allow users to attach various renderable objects rather than trying to extend these or re-implement these classes. These allow renderable Cinder objects to gain hit-testing, matrix transforms, animations, and everything else provided by the `po::scene::Node` class.

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
	

![Shapes example](images/exampleShapes.png)<br>
*Examples of the above shapes drawn on a 300x300 app window* 


These create and set the backing `ci::Shape2d`. This can be replaced at any time by the user.

This shape will be drawn using the `po::scene::Node` fillColor attribute, and hit-testing will be done using the built in `ci::Shape2d` contains method.

In addition, a `ci::gl::TextureRef` can be attached to it to any `po::scene::Shape` and mapped using a number of alignments.


![Texture example](images/exampleTextureShape.png)<br>
*Texture drawn on an ellipse; see the ShapeTextureSample for changes in alignment and additional shapes* 

### po::scene::Video
`po::scene::Video` provides a Node that wraps a Cinder movie player. Because there are numerous Cinder video players, it was created as a generic wrapper. It provides a `po::scene::VideoGl` type, which uses the Cinder Quicktime player. You can access or change the Cinder movie and texture reference by calling `setMovieRef` and `getMovieRef`.

	po::scene::VideoGLRef poVideo = po::scene::VideoGl::create();
	 
	//	create the Cinder movie reference
	ci::fs::path moviePath = ci::app::getAssetPath("path_to_movie");
	ci::qtime::MovieGlRef movieRef = ci::qtime::MovieGl::create(moviePath);
	
	//	access the Cinder movie reference from VideoGL node
	poVideo->setMovieRef(movieRef);
	poVideo->getMovieRef()->play();
	

### po::scene::TextBox

`po::scene::TextBox` wraps the `ci::TextBox` class.

You can create a `po::scene::TextBox` with an preexisting `ci::TextBox`; alternatively, `po::scene::TextBox::create()` will return a `po::scene::TextBox` wrapping a newly created `ci::TextBox`.

To set the `ci::TextBox` member of the `po::scene::TextBox`, call the `setCiTextBox` method. This automatically renders the Cinder TextBox and creates a new texture reference.

The bounds of a `po::scene::TextBox` Node are the bounds of that texture or, if no texture has been created, `getBounds()` returns a new `ci::Rectf`.

To manipulate the underlying `ci::TextBox`, call `getCiTextBoxCopy()`.

## Custom Nodes
Custom nodes should be made for anything that needs to draw into the scene graph and be considered for hit-testing/interaction.

To create a Node, extend this class and implement the following methods:

+ `update()` Called every frame, do any general code updates here before drawing.
+ `draw()` This draws the Node. Override this method and do any OpenGL drawing here. When this method is called all transforms have been applied to the Node (i.e. position, scale, rotation), so everything is relative to the Node's (0,0) origin.
+ `getBounds()` This is necessary for any hit-testing of an object. Return a ci::Rectf with the Top Left and Bottom Right of the content drawn (or a wider area if you'd like to expand the hit area). For example, the po::scene::Image class returns the top left as 0,0 (relative to the Node's origin) and the width and height of the associated `ci::gl::TextureRef`.

		ci::Rectf Image::getBounds()
		{
			return ci::Rectf(0,0,mTexture->getWidth(),mTexture->getHeight());
		}
		

Custom NodeContainers are often convenient, but they will draw their children automatically and calculate their bounds, so it is not necessary to override any of the above methods.

## History

[Potion](http://www.potiondesign.com) is an interactive design and development studio based in New York City. [Potion](http://www.potiondesign.com) was founded in 2005 with the idea of bringing interaction beyond the desktop and into the real world. At the time the tools for doing this type of work were few and far between, and most of our work involved writing many things from scratch. Examples include Vision Systems, custom touchscreens, and numerous OpenGL implementations.

As Potion began to grow, our OpenGL work slowly started to evolve into our internal framework [poCode](http://www.pocode.org/). We executed many projects and improved [poCode](http://www.pocode.org/) over the course of nearly 6 years, but supporting the basic infrastructure for a growing number of platforms, library updates and devices soon became a larger task than we could handle. 

Around 2010 a number of open-source C++ frameworks had began to emerge that covered a lot of the basic functionality (and beyond) of [poCode](http://www.pocode.org/), and they also began to amass a large base of users. At the same time, the core feature of [poCode](http://www.pocode.org/) that we used for every project was the scene graph, and there did not seem to be a clearly superior implementation of this in any other frameworks.

[Potion](http://www.potiondesign.com) made the decision in 2014 to move to [Cinder](http://libcinder.org/), and at the same time rewrite our scene graph to work on top of all of the great work that the [Cinder](http://libcinder.org/) team had done (and continue to do). Since then we have planned, written, tested, re-planned, and rewritten our scene graph while using it to complete several large-scale interactive installations.

We now feel ready to release po::scene, a 2D scene graph built on top of [Cinder](http://libcinder.org/). It is a tool that we find extremely useful, and we're hoping others do as well. It is not right for every project, but we are hopeful that there are enough people out there doing similar work so that it will help save a few people from rewriting scene graphs from scratch for each project. We would love it if others gave it a try and let us know what they think. We're happy to answer any questions (within reason) and respond to pull requests, address bugs, etc.

At the same time, regardless of the amount of people who decide to use it, this is the library that we will use for the foreseeable future for the majority of our installations, so it will be regularly tested, updated, fixed and improved for our own benefit.

We are happy to have access to the wonderful resource that is Cinder, so hopefully we can pay it forward by open sourcing this library.