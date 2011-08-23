/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: RulesNBall.cpp,v $
Revision 1.1.2.7  2007/06/21 18:13:57  jay
Turn off debug out

Revision 1.1.2.6  2007/06/10 21:33:27  Administrator
Check for extra states BALL_IN_POCKET and BALL_OUT_OF_BOUNDS

Revision 1.1.2.5  2007/06/10 08:58:04  jay
BallInPocket() is not told which pocket the ball hit. This info is lost
when a State change is used to flag ball/pocket collisions.

Revision 1.1.2.4  2007/05/23 18:45:43  jay
Send all object changes in one Http request (POST), not individually.
 + Some tidying up.

Revision 1.1.2.3  2006/08/30 21:12:21  Administrator
Get Pool to build in MSVC

Revision 1.1.2.2  2006/08/14 17:50:25  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:40  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.13  2006/04/11 17:07:05  jay
Check for bad break

Revision 1.1.2.12  2005/10/01 22:05:26  jay
Show Rules name at game start

Revision 1.1.2.11  2005/09/05 20:08:32  jay
Turn off debug output

Revision 1.1.2.10  2005/09/04 21:09:58  jay
Comment added

Revision 1.1.2.9  2005/09/04 20:55:08  Administrator
Hopefully fix bug when N-ball potted too soon

Revision 1.1.2.8  2005/09/01 19:52:23  jay
Fix where potting the N ball with balls remaining always lost

Revision 1.1.2.7  2005/08/26 21:59:17  jay
Fix foul on final ball test

Revision 1.1.2.6  2005/08/02 18:28:55  jay
Don't pass self to PoolBehaviour object; this doesn't work with Undo.

Revision 1.1.2.5  2005/07/30 12:27:25  jay
Added virtual Clone() so undo stack can copy the current Rules object.

Revision 1.1.2.4  2005/07/05 09:40:17  jay
Added help text filename

Revision 1.1.2.3  2005/06/29 20:24:33  jay
Ongoing fixes to rules.

Revision 1.1.2.2  2005/06/22 21:44:42  jay
Ongoing fixes to N-ball rules

Revision 1.1.2.1  2005/06/19 21:37:40  jay
Move Rules variants to separate files

*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "RulesNBall.h"
#include "PoolBall.h"
#include "PoolHole.h"
#include "Engine.h"
#include "EngineStatePoolSetUpShot.h"
#include "EngineStatePoolSetUpShotDemo.h"
#include "PoolMisc.h"
#include "LevelServer.h"
#include "EngineStateTitle.h"
#include "GameState.h"
#include "StringUtils.h"
#include "PoolPlayerStats.h"

//#define RULES_DEBUG

