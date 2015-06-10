po::scene
======================================
###A Cinder scene graph from Potion

##History
Potion is an interactive design and development studio based in New York City. Potion was founded in 2005 with the idea of bringing interaction beyond the desktop and into the real world. At the time the tools for doing this type of work were few and far between, and most of our work involved writing many things from scratch. Examples include Vision Systems, custom touchscreens, and numerous OpenGL implementations.

As Potion began to grow, our OpenGL work slowly started to evolve into our internal framework "poCode". We executed many projects and improved poCode over the course of nearly 6 years, but supporting the basic infrastructure for a growing number of platforms, library updates and devices soon became a larger task than we could handle. 

Around 2010 a number of open-source C++ frameworks had began to emerge that covered a lot of the basic functionality (and beyond) of poCode, and they also began to ammas a large base of users. At the same time, the core feature of poCode that we used for every project was the Scene Graph, and there did not seem to be a clear implementation of this in any other frameworks.

Potion made the decision in 2014 to move to Cinder, and at the same time rewrite our Scene Graph to work on top of all of the great work that the Cinder team had done and continued to do. Since then we have planned, written, tested, re-planned, and rewritten our scene graph while using it to complete 2 large-scale interactive installations.

We now feel ready to release po::scene, a 2D scene graph built on top of Cinder. It is a tool that we find extremely useful, and we're hoping others do as well. It is not right for every project, but we are hopeful that there are enough people out there doing similar work that it will help save a few people from rewriting scene graphs from scratch for each project. We would love it if others gave it a try and let us know what they think. We're happy to answer any questions (within reason) and respond to pull request, address bugs, etc. that anyone might find.

At the same time, regardless of the amount of people who decide to use it, this is the library that we will use for the foreseeable future for the majority of our installations, so it will be regularly tested, updated, fixed and improved for our own benefit.

We are happy to have access to the wonderful resource that is Cinder, so hopefully we can pay it forward by open sourcing this library.

##What is a Scene Graph
[Wikipedia](http://en.wikipedia.org/wiki/Scene_graph) is probably the best place for a definition that is far more succinct than we can offer.

> A scene graph is a collection of nodes in a graph or tree structure. A tree node (in the overall tree structure of the scene graph) may have many children but often only a single parent, with the effect of a parent applied to all its child nodes; an operation performed on a group automatically propagates its effect to all of its members. In many programs, associating a geometrical transformation matrix (see also transformation and matrix) at each group level and concatenating such matrices together is an efficient and natural way to process such operations. A common feature, for instance, is the ability to group related shapes/objects into a compound object that can then be moved, transformed, selected, etc. as easily as a single object.

##po::scene
**po::scene** is a 2D Scene graph for working with layout, animation and interaction of multiple objects and views within Cinder. It is at it's most useful when used with applications that contain a large amount of interative objects with various states and attributes.

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
 

##Using po::scene

Using a tree metaphor, **po::scene** contains three main classes:

+ `po::scene::Scene` is the root system of the tree. It contains the trunk of the tree (the root node), which in turn contains all branches and leaves.
+ `po::scene::NodeContainer` represents a branch. It can contain any number of other branches, as well as leaves.
+ `po::Scene::Node` represents a leaf, or an end point in the scene-graph.

Moving any branch of a tree will also move all of it's connected branches and leaves. In po::scene moving, scaling, rotating or manipulating a `po::scene::NodeContainer` will transform all of the child nodes within it.

### Scene
The `po::scene::Scene` class represents the main entry point for a scene-graph. Any program can contain multiple Scene graphs, which will exist entirely independent of each other.

The Scene class requires a Root Node, which inherits from `po::scene::NodeContainer`. This node represents the beginning of the draw tree, and is the origin for the scene.

To create a scene, you can either start with an empty root node

	using namespace po::scene;
	SceneRef myScene = Scene::create();
	
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
	

This in turn will traverse the Node Tree, automatically triggering the `update()` and `draw()` trees of every node in the scene.

###Nodes
Nodes are the basic building blocks of a scene. The po::scene::Node class is a base class that can not be used on it's own. To create a Node, extend this class and implement the following methods:

+ `update()` Called every frame, do any general code updates here before drawing.
+ `draw()` This draws the Node. Override this method and do any OpenGL drawing here. When this method is called all transforms have been applied to the node (i.e. position, scale, rotation), so everything is relative to the Node's 0,0 origin
+ `getBounds()` This is necessary for any hit-testing of an object. Return a ci::Rectf with the Top Left and Bottom Right of the content drawn (or a wider area if you'd like to expand the hit area). For example, the po::scene::Image class returns the top left as 0,0 (relative to the node's origin) and the width and height of the associated `ci::gl::TextureRef`.

		ci::Rectf Image::getBounds()
		{
			return ci::Rectf(0,0,mTexture->getWidth(),mTexture->getHeight());
		}
		
		IMAGE EXPLAINING BOUNDS HERE?

###Node Containers
Node Containers inherit from the Node class and can contain any number of Nodes or other Node Containers. These are the branches of the scene, and they allow for hierarchy, grouping, complex layouts. 

Adjusting any attributes of a node container also effects every node that it contains.

IMAGE EXPLAINING NODE TRANSFORMATIONS HERE? 

###Interaction

IMAGE EXPLAINING EVENTS HERE

+ Mouse Events 
	+ Down
	+ Down Inside
	+ Up
	+ Up Inside
	+ Move
	+ Move Inside
	+ Drag
	+ Drag Inside 
+ Touch Events
	+ Began
	+ Began Inside
	+ Moved
	+ Moved Inside
	+ Ended
	+ Ended Inside 

###Cinder Nodes
po::scene provides a number of convenient wrappers around Cinder classes that allow users to attach various renderable objects rather than trying to extend these or reimpliment these classes. These allow renderable Cinder objects to gain hit-testing, matrix transforms, animations, and everything else provided by the `po::scene::Node` class.

####po::scene::Image
`po::scene::Image` provides a simple Node that displays a `ci::gl::TextureRef`.

####po::scene::Shape
`po::scene::Shape` is a more complex wrapper for both the `ci::Shape2d` class as well as the `ci::gl::TextureRef` class. 

The first way to use a `po::scene::Shape` is as a solid-filled shape. `po::scene::Shape` provides number of convenience methods for this purpose:

	using namespace po::scene;
	ShapeRef myRect = Shape::createRect(100,200);
	ShapeRef mySquare = Shape::createSquare(100);
	ShapeRef myEllipse = Shape::createEllipse(100,200);
	ShapeRef myCircle = Shape::createCircle(100);
	
EXAMPLE PICTURE OF THIS

These create and set the backing `ci::Shape2d`. This can be replaced at any time by the user.

This shape will be drawn using the `po::scene::Node` fillColor attribute, and hit-testing will be done using the built in `ci::Shape2d` contains method.

In addition, a `ci::gl::TextureRef` can be attached to it to any `po::scene::Shape` and mapped using a number of alignments.

EXAMPLE OF TEXTURES AND ALIGNMENTS

####po::scene::Video

####po::scene::TextBox
