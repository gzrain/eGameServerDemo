#ifndef _UNICODE_CONVERTER_H_
#define _UNICODE_CONVERTER_H_

#include "tstring.h"

#ifdef COMMON_EXPORTS
#define UNICODECONVERTER_API __declspec(dllexport)
#else
#define UNICODECONVERTER_API __declspec(dllimport)
#endif

UNICODECONVERTER_API void unicodeToUTF8(const wstring &src, string& result);
UNICODECONVERTER_API void unicodeToGB2312(const wstring& wstr, string& result);
UNICODECONVERTER_API void utf8ToUnicode(const string& src, wstring& result);
UNICODECONVERTER_API void gb2312ToUnicode(const string& src, wstring& result);

UNICODECONVERTER_API void gb2312ToUTF8(const string &src, string& result);
UNICODECONVERTER_API void utf8ToGB2312(const string &src, string& result);


#endif