#pragma once
#include "config.hxx"
#include <gl/pogl.hxx>

class POGLEffect : public IPOGLEffect
{
public:
	POGLEffect(IPOGLDevice* device);
	~POGLEffect();

	void AddRef();
	void Release();
	IPOGLDevice* GetDevice();
	
private:
	POGL_UINT32 mRefCount;
	IPOGLDevice* mDevice;
};
