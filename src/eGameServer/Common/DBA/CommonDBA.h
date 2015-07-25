
#ifndef COMMONDBA_H_
#define COMMONDBA_H_

#ifndef LINUX
#ifdef COMMON_EXPORTS
		#define COMMONDBA_API __declspec(dllexport)
	#else
		#define COMMONDBA_API __declspec(dllimport)
	#endif
#else
	#define COMMONDBA_API
#endif

#include "tstring.h"
#include <tchar.h>

/*
*note
*demoҪ���У�ֻ��Ҫʹ��Mysql���ݿ�
*Ϊ������ʾ�����Լ�����Sqlite���ݿ�Ĳ����ӿ�
*�������ͣ����ϲ��������˵�����κ����𣬿����޷��л�
*/

#ifndef DBA_TYPE_MYSQL
#define DBA_TYPE_MYSQL			1
#endif
#ifndef DBA_TYPE_SQLITE
#define DBA_TYPE_SQLITE			2
#endif

#define STRING_FIELD_BUF_LEN (512)

class CBaseDBA;
class COMMONDBA_API CComnDBAMgr
{
public:
	CComnDBAMgr();
	virtual ~CComnDBAMgr();
	
public:
	LRESULT OpenDBA(LPCTSTR strIP = _T("127.0.0.1")
		, LPCTSTR strDBName = _T("eGameServer")
		, LPCTSTR strUserName = _T("root")
		, LPCTSTR strPassword = _T("test")
		, UINT nDBAType = DBA_TYPE_MYSQL
		,LPTSTR pstrErrOut = NULL
		,UINT  unErrSizeInByte = 0);
	long CloseDBA();
	int SelectSQL(LPCTSTR SqlStr);
	LRESULT ExcuteSQL(LPCTSTR SqlStr);
	LRESULT MoveToNext();
	LRESULT MoveToFirst();
	
	void InitSelectTask();
	void FinishSelectTask();
	
	BOOL GetFieldAsBool(LPCTSTR szColumn);
	int GetFieldAsInt32(LPCTSTR szColumn);
	UINT64	GetFieldAsInt64(LPCTSTR szColumn);
	double GetFieldAsDouble(LPCTSTR szColumn);
	long GetFieldAsDateTime(LPCTSTR szColumn);
	LRESULT GetFieldAsString(LPCTSTR szColumn, LPTSTR pszOut, UINT unSizeInWords);
	LRESULT GetFieldAsString(LPCTSTR szColName, tstring &strOut);
	
	BOOL GetLastDBAError(LPTSTR strOutErrInfo, UINT unSizeInByte);
	int	GetLastDBAErrno();

private:
	int 				m_nDBAType;
	CBaseDBA		*m_pDBA;
};

#endif
