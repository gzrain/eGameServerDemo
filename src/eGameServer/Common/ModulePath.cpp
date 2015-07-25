#include "ModulePath.h"
#include <windows.h>
#include <tchar.h>

// tstring GetModulePath()
// {
// 	TCHAR lpFilename[256];
// 	GetModuleFileName(NULL, (LPTSTR)(lpFilename), 256);
// 
// 	tstring strPath(lpFilename);
// 	int nIndex = strPath.rfind(_T('\\'));
// 	if (nIndex != -1)
// 		strPath = strPath.substr(0, nIndex);
// 	if (strPath.empty() == FALSE && strPath.at(strPath.size() - 1) != _T('\\'))
// 	{
// 		strPath += _T("\\");
// 	}
// 	return strPath;
// }

string GetModulePathA()
{
	CHAR lpFilename[256];
	GetModuleFileNameA(NULL, (LPSTR)(lpFilename), 256);

	string strPath(lpFilename);
	int nIndex = strPath.rfind('\\');
	if (nIndex != -1)
		strPath = strPath.substr(0, nIndex);
	if (strPath.empty() == FALSE && strPath.at(strPath.size() - 1) != '\\')
	{
		strPath += "\\";
	}
	return strPath;
}

wstring GetModulePathW()
{
	TCHAR lpFilename[256];
	GetModuleFileName(NULL, (LPTSTR)(lpFilename), 256);

	tstring strPath(lpFilename);
	int nIndex = strPath.rfind(_T('\\'));
	if (nIndex != -1)
		strPath = strPath.substr(0, nIndex);
	if (strPath.empty() == FALSE && strPath.at(strPath.size() - 1) != _T('\\'))
	{
		strPath += _T("\\");
	}
	return strPath;
}

tstring GetModuleName()
{
	TCHAR lpFilename[256];
	GetModuleFileName(NULL, (LPTSTR)(lpFilename), 256);

	tstring strPath(lpFilename);
	int nIndex = strPath.rfind(_T('\\'));
	if (nIndex != -1)
		strPath = strPath.substr(nIndex+1, strPath.length());
	return strPath;
}
