/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Mouse.cpp,v $
Revision 1.1.10.2  2005/07/30 12:31:26  jay
Now holds state for Right- and middle-buttons

Revision 1.1.10.1  2005/07/05 09:45:47  jay
Initially set mouse coords to off-screen. This is so GUI elements start
off unselected.

Revision 1.1  2004/09/08 15:43:19  jay
Added to repository
  
*/

// This source code originally written by JASON COLMAN 2000-2003. 
// You may use it as you wish, at your own risk!  jason@amju.com.

#include <iostream>
#include "Mouse.h"
#ifdef MACOSX_BUNDLE
#include <Carbon/Carbon.h>
#endif
#ifdef WIN32
#include <windows.h>
// This is a bit nasty :-(
//#include "../WinMain/resource.h"
#endif
#include "SchAssert.h"

namespace Amju
{
int Mouse::s_mousex = -1;
int Mouse::s_mousey = -1;
bool Mouse::s_mouseDown = false;
bool Mouse::s_mouseRDown = false;
bool Mouse::s_mouseMidDown = false;

#ifdef WIN32
void Mouse::SetInstance(HINSTANCE hinst)
{
  s_hinst = hinst;
}

HINSTANCE Mouse::s_hinst;
#endif

void Mouse::SetCursor(Mouse::CursorType ct)
{
//
//
//   NB does nothing!
//
//

/*
#ifdef MACOSX_BUNDLE
  // TODO These IDs are allocated by ResEdit
  static CursHandle cursorHand = GetCursor(128);
  static CursHandle cursorFinger = GetCursor(129);
#endif 

#ifdef WIN32
  // TODO These IDs are just examples from MSDN
  Assert(s_hinst);
  static HCURSOR cursorHand = LoadCursor(s_hinst, MAKEINTRESOURCE(IDC_CURSOR1));
  static HCURSOR cursorFinger = LoadCursor(s_hinst, MAKEINTRESOURCE(IDC_CURSOR2));
  static HCURSOR cursorStandard = LoadCursor(0, IDC_ARROW);
#endif

  //std::cout << "Set cursor " << ct << "\n";

  static CursorType oldCt = STANDARD;

  //if (ct == oldCt)
  //{
  //  return;
  //}
 
  oldCt = ct;
  switch (ct)
  {
  case STANDARD:
#ifdef MACOSX_BUNDLE
    InitCursor();
#endif
#ifdef WIN32
    ::SetCursor(cursorStandard);
#endif
    break;

  case HAND:
#ifdef MACOSX_BUNDLE
    ::SetCursor(*cursorHand);
#endif
#ifdef WIN32
    ::SetCursor(cursorHand);
#endif
    break;

  case FINGER:
#ifdef MACOSX_BUNDLE
    ::SetCursor(*cursorFinger);
#endif
#ifdef WIN32
    ::SetCursor(cursorFinger);
#endif

    break;
  }
*/
}

}



