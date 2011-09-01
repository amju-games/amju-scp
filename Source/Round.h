/*
Amju Games source code (c) Copyright Jason Colman 2000-2006
$Log: Round.h,v $
Revision 1.1.2.2  2006/08/28 22:33:18  jay
Round shot info in an attempt to remove inconsistencies between clients

*/

#ifndef ROUND_H_INCLUDED
#define ROUND_H_INCLUDED

#include "GameState.h"

namespace Amju
{
// Round float f to the given number of decimal places.
// This is used for sending Online data, where we want the server and all
// clients to get the same float values. 
// TODO something more robust, so many float->string conversions will not
//  have acumulative affect on accuracy.
void Round(float& f, unsigned int decimalPlaces);

// Round shot data members to appropriate numbers of decimal digits.
// This is done so all clients start a shot with the same values,
// whatever string/float/DB conversions have happened.
// Call this when the client takes a shot, and when this info is
// received from another client.
// Calling more than once should NOT affect the values.
void Round(PoolGameState::PlayerInfo* pInfo);

}

#endif

