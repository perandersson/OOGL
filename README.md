# POGL (PlayOpenGL) #

PlayOpenGL is an object-oriented multithread friendly interface for OpenGL 3.3 and above and is used by my other projects.

## What is it? ##

POGL is used to simplify OpenGL over multiple threads, both with resource sharing and state management. 

## Requirements ##

* CMake 2.6, 3.x
* C++11 Compatible Compiler

## Examples ##

### Example 1: Create Device ###

This example contains an example how to create an IPOGLDevice instance, which is the entry-point for the POGL framework

### Example 2: Draw Triangle ###

This example shows how to draw a triangle using the POGL framework

![alt text](https://raw.githubusercontent.com/perandersson/POGL/master/example2_drawtriangle/screenshot.png "Example 2")

### Example 3: Blending ###

Example showing how to use blending

![alt text](https://raw.githubusercontent.com/perandersson/POGL/master/example3_blending/screenshot.png "Example 3")

### Example 4: Texture Mapping ###

Example showing perform texture mapping

![alt text](https://raw.githubusercontent.com/perandersson/POGL/master/example4_texturing/screenshot.png "Example 4")

### Example 5: Threaded Vertices ###

Example showing creating and updating vertices in separate threads using the Map-Unmap functionality and render the same vertex buffer
at the same time.

![alt text](https://raw.githubusercontent.com/perandersson/POGL/master/example5_threadedvertices/screenshot.png "Example 5")

### Example 6: Custom Vertex Type ###

Example showing how we can create custom vertex types instead of using the build-in ones.

![alt text](https://raw.githubusercontent.com/perandersson/POGL/master/example6_customvertextype/screenshot.png "Example 5")