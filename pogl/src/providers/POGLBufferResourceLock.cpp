#include "MemCheck.h"
#include "POGLBufferResourceLock.h"

POGLBufferResourceLock::POGLBufferResourceLock()
{
}

POGLBufferResourceLock::~POGLBufferResourceLock()
{
	Fences::iterator end = mFences.end();
	for (Fences::iterator it = mFences.begin(); it != end; ++it) {
		free(*it);
	}
	mFences.clear();

	end = mFreeFences.end();
	for (Fences::iterator it = mFreeFences.begin(); it != end; ++it) {
		free(*it);
	}
	mFreeFences.clear();
}

void POGLBufferResourceLock::PrepareFence()
{
	PrepareFence(0, UINT_MAX);
}

void POGLBufferResourceLock::PrepareFence(POGL_UINT32 offset, POGL_UINT32 length)
{
	auto fence = GetOrCreateFreeFence();
	fence->min = offset;
	fence->max = offset + length;
	fence->lock = nullptr;
	mFences.push_back(fence);
}

void POGLBufferResourceLock::AddFences()
{
	Fences::iterator end = mFences.end();
	for (Fences::iterator it = mFences.begin(); it != end; ++it) {
		auto fence = *it;
		if (fence->lock == nullptr)
			fence->lock = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
		CHECK_GL("Could not create memory fence");
	}
}

void POGLBufferResourceLock::WaitAndClear()
{
	Fences::iterator end = mFences.end();
	for (Fences::iterator it = mFences.begin(); it != end; ++it) {
		auto fence = *it;
		glWaitSync(fence->lock, 0, GL_TIMEOUT_IGNORED);
		CHECK_GL("Could not wait for memory fence");
		glDeleteSync(fence->lock);
		CHECK_GL("Could not delete memory fence");
		fence->lock = nullptr;
		mFreeFences.push_back(fence);
	}
	mFences.clear();
}

void POGLBufferResourceLock::WaitAndClear(POGL_UINT32 offset, POGL_UINT32 length)
{
	Fences::iterator end = mFences.end();
	for (Fences::iterator it = mFences.begin(); it != end;) {
		auto fence = *it;
		if (IsInRange(fence, offset, length)) {
			glWaitSync(fence->lock, 0, GL_TIMEOUT_IGNORED);
			CHECK_GL("Could not wait for memory fence");
			glDeleteSync(fence->lock);
			CHECK_GL("Could not delete memory fence");
			fence->lock = nullptr;
			mFreeFences.push_back(fence);
			Fences::iterator prev = it;
			it++;
			mFences.erase(prev);
		}
		else {
			it++;
		}
	}
}

void POGLBufferResourceLock::WaitClientAndClear()
{
	Fences::iterator end = mFences.end();
	for (Fences::iterator it = mFences.begin(); it != end; ++it) {
		auto fence = *it;
		glClientWaitSync(fence->lock, GL_SYNC_FLUSH_COMMANDS_BIT, GL_TIMEOUT_IGNORED);
		CHECK_GL("Could not wait for memory fence");
		glDeleteSync(fence->lock);
		CHECK_GL("Could not delete memory fence");
		fence->lock = nullptr;
		mFreeFences.push_back(fence);
	}
	mFences.clear();
}

void POGLBufferResourceLock::WaitClientAndClear(POGL_UINT32 offset, POGL_UINT32 length)
{
	Fences::iterator end = mFences.end();
	for (Fences::iterator it = mFences.begin(); it != end;) {
		auto fence = *it;
		if (IsInRange(fence, offset, length)) {
			glClientWaitSync(fence->lock, GL_SYNC_FLUSH_COMMANDS_BIT, GL_TIMEOUT_IGNORED);
			CHECK_GL("Could not wait for memory fence");
			glDeleteSync(fence->lock);
			CHECK_GL("Could not delete memory fence");
			fence->lock = nullptr;
			mFreeFences.push_back(fence);
			Fences::iterator prev = it;
			it++;
			mFences.erase(prev);
		}
		else {
			it++;
		}
	}
}

POGLBufferResourceLock::Fence* POGLBufferResourceLock::GetOrCreateFreeFence()
{
	if (mFreeFences.empty()) {
		return (Fence*)malloc(sizeof(Fence));
	}

	Fences::iterator it = mFreeFences.begin();
	auto fence = *it;
	mFreeFences.erase(it);
	return fence;
}
