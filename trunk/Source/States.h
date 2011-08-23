/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: States.h,v $
Revision 1.1.10.3  2007/06/17 20:36:15  jay
POOL ONLINE: add FOUL state

Revision 1.1.10.2  2007/06/10 08:59:00  jay
Add two more States for Online Pool

Revision 1.1.10.1  2006/08/14 17:37:30  jay
Rename "Pool"

Revision 1.1  2004/09/08 15:42:57  jay
Added to repository
  
*/

#if !defined(STATES_H_INCLUDED)
#define STATES_H_INCLUDED

namespace Amju
{
// States which a game object can be in.
enum State
{
  UNKNOWN         = 0,  // initial, default state for all Game Objects
  //IDLE          = 1,  // use UNKNOWN, this is superfluous
  OUT_OF_PLAY     = 2,  // Object is taken, killed etc. 
  INVISIBLE       = 3,
  VISIBLE         = 4,
  TAKEN           = 5,  // for Takeables: player has picked it up.
  MOVING          = 6,  // for Platforms
  STATIONARY      = 7,  // for Platforms
  EGG_SHOW_NUM    = 8,  // for eggs
  DEAD            = 9,  // for NPCs
  FADING          = 10, // for NPCs
  CHASE           = 11, // for NPCs
  NEAR_SHED       = 12, // for Chicks
  ON_RAMP         = 13, // for Chicks
  BONUS_EXPLODE   = 14,
  FIGHT           = 15,
  HATCH           = 16, // for eggs: command to hatch, rather than a state
  CAPTURED        = 17, // for Chicks
  RUN_HOME        = 18, // for NPCs
  FROZEN          = 19, // for NPCs
  SET             = 20, // buttons(switches)
  UNSET           = 21, // buttons(switches)
  JUST_HATCHED    = 22, // for chicks
  DECIDE          = 23, // NPCs, decide where to go next
  DETONATE        = 24, // Bombs
  DETONATING      = 25, // Bombs
  RISE_FROM_DEAD  = 26, // for NPCs; reset orientation then go to UNKNOWN state.

  AI_COLLECT      = 27,
  AI_PICK_UP      = 28,
  AI_TAKE         = 29,
  AI_ROTATE       = 30,
  AI_DROP         = 31,
  AI_JUMP_DOWN    = 32,
  AI_DUMP         = 33,
  AI_ROTATE_FAIL  = 34,
  AI_JUMP_FAIL    = 35,
  AI_RUN_TO       = 36,

  GOLF_SWING_UP   = 37, // Pool: raise club
  GOLF_SWING_DOWN = 38, // Pool: swing club to strike ball
  GOLF_WATCH_BALL = 39, // Pool: change view to watch ball travel.

  BALL_IN_POCKET  = 40,
  BALL_OUT_OF_BOUNDS = 41,
  FOUL = 42

};
}
#endif

