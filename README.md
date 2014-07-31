OOGL
====

OOGL is an object-oriented multithread friendly approach to the OpenGL interface. Example (psuedo-ish code at the moment):

```cpp
//
IOOGLDevice* device = OOGLCreateDevice(...);
std::thread t([device] {
  // Bind the device to the current thread and return a thread-local context used 
  // to do the actual graphics related programming 
  IOOGLDeviceContext* context = device->Bind();

  // Fill a struct with data and create a buffer that we want to be static.
  // This might, depenending on your computers resources, put the data into a larger "static" buffer containing 
  // more than one buffer.
  PositionVertex data[6] = {...};
  IOOGLBuffer* buffer = context->CreateBuffer(BufferType::VERTEXBUFFER, &data, sizeof(data), BufferMode::STATIC);

  // Do stuff
  
  buffer->Release();
  
  // Unbind the context and release it from the current thread
  context->Release();
});

// Release OOGL
device->Release();

```
