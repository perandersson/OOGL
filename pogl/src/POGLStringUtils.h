#pragma once
#include "config.h"
#include <gl/pogl.h>
#include <locale>
#include <codecvt>
#include <algorithm>

class POGLStringUtils
{
public:
	static std::vector<POGL_STRING> Split(const POGL_STRING& str, const POGL_CHAR delim) {
		std::vector<POGL_STRING> elems;
		return Split(str, delim, elems);
	}

	static POGL_STRING RemoveWhitespaces(const POGL_STRING& str) {
		static const POGL_STRING NOTHING;
		return Replace(Replace(Replace(str, POGL_TOCHAR('\r'), NOTHING), POGL_TOCHAR('\t'), NOTHING), POGL_TOCHAR('\n'), NOTHING);
	}

	static bool StartsWith(const POGL_STRING& value, const POGL_STRING& prefix) {
		if (value.length() < prefix.length()) {
			return false;
		}

		POGL_STRING::size_type size = prefix.length();
		for (POGL_STRING::size_type i = 0; i < size; ++i) {
			if (value[i] != prefix[i])
				return false;
		}

		return true;
	}

	static POGL_STRING Replace(const POGL_STRING& value, const POGL_CHAR replace, const POGL_STRING& newval) {
		POGL_STRING result;
		for (size_t i = 0; i < value.size(); ++i) {
			if (value[i] == replace) {
				result += newval;
			}
			else {
				result += value[i];
			}
		}
		return result;
	}

	static POGL_STRING ToLowerCase(const POGL_STRING& otherString) {
		POGL_STRING data = otherString;
		std::transform(data.begin(), data.end(), data.begin(), ::tolower);
		return data;
	}

	static POGL_STRING ToUpperCase(const POGL_STRING& otherString) {
		POGL_STRING data = otherString;
		std::transform(data.begin(), data.end(), data.begin(), ::toupper);
		return data;
	}

	static POGL_STRING ToLowerCase(const POGL_CHAR* otherString) {
		POGL_STRING data(otherString);
		std::transform(data.begin(), data.end(), data.begin(), ::tolower);
		return data;
	}
	
	/*!
		\brief Convert the supplied unicode string into a POGL_STRING

		\param in
		\return
	*/
	static POGL_STRING ToString(const std::wstring& in) {
#ifdef UNICODE
		return in;
#else
		typedef std::codecvt_utf8<wchar_t> convert_typeX;
		std::wstring_convert<convert_typeX, wchar_t> converterX;
		return POGL_STRING(converterX.to_bytes(in.c_str()));
#endif
	}
	
	/*!
		\brief Convert the supplied ASCII string into a POGL_STRING

		\param in
		\return
	*/
	static POGL_STRING ToString(const std::string& in) {
#ifdef UNICODE
		typedef std::codecvt_utf8<wchar_t> convert_typeX;
		std::wstring_convert<convert_typeX, wchar_t> converterX;
		return POGL_STRING(converterX.from_bytes(in.c_str()));
#else
		return in;
#endif
	}

private:
	static std::vector<POGL_STRING>& Split(const POGL_STRING& str, const POGL_CHAR delim, std::vector<POGL_STRING>& elems) {
		POGL_STRINGSTREAM ss(str);
		POGL_STRING item;
		while (std::getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}
};
