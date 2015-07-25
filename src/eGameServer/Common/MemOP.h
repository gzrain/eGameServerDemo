#ifndef _MEMOP_H_
#define _MEMOP_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef COMMON_EXPORTS

#define MEMOP_API __declspec(dllexport)
#else
#define MEMOP_API __declspec(dllimport)
#endif

MEMOP_API int MemOP_Write(void *pMemBuf, void * pData, int nDataLen, int nMemOffSet);
MEMOP_API int MemOP_Read(void * pMemBuf, void *pData, int nDataLen, int nMemOffSet);


#ifdef __cplusplus
		}
#endif

#endif