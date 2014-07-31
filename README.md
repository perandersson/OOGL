# OOGL (Work in progress) #

OOGL is an object-oriented multithread friendly interface for OpenGL 3.3 and above.

Start with creating a device containing the Application -> OpenGL connection. The device is thread-safe and can be used by any thread you want:
```cpp
OGL_HANDLE windowHandle = ...;
OGL_DEVICE_INFO deviceInfo;
deviceInfo.window = windowHandle;
deviceInfo.format = DeviceInfoFormat::R8G8B8A8;
IOOGLDevice* device = OOGLCreateDevice(&deviceInfo);

// Release the device and delete the devices internal memory
device->Release();
```

You then retrieve a device context for the thread you are using;
```cpp
IOGLDevice* device = ....;

// Create and bind a device context to the current thread and then return it to the program
IOGLDeviceContext context = device->GetContext();

// Return the context to the device and unbind it from the current thread
context->Release();
```

You use the IOGLDeviceContext to manage and create OpenGL resources, such as vertex buffers and textures:
```cpp
IOGLDevice* device = ....;

// Create and bind a device context to the current thread and then return it to the program
IOGLDeviceContext context = device->GetContext();

// Create a static buffer and fill it with data
PositionVertex data[6] = {...};
IOOGLBuffer* buffer = context->CreateBuffer(BufferType::VERTEXBUFFER, &data, sizeof(data), BufferMode::STATIC);

// Return the context to the device and unbind it from the current thread
context->Release();
```

### Multithreading ###

Using OOGL in a multithreaded environment is easy. Example:
```cpp
IOGLDevice* device = ....;

// Create and bind a device context to the current thread and then return it to the program
IOGLDeviceContext context = device->GetContext();

std::thread t([device] {
  // Create and bind a new device context to the current thread.
  IOOGLDeviceContext* threadContext = device->GetContext();

  // Unbind the context and release it from the current thread
  threadContext->Release();
});

  // Unbind the context and release it from the current thread
context->Release();

// Release OOGL
device->Release();

```

IOGLDevice keeps track on which thread you are inside at the moment and returns a unqiue IOGLDeviceContext for each thread. This also means that you are not allowed to save a pointer to the context unless you are sure that you're always inside the same thread.
