#pragma once
#include "config.h"

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