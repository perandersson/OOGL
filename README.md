# POGL (PlayOpenGL) #

PlayOpenGL is an object-oriented multithread friendly interface for OpenGL 3.3 and above and is used by my other projects.

## What is it? ##

POGL is used to simplify OpenGL over multiple threads, both with resource sharing and state management. 

## What is it not? ##

POGL have a high initial performance cost, relative to vanilla OpenGL. The reason for this is that a lot of code running is to prevent unnecessary state changes and to help synchronize OpenGL data between multiple render contexts. 

Using POGL only in a single thread will, at the moment, make use of synchronization methods (such as glFenceSync, glClientWaitSync and std::recursive_mutex). 
The glClientWaitSync function will lock the CPU until the GL server (Graphics Card driver) has reached the glFenceSync command on the OpenGL queue. This is, for example, to
prevent threads from working on an undefined/incomplete buffer. Some of the synchronizations that we are doing have an unnecessary cost in single-thread mode.

## Known issues ##

Visit the [Issues page](https://github.com/perandersson/POGL/labels/bug) for more information.

## Requirements ##

* CMake 2.6, 3.x
* C++11 Compatible Compiler

## Examples ##

### Example 1: Create Device ###

This example contains an example how to create an IPOGLDevice instance, which is the entry-point for the POGL library.

### Example 2: Draw Triangle ###

This example shows how to draw a triangle vertex buffer using a very simple Effect.

![alt text](https://raw.githubusercontent.com/perandersson/POGL/master/example2_drawtriangle/screenshot.png "Example 2")

### Example 3: Blending ###

Example showing how to blend two vertice colors on the screen.

![alt text](https://raw.githubusercontent.com/perandersson/POGL/master/example3_blending/screenshot.png "Example 3")

### Example 4: Texture Mapping ###

This examples shows you how to use texture mapping to display a texture onto the screen using a vertex- and index buffer.

![alt text](https://raw.githubusercontent.com/perandersson/POGL/master/example4_texturing/screenshot.png "Example 4")

### Example 5: Threaded Vertices ###

This example shows us how to use the POGL library in multiple threads at the same time.

We create one circle vertex buffer, update the same vertex buffer in two threads and render said vertex buffer onto the screen  at the same time.

![alt text](https://raw.githubusercontent.com/perandersson/POGL/master/example5_threadedvertices/screenshot.png "Example 5")

### Example 6: Custom Vertex Type ###

An example on how we can create custom vertex types instead of using any of the build-in ones.

![alt text](https://raw.githubusercontent.com/perandersson/POGL/master/example6_customvertextype/screenshot.png "Example 6")

### Example 7: Framebuffers ###

An example that shows how we can create a framebuffer with two attached render targets and one depth render target

![alt text](https://raw.githubusercontent.com/perandersson/POGL/master/example7_framebuffers/screenshot.png "Example 6")