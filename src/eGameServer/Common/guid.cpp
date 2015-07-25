
#include "guid.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <algorithm>


int CrossPlatform_CreateGuid(COS_GUID* pGuid)
{
	assert(pGuid);
	int nRet = 0;

	if(CoCreateGuid((GUID *)pGuid) != S_OK)
		nRet = - 1;
	return nRet;
}

BOOL CrossPlatform_ValidGuid(const COS_GUID* pGuid)
{
	assert(pGuid);
	BOOL bRet = TRUE;
	
	//COS_GUID guidInvalid = GUID_NULL;
	//if(memcmp(&guidInvalid,pGuid,sizeof(COS_GUID)) == 0)
	if (GUID_NULL == *pGuid)
		bRet = FALSE;
	return bRet;
}

void CrossPlatform_InValidGuid(COS_GUID* pGuid)
{
	assert(pGuid);

	*pGuid = GUID_NULL;
	//memset(pGuid,0,sizeof(COS_GUID));
}

int CrossPlatform_CopyGuid(COS_GUID* pGuidDes,const COS_GUID* pGuidSrc)
{
	assert(pGuidDes);
	assert(pGuidSrc);

	*pGuidDes = *pGuidSrc;
	//memcpy(pGuidDes,pGuidSrc,sizeof(COS_GUID));
	return 0;
}

int CrossPlatform_CompareGuid(const COS_GUID &GuidDes,const COS_GUID &GuidSrc)
{
	return memcmp(&GuidDes,&GuidSrc,sizeof(COS_GUID));
}

int CrossPlatform_GUID2StringA(const COS_GUID &guid, CHAR *pszGuidStr, int nStrLen)
{
	assert(pszGuidStr);
	int nRet = 0;
	size_t nRetValue = 0;
	int nMaxSizeInWords = min(MIN_GUID_STR_LEN, nStrLen);
	TCHAR strGuid[MIN_GUID_STR_LEN] = { 0 };

	nRet = CrossPlatform_GUID2StringW(guid, strGuid, MIN_GUID_STR_LEN);
	if (0 < nRet)
		wcstombs_s(&nRetValue, pszGuidStr, nMaxSizeInWords, strGuid, nMaxSizeInWords);
	return nRet;
}

int CrossPlatform_GUID2StringW(const COS_GUID &guid, TCHAR *pszGuidStr, int nStrLen)
{
	assert(pszGuidStr);
	int nRet = 0;
	int nMaxSizeInWords = min(MIN_GUID_STR_LEN, nStrLen);

	nRet = ::StringFromGUID2(guid, pszGuidStr, nMaxSizeInWords);
	if (0 >= nRet)
		nRet = -1;
	return nRet;
}

int CrossPlatform_StringToGuid(LPCTSTR lpszGuid,COS_GUID *pGuid)
{
	assert(pGuid);
	int nRet = 0;
	LPCTSTR lpstr = lpszGuid;
	TCHAR strGuid[MIN_GUID_STR_LEN] = { 0 };
	if (_T('{') != lpszGuid[0])
	{
		_stprintf_s(strGuid, MIN_GUID_STR_LEN, _T("{%s}"), lpszGuid);
		lpstr = (LPCTSTR)strGuid;
	}

	*pGuid = GUID_NULL;
	if (FAILED(CLSIDFromString(lpstr, pGuid)))
		nRet = -1;
	return nRet;
}

int CrossPlatform_GUID2String_WithoutBracesA(const COS_GUID &guid, CHAR *pszGuidStr, int nStrLen)
{
	assert(pszGuidStr);
	int nRet = 0;
	size_t nRetValue = 0;
	int nMaxSizeInWords = min(MIN_GUID_STR_LEN, nStrLen);
	TCHAR strGuid[MIN_GUID_STR_LEN] = { 0 };

	nRet = CrossPlatform_GUID2String_WithoutBracesW(guid, strGuid, MIN_GUID_STR_LEN);
	if (0 < nRet)
		wcstombs_s(&nRetValue, pszGuidStr, nMaxSizeInWords, strGuid, nMaxSizeInWords);
	return nRet;
}

int CrossPlatform_GUID2String_WithoutBracesW(const COS_GUID &guid, TCHAR *pszGuidStr, int nStrLen)
{
	assert(pszGuidStr);
	int nRet = 0;
	int nCopyLen = 0;
	int nMaxSizeInWords = min(MIN_GUID_STR_LEN, nStrLen);
	TCHAR strGuid[MIN_GUID_STR_LEN] = { 0 };

	nRet = ::StringFromGUID2(guid, strGuid, MIN_GUID_STR_LEN);
	if (0 < nRet)
	{
		//È¥µô´óÀ¨ºÅ
		strGuid[_tcslen(strGuid) - 1] = 0;
		_tcscpy_s(pszGuidStr, nStrLen, (TCHAR *)(strGuid + 1));
	}
	else
		nRet = -1;
	return nRet;
}

void CrossPlatform_FormatGuidString(TCHAR *pszGuidStr, int nStrLen)
{
	assert(pszGuidStr);
	if (_T('{') != pszGuidStr[0])
	{
		TCHAR strGuid[MIN_GUID_STR_LEN] = { 0 };
		_tcscpy_s(strGuid, MIN_GUID_STR_LEN, pszGuidStr);
		_stprintf_s(pszGuidStr, nStrLen, _T("{%s}"), strGuid);
	}
}

void CrossPlatform_FormatGuidStringA(string &strGuid)
{
	if ('{' != strGuid.at(0))
	{
		string strTemp(strGuid);
		strGuid = "{" + strTemp + "}";
	}
	transform(strGuid.begin(), strGuid.end(), strGuid.begin(), ::toupper);
}

void CrossPlatform_FormatGuidStringW(wstring &strGuid)
{
	if (!strGuid.empty() && '{' != strGuid.at(0))
	{
		wstring strTemp(strGuid);
		strGuid = _T("{") + strTemp + _T("}");
	}
	transform(strGuid.begin(), strGuid.end(), strGuid.begin(), ::toupper);
}
