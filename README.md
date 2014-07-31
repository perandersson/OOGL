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
PGL_BUFFER_INFO bufferInfo;
bufferInfo.data = data;
bufferInfo.size = sizeof(data);
bufferInfo.mode = BufferMode::STATIC;
bufferInfo.type = BufferType::VERTEX_BUFFER;
IPGLBuffer* buffer = context->CreateBuffer(&bufferInfo);

PGL_TEXTURE2D_INFO textureInfo;
textureInfo.width = 128;
textureInfo.height = 128;
textureInfo.format = TextureFormat::RGBA16F;
textureInfo.data = nullptr;
textureInfo.size = 0;
IPGLTexture2D* texture = context->CreateTexture2D(&textureInfo);

IPGLFrameBuffer* frameBuffer = context->CreateFrameBuffer();
frameBuffer->SetRenderTarget(0, texture);
texture->Release(); // The framebuffer not owns the reference

// Release the context and delete it's internal resources
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

### Effects ###

You can load effects in one of two ways. Those are:

1. Load an effect file containing the different shaders
2. Load the individual shader programs and link them together manually.

This is how you load an effect file:

```cpp
IPGLDeviceContext* context = ...;

// Load the effect with a given filename
IPGLEffect* effect = context->CreateEffectFromFile("myfile.effect");

// You can also load the effect from memory
const char* memory = ...;
size_t memorySize = ...;
IPGLEffect* effect = context->CreateEffectFromMemory(memory, memoeySize);

context->Release();
```

This is how you load the individual shader programs and link them together manually:

```cpp
IPGLDeviceContext* context = ...;

// Load the effect with a given filename
IPGLShaderProgram* vertexShader = context->CreateShaderProgramFromFile("myfile.vs", ShaderProgramType::VERTEX_SHADER);
IPGLShaderProgram* fragmentShader = context->CreateShaderProgramFromFile("myfile.fs", ShaderProgramType::FRAGMENT_SHADER);
PGL_EFFECT_PROGRAMS programs[2] = {
  { vertexShader, ShaderProgramType::VERTEX_SHADER },
  { fragmentShader, ShaderProgramType::FRAGMENT_SHADER }
};
IPGLEffect* effect = context->CreateEffectFromPrograms(programs, 2);

// You can also load from memory
// .....

context->Release();
```
