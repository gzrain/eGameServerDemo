#ifndef _MODULEPATH_H_
#define _MODULEPATH_H_

#include "tstring.h"

#ifdef COMMON_EXPORTS
#define MODULEPATH_API __declspec(dllexport)
#else
#define MODULEPATH_API __declspec(dllimport)
#endif

#ifdef UNICODE
#define GetModulePath GetModulePathW
#else
#define GetModulePath GetModulePathA
#endif

//MODULEPATH_API tstring GetModulePath();
MODULEPATH_API string GetModulePathA();
MODULEPATH_API wstring GetModulePathW();
MODULEPATH_API tstring GetModuleName();


#endif