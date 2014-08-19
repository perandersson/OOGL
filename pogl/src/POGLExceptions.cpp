#include "MemCheck.h"
#include "POGLDevice.h"

#include <string>
#ifdef UNICODE
std::wstring GenExceptionMessage(const char* format, va_list argp)
{
	POGL_CHAR tmp[5096];
	vswprintf_s(tmp, 5096, format, argp);
	return std::wstring(tmp);
}
#else
std::string GenExceptionMessage(const char* format, va_list argp)
{
	POGL_CHAR tmp[5096];
	vsprintf_s(tmp, 5096, format, argp);
	return std::string(tmp);
}
#endif

POGLException::POGLException(const POGL_CHAR* function, const POGL_UINT64 line, const POGL_CHAR* file)
{
	strcpy_s(mFunction, sizeof(mFunction), function);
	strcpy_s(mFile, sizeof(mFile), file);
	mLine = line;
}

POGLException::POGLException(const POGL_CHAR* function, const POGL_UINT64 line, const POGL_CHAR* file, const POGL_CHAR* message, ...)
{
	strcpy_s(mFunction, sizeof(mFunction), function);
	strcpy_s(mFile, sizeof(mFile), file);
	mLine = line;

	va_list arglist;
	va_start(arglist, message);
	strcpy_s(mMessage, sizeof(mMessage), GenExceptionMessage(message, arglist).c_str());
	va_end(arglist);
}

POGLException::~POGLException()
{
}

#pragma push_macro("GetMessage")
#undef GetMessage
const POGL_CHAR* POGLException::GetMessage() const 
{ 
	return mMessage;
}
#pragma pop_macro("GetMessage")

const POGL_CHAR* POGLException::GetFunction() const 
{
	return mFunction; 
}

const POGL_CHAR* POGLException::GetFile() const 
{ 
	return mFile; 
}

POGL_UINT64 POGLException::GetLine() const 
{ 
	return mLine; 
}

POGLResourceException::POGLResourceException(const POGL_CHAR* function, const POGL_UINT64 line, const POGL_CHAR* file, const POGL_CHAR* message, ...)
: POGLException(function, line, file)
{
	va_list arglist;
	va_start(arglist, message);
	strcpy_s(mMessage, sizeof(mMessage), GenExceptionMessage(message, arglist).c_str());
	va_end(arglist);
}

POGLResourceException::~POGLResourceException()
{
}

POGLProgramException::POGLProgramException(const POGL_CHAR* function, const POGL_UINT64 line, const POGL_CHAR* file, const POGL_CHAR* message, ...)
: POGLException(function, line, file)
{
	va_list arglist;
	va_start(arglist, message);
	strcpy_s(mMessage, sizeof(mMessage), GenExceptionMessage(message, arglist).c_str());
	va_end(arglist);
}

POGLProgramException::~POGLProgramException()
{
}

POGLStateException::POGLStateException(const POGL_CHAR* function, const POGL_UINT64 line, const POGL_CHAR* file, const POGL_CHAR* message, ...)
: POGLException(function, line, file)
{
	va_list arglist;
	va_start(arglist, message);
	strcpy_s(mMessage, sizeof(mMessage), GenExceptionMessage(message, arglist).c_str());
	va_end(arglist);
}

POGLStateException::~POGLStateException()
{
}

POGLInitializationException::POGLInitializationException(const POGL_CHAR* function, const POGL_UINT64 line, const POGL_CHAR* file, const POGL_CHAR* message, ...)
: POGLException(function, line, file)
{
	va_list arglist;
	va_start(arglist, message);
	strcpy_s(mMessage, sizeof(mMessage), GenExceptionMessage(message, arglist).c_str());
	va_end(arglist);
}

POGLInitializationException::~POGLInitializationException()
{
}

POGLNotImplementedException::POGLNotImplementedException(const POGL_CHAR* function, const POGL_UINT64 line, const POGL_CHAR* file, const POGL_CHAR* message, ...)
: POGLException(function, line, file)
{
	va_list arglist;
	va_start(arglist, message);
	strcpy_s(mMessage, sizeof(mMessage), GenExceptionMessage(message, arglist).c_str());
	va_end(arglist);
}

POGLNotImplementedException::~POGLNotImplementedException()
{
}
