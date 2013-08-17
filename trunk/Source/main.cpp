#ifdef WIN32
#if defined(_DEBUG)
#pragma comment(lib, "../../../../amjulib/Build/Debug/AmjuLibMsvc.lib")
#else
#pragma comment(lib, "../../../../amjulib/Build/Release/AmjuLibMsvc.lib")
#endif 
#endif // WIN32

#ifndef IPHONE

#define AMJU_USE_OPENGL

#ifdef MACOSX
#define AMJU_USE_GLUT
#endif

//#define AMJU_USE_DX11

// For debugging when faced with black screen of misery
#define GEKKO_CONSOLE

#include <main.h>

#endif // not IPHONE
