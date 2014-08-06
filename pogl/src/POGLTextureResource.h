#pragma once
#include "config.h"
#include <gl/pogl.h>

class POGLSyncObject;
class POGLTextureResource : public IPOGLInterface
{
public:
	POGLTextureResource(GLuint textureID, GLenum textureTarget, POGLTextureFormat::Enum format, POGLSyncObject* syncObject, IPOGLDevice* device);
	virtual ~POGLTextureResource();

	void AddRef();
	void Release();

	void WaitSyncDriver();
	void WaitSyncClient();
	bool WaitSyncClient(POGL_UINT64 timeout);
	bool WaitSyncClient(POGL_UINT64 timeout, IPOGLWaitSyncJob* job);

	POGL_UINT32 GetUID() const;
	GLuint GetTextureID() const;
	GLenum GetTextureTarget() const;
	POGLTextureFormat::Enum GetTextureFormat() const;

private:
	POGL_UINT32 mRefCount;
	POGL_UINT32 mUID;
	GLuint mTextureID;
	GLenum mTextureTarget;
	POGLTextureFormat::Enum mTextureFormat;
	POGLSyncObject* mSyncObject;
	IPOGLDevice* mDevice;
};
