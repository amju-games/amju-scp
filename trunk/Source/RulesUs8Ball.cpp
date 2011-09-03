/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: RulesUs8Ball.cpp,v $
Revision 1.1.2.8  2007/06/24 11:42:23  jay
Turn off debug out for release build

Revision 1.1.2.7  2007/06/10 21:33:28  Administrator
Check for extra states BALL_IN_POCKET and BALL_OUT_OF_BOUNDS

Revision 1.1.2.6  2007/06/10 08:58:04  jay
BallInPocket() is not told which pocket the ball hit. This info is lost
when a State change is used to flag ball/pocket collisions.

Revision 1.1.2.5  2007/05/23 18:45:43  jay
Send all object changes in one Http request (POST), not individually.
 + Some tidying up.

Revision 1.1.2.4  2006/08/30 21:12:22  Administrator
Get Pool to build in MSVC

Revision 1.1.2.3  2006/08/17 18:15:42  jay
Ongoing work for Online Pool

Revision 1.1.2.2  2006/08/14 17:50:25  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:40  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.13  2006/04/11 17:07:50  jay
Check for bad break

Revision 1.1.2.12  2005/10/28 12:49:40  jay
Fix US 8 ball rules: when cue ball is potted, it can only be placed
behind the headstring.

Revision 1.1.2.11  2005/10/01 22:05:26  jay
Show Rules name at game start

Revision 1.1.2.10  2005/09/29 19:14:30  jay
ValidBalls must use IsPoolBall so we don't count subclassed objects

Revision 1.1.2.9  2005/09/05 20:08:50  jay
Turn off debug output

Revision 1.1.2.8  2005/09/01 20:55:07  jay
Hopefully fixed bug where 8 ball is legally pocketed before the last colour.
This should now be recognised as a foul and lose the game.

Revision 1.1.2.7  2005/08/26 22:00:14  jay
Fix pot 8 ball then final colour

Revision 1.1.2.6  2005/08/02 18:28:55  jay
Don't pass self to PoolBehaviour object; this doesn't work with Undo.

Revision 1.1.2.5  2005/07/30 12:27:25  jay
Added virtual Clone() so undo stack can copy the current Rules object.

Revision 1.1.2.4  2005/07/05 09:40:59  jay
Added helptext filename

Revision 1.1.2.3  2005/06/29 20:25:24  jay
Ongoing fixes to rules - removed one of two very similar functions.

Revision 1.1.2.2  2005/06/22 21:45:38  jay
Implemented Us 8 Ball rules

Revision 1.1.2.1  2005/06/19 21:37:42  jay
Move Rules variants to separate files

*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "RulesUs8Ball.h"
#include "PoolBall.h"
#include "PoolHole.h"
#include "Engine.h"
#include "EngineStatePoolSetUpShot.h"
#include "EngineStatePoolSetUpShotDemo.h"
//#include "EngineStatePoolBallInHole.h"
#include "PoolMisc.h"
#include "LevelServer.h"
#include "EngineStateTitle.h"
#include "PoolGameState.h"
#include "StringUtils.h"
#include "PoolPlayerStats.h"

// TODO TEMP TEST
#ifdef _DEBUG
#define RULES_DEBUG  
#endif

