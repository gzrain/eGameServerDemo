#ifndef TSTRING_H_
#define TSTRING_H_


#include <string>
#include <sstream>
using namespace std;
#ifdef _UNICODE
typedef wstring tstring;
typedef wostringstream tostringstream;
#else
typedef string tstring;
typedef ostringstream tostringstream;

#endif



#endif
