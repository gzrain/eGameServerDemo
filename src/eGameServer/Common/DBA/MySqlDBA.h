// MySqlDBA.h
// by gaozhe
// 20150702

#include "BaseDBA.h"
#include <WinSock2.h>
#include "./MySqlSDK/mysql.h"

using namespace std;
class CMySqlDBA:public CBaseDBA
{
public:
	BOOL OpenDBA(LPCTSTR strSerIP, LPCTSTR strDBName, LPCTSTR strUser,
		LPCTSTR strPwd, int nTimeOut, LPTSTR pstrErrOut, UINT unErrSizeInByte);
	void CloseDBA();
	int ExcuteSQL(LPCTSTR szSqlStr);
	int SelectSQL(LPCTSTR szSqlStr);
	void CloseRecordset();

	BOOL GotoFirst();
	BOOL GotoNext();
	BOOL GetFieldAsBool(LPCTSTR szColName);
	double GetFieldAsDateTime(LPCTSTR szColName);
	double GetFieldAsDouble(LPCTSTR szColName);
	int GetFieldAsInt32(LPCTSTR szColName);
	UINT64 GetFieldAsInt64(LPCTSTR szColName);
	tstring GetFieldAsString(LPCTSTR szColName);

	BOOL GetLastDBAError(LPTSTR strOutErrInfo, UINT unSizeInByte);
	int GetLastDBAErrno();

public:
	CMySqlDBA();
	~CMySqlDBA();

private:
	int GetColIdxAndTypeByName(LPCTSTR szColName, UINT &unFieldType);

private:
	MYSQL				*m_pMySQL;//MySQL的连接句柄  
	MYSQL_RES			*m_pResRet;//返回的结果集  
	my_ulonglong		m_llRecordCount;//共有几条数据  
	UINT				m_nFieldCount;
	MYSQL_ROW			m_ppCurRow;
};
