#pragma once
#include "config.h"

/*!
	\brief A special state value container for the deferred render pipeline

	We want to prevent unneccessary state-changes, but a simple compare is not enough for the deferred render pipeline. 
	
	What we know is:

	* Commands put in the command queue will always be executed in the same order as they are put into the command queue.

	What we don't know is:

	* In which order the deferred render contexts are executed in the main thread.
	
	This means that we have to always create a state-change command in the command queue when it's called, even though it might already have the value we want to assign it.
	What does this mean performance-wise? It doesn't affect performance much. It will allocate some extra memory for the unneccessary commands. 
	When the command is executed in the main thread, the immediately render context will ensure that the command won't perform any unneccessary state changes.
*/
template<typename T>
class POGLDeferredStateValue
{
public:
	POGLDeferredStateValue(T defaultValue) : mAssigned(false), mValue(defaultValue) {

	}

	~POGLDeferredStateValue() {

	}

	bool Set(T value) {
		bool assigned = !mAssigned || mValue != value;
		if (assigned) {
			mValue = value;
		}
		mAssigned = true;
		return assigned;
	}

	void Unset() {
		mAssigned = false;
	}

	inline const T& GetValue() {
		return mValue;
	}

private:
	bool mAssigned;
	T mValue;
};