
#include "BaseDBA.h"
#include "./SqliteSDK/sqlite3.h"

#include <vector>
using namespace std;

typedef	vector<char *> GB2312STRLIST;

class CSqliteDBA:public CBaseDBA
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
	CSqliteDBA();
	virtual ~CSqliteDBA();
	
private:
	int GetColIdxByColName(LPCTSTR szColName);

private:
	sqlite3 			*m_pSqliteDB;

	int				m_nRowCnt;//the row count of the result set
	int				m_nColCnt;//the column count of the result set
	int 				m_nCurResultPos;//current result position in result set
	char 				**m_ppszResultSet;//the result set of query
	int				m_nTimeOut;
};
