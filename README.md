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
IPGLBuffer* buffer = context->CreateBuffer(BufferType::VERTEX_BUFFER, data, sizeof(data), BufferUsage::STATIC);

// Create a texture
IPGLTexture2D* texture = context->CreateTexture2D(width, height, TextureFormat::RGBA16F, nullptr, 0);

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

});
t.join();

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
```

This is how you load the individual shader programs and link them together manually:

```cpp
IPGLDeviceContext* context = ...;

// Load the effect with a given filename
IPGLShaderProgram* vertexShader = context->CreateShaderProgramFromFile("myfile.vs", ShaderProgramType::VERTEX_SHADER);
IPGLShaderProgram* fragmentShader = context->CreateShaderProgramFromFile("myfile.fs", ShaderProgramType::FRAGMENT_SHADER);
IPGLShaderProgram* programs[2] = { vertexShader, fragmentShader };
IPGLEffect* effect = context->CreateEffectFromPrograms(programs, 2);

// You can also load from memory
IPGLShaderProgram* vertexShader = context->CreateShaderProgramFromMemory(memoryPtr, memorySize, ShaderProgramType::VERTEX_SHADER);
IPGLShaderProgram* fragmentShader = context->CreateShaderProgramFromFile(fragMemoryPtr, fragMemorySize, ShaderProgramType::FRAGMENT_SHADER);
IPGLShaderProgram* programs[2] = { vertexShader, fragmentShader };
IPGLEffect* effect = context->CreateEffectFromPrograms(programs, 2);
```

You use an effect by binding the effect to the IPGLDeviceContext:

```cpp
IPGLDeviceContext* context = ...;
IPGLEffect* effect = ...;

// Apply the effect
IPGLRenderState* state = context->Apply(effect);

// Draw some vertex- and index buffer
state->Draw(vertexBuffer, indexBuffer);

// When the drawing is complete then end the frame
state->EndFrame();
```
