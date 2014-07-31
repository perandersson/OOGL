# PlayGL (Work in progress) #

PlayGL is an object-oriented multithread friendly interface for OpenGL 3.3 and above and is used by my other projects.

Start with creating a device containing the Application -> OpenGL connection. The device is thread-safe and can be used by any thread you want:
```cpp
PGL_HANDLE windowHandle = ...;
PGL_DEVICE_INFO deviceInfo;
deviceInfo.window = windowHandle;
deviceInfo.format = DeviceInfoFormat::R8G8B8A8;
IPGLDevice* device = PGLCreateDevice(&deviceInfo);

// Release the device and delete the devices internal memory
device->Release();
```

You then retrieve a device context for the thread you are using;
```cpp
IPGLDevice* device = ....;

// Create a device context
IPGLDeviceContext* context = device->CreateDeviceContext();

// Return the context to the device and unbind it from the current thread
context->Release();

// Release the device and delete the devices internal memory
device->Release();
```

You use the IPGLDeviceContext to manage and create OpenGL resources, such as vertex buffers and textures:
```cpp
IPGLDevice* device = ....;

// Create a device context
IPGLDeviceContext* context = device->CreateDeviceContext();

// Create a static buffer and fill it with data
PositionVertex data[6] = {...};
IPGLBuffer* buffer = context->CreateBuffer(BufferType::VERTEXBUFFER, &data, sizeof(data), BufferMode::STATIC);

// Return the context to the device and unbind it from the current thread
context->Release();

// Release the device and delete the devices internal memory
device->Release();
```

### Multithreading ###

Using PlayGL in a multithreaded environment is easy. Example:
```cpp
IPGLDevice* device = ....;

// Create a device context
IPGLDeviceContext* context = device->CreateDeviceContext();

std::thread t([context] {
  // Save a reference just in case
  context->AddRef();
  
  // Unbind the context and release it from the current thread
  context->Release();
});

  // Unbind the context and release it from the current thread
context->Release();

// Release OOGL
device->Release();
```

Resources are automatically shared between contexts. Client locks and fences are used internally to verify the data integrity for the shared resources.