namespace Amju
{
GameObject* CreateRulesNBall()
{
  return new RulesNBall;
}

RulesNBall::RulesNBall()
{
  // Don't create behaviour yet - we don't know how many balls there are.
  // Also we can't get the help file yet until we know how many balls 
  // there are.
  m_numBalls = 0;
}

Rules* RulesNBall::Clone()
{
  return new RulesNBall(*this);
}

const char* RulesNBall::GetTypeName() const
{
  return "rules-n-ball";
}

bool RulesNBall::Load(File* pf)
{
  // Load no of balls
  if (!pf->GetInteger(&m_numBalls))
  {
    pf->ReportError("Expected number of balls for rules.");
    return false;
  }

  // Now we know the no of balls we can create the AI behaviour.
  m_pBehaviour = new PoolBehaviourNBall(m_numBalls);

  m_helpfilename = "help-";
  m_helpfilename += ToString(m_numBalls);
  m_helpfilename += "-balls.txt";

  if (!LoadHelpText())
  {
    return false;
  }

  return true;
}
void RulesNBall::CueBallHitsBall(PoolBall* pBall)
{
  if (m_firstBallId == -1)
  {
    // First ball to be hit must have lowest number.
    int lowest = GetLowestBall(m_pLevel)->GetNumber();
    if (pBall->GetNumber() > lowest)
    {
      ThisPlayerFouls();
      m_text = "Foul! ";
      m_text += GetName();
      m_text += " didn't hit the lowest ball (";
      m_text += ToString(lowest);
      m_text += ") first (hit ";
      m_text += ToString(pBall->GetNumber());
      m_text += ")";
    }
  }
  Rules::CueBallHitsBall(pBall);
}

bool RulesNBall::PotEndsGame()
{
  // Check if the highest numbered ball just got potted.
  // If so the current player has won.
#ifdef RULES_DEBUG
std::cout << "Rules n-ball: get highest ball: " << m_numBalls << "\n";
#endif
  PoolBall* pHighBall = GetNumberBall(m_pLevel, m_numBalls);
  Assert(pHighBall && "Can't find highest ball ?");
  if (!InPlay(pHighBall))
  {
    if (!IsShotFoul())
    {
      // High ball potted - player has won.
      ThisPlayerWins();
      return true;
    }
    else
    {
      // High ball potted on foul shot - player loses
      ThisPlayerLoses();
      return true;
    }
  }
  return false; // i.e. game is not over
}

#if defined(SCENE_EDITOR)
bool RulesNBall::Save(File* pf)
{
  if (!pf->WriteComment("// Num balls for rules"))
  {
    return false;
  }
  pf->WriteInteger(m_numBalls);
  return true;
}

static const char* NUM_BALLS = "Num balls";
GameObject::PropertyMap RulesNBall::GetProperties() const
{
  PropertyMap pm;
  pm[NUM_BALLS] = m_numBalls;
  return pm;
}

void RulesNBall::SetProperties(const PropertyMap& pm)
{
  m_numBalls = const_cast<GameObject::PropertyMap&>(pm)[NUM_BALLS].GetInt();
}
#endif

// New overrides

void RulesNBall::OnRoomEntry()
{
  Rules::OnRoomEntry();

  m_text = ToString(m_numBalls);
  m_text += " ball pool";
}

void RulesNBall::Reset()
{
  Rules::Reset();
}

void RulesNBall::BallInPocket(PoolBall* pBall)
{
  Rules::BallInPocket(pBall);

  // If this is the highest numbered ball, and there are other balls still
  // on the table, this loses the game.
  // NB IF this is a foul shot - if shot is ok, then this WINS the game!

  if (pBall->GetNumber() == m_numBalls)
  {
    int n = GetNumBallsRemainingForPlayer();
#ifdef RULES_DEBUG
std::cout << "POTTED HIGHEST NUMBERED BALL num balls left: " << n << "\n";
#endif

    // Lose if colours remaining AND shot is a foul
    if (n > 0 && IsShotFoul())
    {
      m_bigText = "lose!"; // ?
      m_text = GetName();
      m_text += " loses by potting the ";
      m_text += ToString(m_numBalls);
      m_text += " ball on a foul.";
      ThisPlayerFouls(); // this will cause player to lose when balls have stopped.
    }
    else
    {
        // Can't do WIN test here - player may foul AFTER potting high ball.
    }
  }
}

void RulesNBall::BallHitsRail(PoolBall* pBall)
{
  Rules::BallHitsRail(pBall);
}

void RulesNBall::BallsHaveStopped()
{
  Rules::BallsHaveStopped();
}

int RulesNBall::GetNumBallsRemainingForPlayer()
{
  return Rules::GetNumBallsRemainingForPlayer();
}

void RulesNBall::GivePlayerFoulAward()
{
  // Check if no ball has been hit yet - if this is the case the cue ball
  // must go back behind the headstring.
  if (m_shotIsBreak && m_firstBallId == -1)
  {
#ifdef RULES_DEBUG
std::cout << "** No ball hit yet! Cue ball must go behind headstring.\n";
#endif
    m_placeBall = false;
    m_placeBallHeadstring = true;
    // Force cue ball back to tee spot
    m_resetCueBall = true;
    return;
  }

  // No extra shot; player can place ball anywhere (not just behind headstring)
  m_placeBall = true;

#ifdef RULES_DEBUG
std::cout << "RULES N BALL: Player can place cue ball ANYWHERE!\n";
#endif
}

}

