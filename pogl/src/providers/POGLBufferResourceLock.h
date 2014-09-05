#pragma once
#include "config.h"

class POGLBufferResourceLock
{
	struct Fence {
		POGL_UINT32 min;
		POGL_UINT32 max;
		GLsync lock;
	};
	typedef std::list<Fence*> Fences;

public:
	POGLBufferResourceLock();
	virtual ~POGLBufferResourceLock();

	void PrepareFence();
	void PrepareFence(POGL_UINT32 offset, POGL_UINT32 length);

	void AddFences();

	void WaitAndClear();
	void WaitAndClear(POGL_UINT32 offset, POGL_UINT32 length);

	void WaitClientAndClear();
	void WaitClientAndClear(POGL_UINT32 offset, POGL_UINT32 length);

	inline bool IsInRange(Fence* fence, POGL_UINT32 offset, POGL_UINT32 length) {
		const POGL_UINT32 min = offset;
		const POGL_UINT32 max = offset + length;
		return fence->max > min && fence->min < max;
	}

private:
	Fence* GetOrCreateFreeFence();

public:
	Fences mFences;
	Fences mFreeFences;
};
