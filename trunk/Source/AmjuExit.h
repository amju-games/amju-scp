/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: AmjuExit.h,v $
Revision 1.1.2.2  2006/08/14 17:50:07  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:24  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.2  2006/05/09 18:16:56  jay
Mac OS X: Don't just exit, post a Quit event, so we can clean up full
screen, etc.

Revision 1.1.2.1  2005/09/23 19:56:35  jay
Add exit functions, which go to Nag Mode if unregistered.

*/

#ifndef AMJU_EXIT_H_INCLUDED
#define AMJU_EXIT_H_INCLUDED

void OnExitClicked();

void ReallyExit();

#ifdef MACOSX
void MacPostQuitEvent();
#endif

#endif

