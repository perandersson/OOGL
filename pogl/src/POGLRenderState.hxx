#pragma once
#include "config.hxx"
#include <gl/pogl.hxx>

class POGLRenderState : public IPOGLRenderState
{
public:
	POGLRenderState(IPOGLDevice* device);
	~POGLRenderState();

	void AddRef();
	void Release();
	
	IPOGLDevice* GetDevice();
	void Clear(POGL_UINT32 clearBits);
	IPOGLUniform* FindUniformByName(const POGL_CHAR* name);
	void Draw(IPOGLBuffer* vertexBuffer);
	void Draw(IPOGLBuffer* vertexBuffer, IPOGLBuffer* indexBuffer);
	void Draw(IPOGLBuffer* vertexBuffer, IPOGLBuffer* indexBuffer, POGL_UINT32 vertexCount);
	void Draw(IPOGLBuffer* vertexBuffer, IPOGLBuffer* indexBuffer, POGL_UINT32 vertexCount, POGL_UINT32 offset);
	void EndFrame();

	/*!
		\brief Applies the supplied effect to this render state
	*/
	IPOGLRenderState* Apply(IPOGLEffect* effect);

private:
	POGL_UINT32 mRefCount;
	IPOGLDevice* mDevice;

	IPOGLEffect* mCurrentEffect;
};
