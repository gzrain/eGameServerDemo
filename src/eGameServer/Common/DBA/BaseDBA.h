
#ifndef BASEDBA_H_
#define BASEDBA_H_

#include "tstring.h"

#ifndef LINUX
	#ifdef COMMON_EXPORTS
		#define BASEDBA_API __declspec(dllexport)
	#else
		#define BASEDBA_API __declspec(dllimport)
	#endif
#else
	#define BASEDBA_API
#endif

#define MAX_ERROR_MESSAGE_CHARS	(512)

class BASEDBA_API CBaseDBA
{

public:
	virtual BOOL OpenDBA(LPCTSTR strSerIP, LPCTSTR strDBName,
					LPCTSTR strUser,LPCTSTR strPwd,
					int nTimeOut, LPTSTR pstrErrOut, UINT unErrSizeInByte) = 0;
	virtual void CloseDBA() = 0;
	virtual int ExcuteSQL(LPCTSTR szSqlStr) = 0;
	virtual int SelectSQL(LPCTSTR szSqlStr) = 0;
	virtual void CloseRecordset() = 0;
	
	virtual BOOL GotoFirst() = 0;
	virtual BOOL GotoNext() = 0;
	virtual BOOL GetFieldAsBool(LPCTSTR szColName) = 0;
	virtual double GetFieldAsDateTime(LPCTSTR szColName) = 0;
	virtual double GetFieldAsDouble(LPCTSTR szColName) = 0;
	virtual int GetFieldAsInt32(LPCTSTR szColName) = 0;
	virtual UINT64 GetFieldAsInt64(LPCTSTR szColName) = 0;
	virtual tstring GetFieldAsString(LPCTSTR szColName) = 0;
	
	virtual BOOL	GetLastDBAError(LPTSTR strOutErrInfo, UINT unSizeInByte) = 0;
	virtual int GetLastDBAErrno() = 0; 

public:
	CBaseDBA();
	virtual ~CBaseDBA();
};

#endif