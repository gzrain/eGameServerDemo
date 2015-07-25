
#ifndef GUID_H_
#define GUID_H_

#include <objbase.h>
#include <string>
using namespace std;

#ifdef COMMON_EXPORTS
#define GUID_API __declspec(dllexport)
#else
#define GUID_API __declspec(dllimport)
#endif

#ifndef COS_GUID
#define COS_GUID GUID
#endif

#ifdef __cplusplus
		extern "C" {
#endif

#define MIN_GUID_STR_LEN 39

#ifdef UNICODE
#define CrossPlatform_GUID2String CrossPlatform_GUID2StringW
#define CrossPlatform_GUID2String_WithoutBraces CrossPlatform_GUID2String_WithoutBracesW
#define CrossPlatform_FormatGuidString CrossPlatform_FormatGuidStringW
#else
#define CrossPlatform_GUID2String CrossPlatform_GUID2StringA
#define CrossPlatform_GUID2String_WithoutBraces CrossPlatform_GUID2String_WithoutBracesA
#define CrossPlatform_FormatGuidString CrossPlatform_FormatGuidStringA
#endif

/**\brief compatible with linux*/
GUID_API int CrossPlatform_CreateGuid(COS_GUID* pGuid);
GUID_API BOOL CrossPlatform_ValidGuid(const COS_GUID* pGuid);
GUID_API void CrossPlatform_InValidGuid(COS_GUID* pGuid);
GUID_API int CrossPlatform_CopyGuid(COS_GUID* pGuidDes, const COS_GUID* pGuidSrc);
GUID_API int CrossPlatform_CompareGuid(const COS_GUID &GuidDes, const COS_GUID &GuidSrc);
GUID_API int CrossPlatform_GUID2StringA(const COS_GUID &guid, CHAR *pszGuidStr, int nStrLen);
GUID_API int CrossPlatform_GUID2StringW(const COS_GUID &guid, TCHAR *pszGuidStr, int nStrLen);
GUID_API int CrossPlatform_StringToGuid(LPCTSTR lpszGuid, COS_GUID *pGuid);

GUID_API int CrossPlatform_GUID2String_WithoutBracesA(const COS_GUID &guid, CHAR *pszGuidStr, int nStrLen);
GUID_API int CrossPlatform_GUID2String_WithoutBracesW(const COS_GUID &guid, TCHAR *pszGuidStr, int nStrLen);

GUID_API void CrossPlatform_FormatGuidStringA(string &strGuid);
GUID_API void CrossPlatform_FormatGuidStringW(wstring &strGuid);


#ifdef __cplusplus
		}
#endif

#endif /*GUID_H_*/
