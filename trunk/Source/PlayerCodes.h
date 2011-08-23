/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PlayerCodes.h,v $
Revision 1.1  2004/09/08 15:42:54  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_PLAYER_CODES_H_INCLUDED)
#define SCHMICKEN_PLAYER_CODES_H_INCLUDED

namespace Amju
{
// Totally hardcoded Message codes.
// Like States, but specifying an action for a Player.
enum PlayerCode
{
  PLAYER_CODES_START = 10000,
  PLAYER_JUMP,
  PLAYER_FWD_VEL,
  PLAYER_ROTATE_VEL
};
}

#endif

