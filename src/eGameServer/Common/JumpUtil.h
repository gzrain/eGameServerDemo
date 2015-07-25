#ifndef JUMP_UTIL_H_
#define JUMP_UTIL_H_

#define FAIL_RET(x) do { if( FAILED( hr = ( x  ) ) ) \
	return hr; } while(0)

#ifndef IF_FAILED_GOTO
#define IF_FAILED_GOTO(hr, label) if (FAILED(hr)) { goto label; }
#endif

#ifndef IF_NOT_SOK_GOTO
#define IF_NOT_SOK_GOTO(hr, label) if (S_OK != (hr)) { goto label; }
#endif

#ifndef IF_FALSE_GOTO
#define IF_FALSE_GOTO(hr, label) if (!(hr)) { goto label; }
#endif

#ifndef IF_NULL_POINTER_GOTO
#define IF_NULL_POINTER_GOTO(pt, label) if (NULL == (pt)) { goto label; }
#endif

#define CHECK_HR(hr) IF_FAILED_GOTO(hr, done)
#define CHECK_SOK(hr) IF_NOT_SOK_GOTO(hr, done)
#define CHECK_BOOL(hr) IF_FALSE_GOTO(hr, done)
#define CHECK_POINTER(pt) IF_NULL_POINTER_GOTO(pt, done)

#ifndef BREAK_IF_FAILED
#define BREAK_IF_FAILED(hr) if(FAILED(hr)) break;
#endif

#ifndef SLEEP_RETURN
#define SLEEP_RETURN(x)\
{\
	Sleep(x); \
	return; \
}
#endif

#ifndef SLEEP_BREAK
#define SLEEP_BREAK(x)\
{\
	Sleep(x); \
	break; \
}
#endif

#ifndef SLEEP_CONTINUE
#define SLEEP_CONTINUE(x)\
{\
	Sleep(x); \
	continue; \
	}
#endif

#ifndef SAFE_FREE_RES_BREAK
#define SAFE_FREE_RES_BREAK(p)\
{\
	if (p != NULL)\
	free(p); \
	p = NULL; \
	break; \
}
#endif


#endif
