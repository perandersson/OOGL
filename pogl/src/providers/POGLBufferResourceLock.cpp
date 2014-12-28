#include "MemCheck.h"
#include "POGLBufferResourceLock.h"

POGLBufferResourceLock::POGLBufferResourceLock()
{
}

POGLBufferResourceLock::~POGLBufferResourceLock()
{
	auto end = mFences.end();
	for (auto it = mFences.begin(); it != end; ++it) {
		free(*it);
	}
	mFences.clear();

	end = mFreeFences.end();
	for (auto it = mFreeFences.begin(); it != end; ++it) {
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
	auto end = mFences.end();
	for (auto it = mFences.begin(); it != end; ++it) {
		auto fence = *it;
		if (fence->lock == nullptr)
			fence->lock = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
		CHECK_GL("Could not create memory fence");
	}
}

void POGLBufferResourceLock::WaitAndClear()
{
	auto end = mFences.end();
	for (auto it = mFences.begin(); it != end; ++it) {
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
	auto end = mFences.end();
	for (auto it = mFences.begin(); it != end;) {
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
	auto end = mFences.end();
	for (auto it = mFences.begin(); it != end; ++it) {
		auto fence = *it;
		glClientWaitSync(fence->lock, 0, GL_TIMEOUT_IGNORED);
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
	auto end = mFences.end();
	for (auto it = mFences.begin(); it != end;) {
		auto fence = *it;
		if (IsInRange(fence, offset, length)) {
			glClientWaitSync(fence->lock, 0, GL_TIMEOUT_IGNORED);
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

	auto it = mFreeFences.begin();
	auto fence = *it;
	mFreeFences.erase(it);
	return fence;
}
