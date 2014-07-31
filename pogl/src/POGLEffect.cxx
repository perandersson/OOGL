#include "POGLEffect.hxx"

POGLEffect::POGLEffect(IPOGLDevice* device)
: mRefCount(1), mDevice(device)
{

}

POGLEffect::~POGLEffect()
{
}

void POGLEffect::AddRef()
{
	mRefCount++;
}

void POGLEffect::Release()
{
	if (--mRefCount == 0)
		delete this;
}

IPOGLDevice* POGLEffect::GetDevice()
{
	mDevice->AddRef();
	return mDevice;
}
