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