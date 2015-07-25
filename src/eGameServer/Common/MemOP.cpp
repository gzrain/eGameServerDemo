
#include "MemOP.h"
#include <windows.h>

int MemOP_Write(void *pMemBuf, void * pData, int nDataLen, int nMemOffSet)
{
	memcpy((BYTE *)pMemBuf + nMemOffSet, pData, nDataLen);
	return nMemOffSet + nDataLen;
}

int MemOP_Read(void * pMemBuf, void *pData, int nDataLen, int nMemOffSet)
{
	memcpy(pData, (BYTE *)pMemBuf + nMemOffSet, nDataLen);
	return nMemOffSet + nDataLen;
}
