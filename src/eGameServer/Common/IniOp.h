#ifndef _INIOP_H_
#define _INIOP_H_

//#include <tchar.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef COMMON_EXPORTS
#define INIOP_API __declspec(dllexport)
#else
#define INIOP_API __declspec(dllimport)
#endif

#ifdef UNICODE
#define INI_GetString INI_GetStringW
#define INI_WriteString INI_WriteStringW
#else
#define INI_GetString INI_GetStringA
#define INI_WriteString INI_WriteStringA
#endif

	INIOP_API UINT INI_GetInt(LPCTSTR lpAppName, LPCTSTR lpKeyName, INT nDefault, LPCTSTR lpFileName);

	INIOP_API DWORD INI_GetStringA(LPCSTR lpAppName, LPCSTR lpKeyName, LPCSTR lpDefault, LPSTR lpReturnedString, DWORD dwSize, LPCSTR lpFileName);
	INIOP_API BOOL INI_WriteStringA(LPCSTR lpAppName, LPCSTR lpKeyName, LPCSTR lpString, LPCSTR lpFileName);

	INIOP_API DWORD INI_GetStringW(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault, LPTSTR lpReturnedString, DWORD dwSize, LPCTSTR lpFileName);
	INIOP_API BOOL INI_WriteStringW(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpString, LPCTSTR lpFileName);

#ifdef __cplusplus
}
#endif



#endif