# PlayGL (Work in progress) #

PlayGL is an object-oriented multithread friendly interface for OpenGL 3.3 and above and is used by my other projects.

Start with creating a device containing the Application -> OpenGL connection. The device is thread-safe and can be used by any thread you want:
```cpp
PGL_HANDLE windowHandle = ...;
PGL_DEVICE_INFO deviceInfo;
deviceInfo.window = windowHandle;
deviceInfo.format = DeviceInfoFormat::R8G8B8A8;
IPGLDevice* device = POGLCreateDevice(&deviceInfo);

// Release the device and delete the devices internal memory
device->Release();
```

You then retrieve a device context for the thread you are using;
```cpp
IPGLDevice* device = ....;

// Create and bind a device context to the current thread and then return it to the program
IPGLDeviceContext context = device->GetContext();

// Return the context to the device and unbind it from the current thread
context->Release();
```

You use the IPGLDeviceContext to manage and create OpenGL resources, such as vertex buffers and textures:
```cpp
IPGLDevice* device = ....;

// Create and bind a device context to the current thread and then return it to the program
IPGLDeviceContext context = device->GetContext();

// Create a static buffer and fill it with data
PositionVertex data[6] = {...};
IPGLBuffer* buffer = context->CreateBuffer(BufferType::VERTEXBUFFER, &data, sizeof(data), BufferMode::STATIC);

// Return the context to the device and unbind it from the current thread
context->Release();
```

### Multithreading ###

Using PlayGL in a multithreaded environment is easy. Example:
```cpp
IPGLDevice* device = ....;

// Create and bind a device context to the current thread and then return it to the program
IPGLDeviceContext context = device->GetContext();

std::thread t([device] {
  // Create and bind a new device context to the current thread.
  IPGLDeviceContext* threadContext = device->GetContext();

  // Unbind the context and release it from the current thread
  threadContext->Release();
});

  // Unbind the context and release it from the current thread
context->Release();

// Release OOGL
device->Release();

```

IPGLDevice keeps track on which thread you are inside at the moment and returns a unqiue IPGLDeviceContext for each thread. This also means that you are not allowed to save a pointer to the context unless you are sure that you're always inside the same thread.