namespace Amju
{
PoolGameObject* CreateRulesUs8Ball()
{
  return new RulesUs8Ball;
}

RulesUs8Ball::RulesUs8Ball()
{
  m_pBehaviour = new PoolBehaviourUs8Ball;
  m_playerColour[0] = SPOTS; // i.e. any colour is ok
  m_playerColour[1] = SPOTS; // i.e. any colour is ok
  m_coloursDecided = false;

  m_helpfilename = "help-us-8-ball.txt";
}

Rules* RulesUs8Ball::Clone()
{
  return new RulesUs8Ball(*this);
}

const char* RulesUs8Ball::GetTypeName() const
{
  return "rules-us-8-ball";
}

bool RulesUs8Ball::ColoursDecided() const
{
  return m_coloursDecided;
}

RulesUs8Ball::SpotsOrStripes RulesUs8Ball::GetPlayerColour(int player) const
{
  Assert(player == 0 || player == 1);
  Assert(m_coloursDecided);
  return m_playerColour[player];
}

void RulesUs8Ball::CueBallHitsBall(PoolBall* pBall)
{
  m_shotIsBreak = false;

  // Set foul if player hits wrong colour
  if (m_firstBallId == -1)
  {
    // First ball to be hit by cue ball. Check the colour.
    m_firstBallId = pBall->GetNumber();

    int id = Engine::Instance()->GetGameState()->GetCurrentPlayer();
    
    if (ColoursDecided())
    {
      // Foul if player hit wrong type; or if player hit the 8 ball
      // with other balls still on the table.
      if (pBall->GetNumber() == 8)
      {
        // Test if other valid balls are on the table.
        int numRem = GetNumBallsRemainingForPlayer(); 
#ifdef RULES_DEBUG
std::cout << "US 8 BALL: Black hit first, num balls left for player: " << numRem << "\n";
#endif
        if (numRem > 1)
        {
          m_text = "Foul! ";
          m_text += GetName();
          m_text += " hit the 8 ball first.";
          ThisPlayerFouls();
        }
      }
      else
      {
        // Ball colour same as player colour ? Foul if different.
        SpotsOrStripes playerColour = m_playerColour[id];
        SpotsOrStripes sos = (pBall->GetNumber() < 8) ? SPOTS : STRIPES;
        if (sos != playerColour)
        {
          // Hit wrong colour ball first
          m_text = "Foul! ";
          m_text += GetName();
          m_text += " hit a ";
          m_text += ((sos == SPOTS) ? "spot" : "stripe");
          m_text += " first.";
          ThisPlayerFouls();
        } 
      }
    }
    else
    {
      // Foul if player hit the 8 ball first.
      if (pBall->GetNumber() == 8)
      {
        m_text = "Foul! ";
        m_text += GetName();
        m_text += " hit the 8 ball first.";
        ThisPlayerFouls();
      }
    }
  }
}

int RulesUs8Ball::GetNumBallsRemainingForPlayer()
{
  if (!m_coloursDecided)
  {
    // Some balls may have been potted without deciding colours, but
    // it doesn't matter so long as there is more than one ball left!
    // TODO Is it possible to pot all balls and not decide ?!
    return 8;
  }
  
  Level* pLevel = LevelServer::Instance()->GetCurrentLevel().GetPtr();
  Assert(pLevel);
  
  // Check for balls which have the number this player is potting.
  int id = Engine::Instance()->GetGameState()->GetCurrentPlayer();
  SpotsOrStripes playerColour = GetPlayerColour(id);
  // Also check for black ball
  int blackNum = 8; // TODO CONFIG
  
  int count = 0;
  // Get the game objects which are in the same room as the player.
  int levelId = pLevel->GetId();
  int roomId = pLevel->GetRoomId();
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  { 
    PPoolGameObject pGo = it->second;
    if (IsPoolBall(pGo.GetPtr()))
    {
      PoolBall* p = dynamic_cast<PoolBall*>(pGo.GetPtr());
      Assert(p);
      if (InPlay(p) && !IsCueBall(p))
      {
        // Spot or stripe for this ball
        SpotsOrStripes sos = (p->GetNumber() < 8) ? SPOTS : STRIPES;

        // Count this ball if it's the right colour or it's the 8 ball.
        if (sos == playerColour || p->GetNumber() == blackNum) 
        {
          ++count;
        }
      }
    }
  }

#ifdef RULES_DEBUG
std::cout << "** US 8 BALL: no of balls left for player: " << count << "\n";
#endif
  
  return count;
}

bool RulesUs8Ball::PotEndsGame()
{
  // ** HACK ** decide colours here
  // TODO put this somewhere nicer
  if (!m_coloursDecided && GotValidPot())
  { 
    bool canDecide = true;
    Assert(GotValidPot());
    Assert(!m_ballsPottedThisShot.empty());
    int colour0 = m_ballsPottedThisShot[0]->GetNumber();
    if (colour0 == 8)
    {
      ThisPlayerLoses();
      return true;
    }
    SpotsOrStripes sos0 = (colour0 < 8) ? SPOTS : STRIPES;

    // If one ball potted, or all balls the same colour, fix the player colours.   
    for (unsigned int i = 1; i < m_ballsPottedThisShot.size(); i++)
    {
      int colour = m_ballsPottedThisShot[i]->GetNumber();
      if (colour == 8)
      {
        ThisPlayerLoses();
        return true;
      }
      SpotsOrStripes sos = (colour < 8) ? SPOTS : STRIPES;
      if (sos0 != sos)
      {
        // Balls of different colours potted - can't decide yet.
        canDecide = false;
      }
    }

    if (canDecide)
    {
      int id = Engine::Instance()->GetGameState()->GetCurrentPlayer();
      m_coloursDecided = true;
      m_playerColour[id] = sos0;
      Assert(id == 0 || id == 1);
      int other = 1 - id;
      SpotsOrStripes otherColour = SPOTS;
      if (sos0 == SPOTS)
      {
        otherColour = STRIPES;
      }
      m_playerColour[other] = otherColour;

#ifdef RULES_DEBUG
std::cout << "UK RULES: Decided colours: player 0: " << m_playerColour[0] << " player 1: " << m_playerColour[1] << "\n";
#endif

      // Check that player colours are now set correctly - it seems
      // that both players are set to the same colour.
      Assert(m_playerColour[0] != m_playerColour[1]);
    }
    else
    {
#ifdef RULES_DEBUG
std::cout << "UK RULES: Can't decide player colours yet.\n";
#endif
    }
  }
  // End of deciding colours code

  // TODO check for 8 ball - if potted and colours remain for the player,
  // it's game over - otherwise this player has won.
  // Is 8-ball potted ?
  PoolBall* p8Ball = GetNumberBall(m_pLevel, 8); 
  if (!InPlay(p8Ball))
  { 
    // Out of play => black has been Potted
//    int id = Engine::Instance()->GetEngineState()->GetCurrentPlayer();
    int numRem = GetNumBallsRemainingForPlayer();
#ifdef RULES_DEBUG
std::cout << "US 8 BALL: Black potted, num balls remaining: " << numRem << "\n";
#endif
    if (numRem > 0 || IsShotFoul())
    {
      // 8-ball potted with colours remaining, this player loses
#ifdef RULES_DEBUG
if (numRem > 1)
{
  std::cout << "US 8 BALL: 8 ball potted before all colours, lose game.\n";
}
else if (IsShotFoul())
{
  std::cout << "US 8 BALL: 8 ball potted but shot is foul, lose game.\n";
}
#endif
      m_text = GetName();
      m_text += " loses by potting the 8 ball.";
      ThisPlayerLoses();
      return true; // game ends
    }
    else
    {
      ThisPlayerWins();
      return true; // game ends
    }
  }
  
  return false;
}

void RulesUs8Ball::BallInPocket(PoolBall* pBall)
{ 
  // Check if the 8 ball goes down before all colours.
  // What can happen is that the 8 ball goes down, _then_ the last colour - 
  // that loses the game, but won't be detected once the balls have both
  // been potted.

#ifdef RULES_DEBUG
std::cout << "RULES: Ball in pocket: " << pBall->GetId() << "\n";
#endif

  // Try to stop ball bouncing out of pocket.
  pBall->SetFalling(false);
  pBall->SetForwardVel(0.1f);
  pBall->SetForwardAccel(-0.1f);
  pBall->SetFalling(true);
 
  // Is the ball the cue ball ? If so this is a foul - and may lose the game.
  if (IsCueBall(pBall))
  {
#ifdef RULES_DEBUG
std::cout << "RULES: FOUL! - cue ball in pocket!\n";
#endif

    m_text = "Foul! ";
    m_text += GetName();
    m_text += " potted the cue ball.";

    ThisPlayerFouls();
    m_resetCueBall = true;

    SetBallOutOfPlay(pBall);
  }
  else
  {
    SetBallOutOfPlay(pBall);
    ThisPlayerGetsBall(pBall);

    int numRem = GetNumBallsRemainingForPlayer();
    if (pBall->GetNumber() == 8 && numRem > 0)
    {
      ThisPlayerFouls();
    }
  }
}

void RulesUs8Ball::OnRoomEntry()
{
  // New frame
  m_playerColour[0] = SPOTS; 
  m_playerColour[1] = SPOTS;
  m_coloursDecided = false;

  Rules::OnRoomEntry();

  m_text = "US 8-ball pool";
}

void RulesUs8Ball::Reset()
{
  // New shot
  Rules::Reset();
}

void RulesUs8Ball::BallHitsRail(PoolBall* pBall)
{
  Rules::BallHitsRail(pBall);
}

void RulesUs8Ball::BallsHaveStopped()
{
  Rules::BallsHaveStopped();
}
  
void RulesUs8Ball::GivePlayerFoulAward()
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

  if (!InPlay(GetBall()))
  {
    // PLACE_BALL
    // NB Customer complaint was that in US 8 Ball, if the cue ball is potted
    // it can only be placed behind the headstring.
    m_placeBall = false;
    m_placeBallHeadstring = true;

#ifdef RULES_DEBUG
std::cout << "**RULES FIX: only behind headstring\n";
#endif
  }
  else
  {
    m_placeBall = true;
#ifdef RULES_DEBUG
std::cout << "**RULES FIX: can place ball anywhere!\n";
#endif
  }
}
}

