/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: PoolChangeState.h,v $
Revision 1.1.2.2  2006/08/14 17:50:22  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:38  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.1  2005/09/23 19:58:42  jay
Functions to change state, taking Mode into account

*/

#ifndef POOL_CHANGE_STATE_H_INCLUDED
#define POOL_CHANGE_STATE_H_INCLUDED

namespace Amju
{
void ChangeStateToSetUpShot();
void ChangeStateToShowShot();
void ChangeStateToShotInPlay();
}

#endif

