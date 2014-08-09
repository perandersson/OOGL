#pragma once
#include "POGLResourceStream.h"

class POGLDeviceContext;
class POGLSyncObject;
class POGLBufferResourceStream : public POGLResourceStream
{
public:
	POGLBufferResourceStream(POGLDeviceContext* context);
	~POGLBufferResourceStream();

	/*!
		\brief Check to see if this stream is opened or not
	*/
	inline bool IsOpen() const {
		return mResource != nullptr;
	}

	/*!
		\brief Open the supplied resource

		\param resource
		\param target
		\param usage
		\param streamType
	*/
	void Open(IPOGLResource* resource, POGLSyncObject* syncObject, GLenum target, GLenum usage, POGLResourceStreamType::Enum streamType);

	void Update(POGL_UINT32 size, const void* data);
	void UpdateRange(POGL_UINT32 offset, POGL_UINT32 size, const void* data);
	void* Map();
	void* Map(POGL_UINT32 offset, POGL_UINT32 size);
	void Close();

private:
	IPOGLResource* mResource;
	POGLSyncObject* mSyncObject;
	GLenum mTarget;
	GLenum mUsage;
	bool mMapping;
	POGLResourceStreamType::Enum mStreamType;
};
