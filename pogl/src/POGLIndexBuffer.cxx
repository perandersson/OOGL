#include "POGLIndexBuffer.hxx"
#include "POGLDeviceContext.hxx"
#include <atomic>
namespace {
	std::atomic<POGL_UINT32> uid;
	POGL_UINT32 GenIndexBufferUID() {
		return ++uid;
	}
}

POGLIndexBuffer::POGLIndexBuffer(GLuint bufferID, POGL_UINT32 typeSize, POGL_UINT32 numIndices, GLenum type, GLenum bufferUsage, IPOGLDevice* device)
: mRefCount(1), mUID(GenIndexBufferUID()), mBufferID(bufferID), mTypeSize(typeSize), mNumIndices(numIndices), mType(type), mBufferUsage(bufferUsage), mDevice(device)
{

}

POGLIndexBuffer::~POGLIndexBuffer()
{
	if (mBufferID != 0) {
		IPOGLDeviceContext* context = mDevice->GetDeviceContext();
		static_cast<POGLDeviceContext*>(context)->DeleteBuffer(mBufferID);
		context->Release();
		mBufferID = 0;
	}
}

void POGLIndexBuffer::AddRef()
{
	mRefCount++;
}

void POGLIndexBuffer::Release()
{
	if (--mRefCount == 0)
		delete this;
}

IPOGLDevice* POGLIndexBuffer::GetDevice()
{
	mDevice->AddRef();
	return mDevice;
}

POGL_HANDLE POGLIndexBuffer::GetHandlePtr()
{
	return this;
}

POGL_UINT32 POGLIndexBuffer::GetNumElements() const
{
	return mNumIndices;
}

POGL_UINT32 POGLIndexBuffer::GetUID() const
{
	return mUID;
}

GLuint POGLIndexBuffer::GetBufferID() const
{
	return mBufferID;
}
