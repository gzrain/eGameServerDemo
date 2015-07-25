#ifndef SAFE_RELEASE_H_
#define SAFE_RELEASE_H_


#ifndef SAFE_FREE
#define SAFE_FREE(p)       { if(p) { free (p);     (p)=NULL; } }
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#endif

#ifndef SAFE_ARRAY_DELETE
#define SAFE_ARRAY_DELETE(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif

#ifndef HANDLE_IS_VALID
#define HANDLE_IS_VALID(h) (HANDLE(h) && HANDLE(h) != INVALID_HANDLE_VALUE)
#endif

#ifndef SAFE_CLOSE_HANDLE
#define SAFE_CLOSE_HANDLE( x )  \
	if (NULL != x)      \
{                   \
	CloseHandle(x);       \
	x = NULL;       \
	}
#endif

#ifndef SAFE_DELETE_OBJECT
#define SAFE_DELETE_OBJECT( h )  \
	if (NULL != h)      \
{                   \
	DeleteObject(h);       \
	h = NULL;       \
	}
#endif


#ifndef SAFE_CLOSE_SOCKET
#define SAFE_CLOSE_SOCKET(h)\
{\
if (h != INVALID_SOCKET)\
	{\
	closesocket(h); \
	h = INVALID_SOCKET; \
	}\
}
#endif



#endif
