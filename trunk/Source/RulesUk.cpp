/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: RulesUk.cpp,v $
Revision 1.1.2.8  2007/06/21 18:13:04  jay
Fix stupid logic bug

Revision 1.1.2.7  2007/06/10 21:33:28  Administrator
Check for extra states BALL_IN_POCKET and BALL_OUT_OF_BOUNDS

Revision 1.1.2.6  2007/06/10 08:58:04  jay
BallInPocket() is not told which pocket the ball hit. This info is lost
when a State change is used to flag ball/pocket collisions.

Revision 1.1.2.5  2007/05/23 18:45:43  jay
Send all object changes in one Http request (POST), not individually.
 + Some tidying up.

Revision 1.1.2.4  2007/03/25 19:20:54  jay
Hopefully fix bug where after a foul cue ball was not reset to spot on headstring

Revision 1.1.2.3  2006/08/30 21:12:22  Administrator
Get Pool to build in MSVC

Revision 1.1.2.2  2006/08/14 17:50:25  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:40  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.18  2006/04/11 17:07:24  jay
Check for bad break

Revision 1.1.2.17  2005/10/01 22:05:26  jay
Show Rules name at game start

Revision 1.1.2.16  2005/09/29 19:14:30  jay
ValidBalls must use IsPoolBall so we don't count subclassed objects

Revision 1.1.2.15  2005/09/06 20:02:09  jay
Add virtual DecShotCount() and override for UK rules, so we never get
two shots on the black.

Revision 1.1.2.14  2005/09/01 20:55:07  jay
Hopefully fixed bug where 8 ball is legally pocketed before the last colour.
This should now be recognised as a foul and lose the game.

Revision 1.1.2.13  2005/08/26 22:00:02  jay
Fix pot 8 ball then final colour

Revision 1.1.2.12  2005/08/07 08:13:18  Administrator
Bug fix: you could pot the black with one colour left and win.

Revision 1.1.2.11  2005/08/06 20:34:05  Administrator
Try to give best explanation text if player loses

Revision 1.1.2.10  2005/08/06 17:56:41  Administrator
Allow AI to take free shots

Revision 1.1.2.9  2005/08/06 10:39:21  Administrator
Added Free Shot Counter

Revision 1.1.2.8  2005/08/06 09:08:39  jay
Check that potting opposite colour ball is only legal on Free shot.

Revision 1.1.2.7  2005/08/04 19:39:55  jay
Fix Rules bugs for v.1.1

Revision 1.1.2.6  2005/08/02 18:28:55  jay
Don't pass self to PoolBehaviour object; this doesn't work with Undo.

Revision 1.1.2.5  2005/07/30 12:27:25  jay
Added virtual Clone() so undo stack can copy the current Rules object.

Revision 1.1.2.4  2005/07/05 09:40:43  jay
Added help text filename

Revision 1.1.2.3  2005/06/29 20:25:24  jay
Ongoing fixes to rules - removed one of two very similar functions.

Revision 1.1.2.2  2005/06/22 21:45:04  jay
Ongoing fixes to UK pool

Revision 1.1.2.1  2005/06/19 21:37:41  jay
Move Rules variants to separate files

*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "RulesUk.h"
#include "PoolBall.h"
#include "PoolHole.h"
#include "Engine.h"
#include "EngineStatePoolSetUpShot.h"
#include "EngineStatePoolSetUpShotDemo.h"
//#include "EngineStatePoolBallInHole.h"
#include "PoolMisc.h"
#include "LevelServer.h"
#include "EngineStateTitle.h"
#include "GameState.h"
#include "StringUtils.h"
#include "PoolPlayerStats.h"

//#define RULES_DEBUG

namespace Amju
{
GameObject* CreateRulesUk()
{
  return new RulesUk;
}

RulesUk::RulesUk()
{
  m_pBehaviour = new PoolBehaviourUk;
  m_playerColour[0] = 0; // i.e. any colour is ok
  m_playerColour[1] = 0; // i.e. any colour is ok
  m_coloursDecided = false;
  m_playerColoursRemaining[0] = 0;
  m_playerColoursRemaining[1] = 0;

  m_helpfilename = "help-uk.txt";
}

Rules* RulesUk::Clone()
{
  return new RulesUk(*this);
}

const char* RulesUk::GetTypeName() const
{
  return "rules-uk";
}

bool RulesUk::ColoursDecided() const
{
  return m_coloursDecided;
}

int RulesUk::GetPlayerColour(int player) const
{
#ifdef RULES_DEBUG
std::cout << "GetPlayerColour: player is " << player << "\n";
#endif
  Assert(player == 0 || player == 1);

  if (!m_coloursDecided)
  {
    return -1;
  } 
  return m_playerColour[player];
}

void RulesUk::CueBallHitsBall(PoolBall* pBall)
{
  m_shotIsBreak = false;

  // TODO
  // Set foul if player hits wrong colour
  // (Red/yellow or black if colours still on table.)
  if (m_firstBallId == -1)
  { 
    // First ball to be hit by cue ball. Check the colour.
    m_firstBallId = pBall->GetNumber();

    if (m_firstBallId == 3)
    {
      // Black ball - only legal if there are no colours left for
      // this player.
      if (!ColoursDecided())
      {
        m_text = "Foul! ";
        m_text += GetName();
        m_text += " hit the black first."; // TODO INTL8N!
        ThisPlayerFouls();
      }
      else
      {
        // Only legal if no colours left for this player
        int numBalls = GetNumBallsRemainingForPlayer();
#ifdef RULES_DEBUG
std::cout << "UK RULES: black hit first, balls for player left on table: " << numBalls << "\n";
#endif
        if (numBalls > 1)
        {
          m_text = "Foul! ";
          m_text += GetName();
          m_text += " hit the black first."; // TODO INTL8N!
          ThisPlayerFouls();
        }
      }

      // Done testing whether this shot is legal
      return;
    }

    int id = Engine::Instance()->GetGameState()->GetCurrentPlayer();
    int playerColour = m_playerColour[id];
    if (!IsFreeShot() && 
        playerColour > 0 && 
        pBall->GetNumber() != playerColour)
    {
      // Colours have been decided, and this player just hit the wrong
      // one with the cue ball.
      m_text = "Foul! ";
      m_text += GetName();
      m_text += " hit the wrong colour ball."; // TODO INTL8N!
      ThisPlayerFouls();
    }
  }
}

int RulesUk::GetNumBallsRemainingForPlayer()
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
  int num = m_playerColour[id];
  // Also check for black ball
  int blackNum = 3; // TODO CONFIG

  int count = 0;
  // Get the game objects which are in the same room as the player.
  int levelId = pLevel->GetId();
  int roomId = pLevel->GetRoomId();
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    PGameObject pGo = it->second;
    if (IsPoolBall(pGo.GetPtr()))
    {
      PoolBall* p = dynamic_cast<PoolBall*>(pGo.GetPtr());
      Assert(p);
      if (InPlay(p) && !IsCueBall(p) &&
          (p->GetNumber() == num || p->GetNumber() == blackNum) )
      {
        ++count;
      }
    }
  }

#ifdef RULES_DEBUG
std::cout << "** UK Pool: no of balls left for player: " << count << "\n";
#endif

  return count;
}

bool RulesUk::PotEndsGame()
{
  // ** HACK ** decide colours here
  // TODO put this somewhere nicer
  if (!m_coloursDecided && GotValidPot())
  {
    bool canDecide = true;
    Assert(GotValidPot());
    Assert(!m_ballsPottedThisShot.empty());
    int colour = m_ballsPottedThisShot[0]->GetNumber();
    // If one ball potted, or all balls the same colour, fix the player colours.
    for (unsigned int i = 1; i < m_ballsPottedThisShot.size(); i++)
    {
      if (colour != m_ballsPottedThisShot[i]->GetNumber())
      {
        // Balls of different colours potted - can't decide yet.
        canDecide = false;
      }
    }

    if (canDecide)
    {
      int id = Engine::Instance()->GetGameState()->GetCurrentPlayer();
      m_coloursDecided = true;
      m_playerColour[id] = colour;
      Assert(id == 0 || id == 1);
      int other = 1 - id;
      int otherColour = 1;
      if (colour == 1)
      {
        otherColour = 2; // TODO CONFIG - depends on ball numbers in data.
      }
      m_playerColour[other] = otherColour;

#ifdef RULES_DEBUG
std::cout << "UK RULES: Decided colours: player 0: " << m_playerColour[0] << " player 1: " << m_playerColour[1] << "\n";
#endif

    }
    else
    {
#ifdef RULES_DEBUG
std::cout << "UK RULES: Can't decide player colours yet.\n";
#endif
    }
  }

  // TODO check for 8 ball - if potted and colours remain for the player,
  // it's game over - otherwise this player has won.
  // Is 8-ball potted ?
  PoolBall* p8Ball = GetNumberBall(m_pLevel, 3); // black ball no. is 3
  if (!InPlay(p8Ball))
  {
    // Out of play => black has been Potted
    int numRem = GetNumBallsRemainingForPlayer();
#ifdef RULES_DEBUG
std::cout << "Black ball was potted... num rem colours: " << numRem << "\n";
#endif

    // Try to give accurate explanation of what happened.
    if (numRem < 1 && IsShotFoul())
    {
      m_text = GetName();
      m_text += " fouled on the black and loses."; // TODO INTL8N!
      ThisPlayerLoses();
      return true; // game ends
    }
    else if (!ColoursDecided() || 
        numRem > 0 ||
        IsShotFoul())
    {
      // 8-ball potted with colours remaining, this player loses
#ifdef RULES_DEBUG
if (numRem > 1)
{
  std::cout << "UK RULES: black potted before all colours, lose game.\n";
}
else if (IsShotFoul())
{
  std::cout << "UK RULES: black potted but shot is foul, lose game.\n";
}
#endif
      m_text = GetName();
      m_text += " potted the black and loses."; // TODO INTL8N!
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

void RulesUk::BallInPocket(PoolBall* pBall)
{
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
    // PLACE_BALL
    m_placeBallHeadstring = true;
    SetBallOutOfPlay(pBall);
  }
  else
  {
    // For UK pool, check that this ball is the right colour.
    int id = Engine::Instance()->GetGameState()->GetCurrentPlayer();
    int playerColour = m_playerColour[id];
    if (!IsFreeShot() && 
        playerColour > 0 && 
        pBall->GetNumber() != 3 &&    // handle potting black elsewhere
        pBall->GetNumber() != playerColour)
    {
      // Foul - potted wrong colour ball
      m_text = "Foul! ";
      m_text += GetName();
      m_text += " potted the wrong colour ball.";

      ThisPlayerFouls();
      m_resetCueBall = true;
      // PLACE_BALL
      SetBallOutOfPlay(pBall);
    }
    else
    {
      SetBallOutOfPlay(pBall);
      ThisPlayerGetsBall(pBall);

      int numRem = GetNumBallsRemainingForPlayer();
      if (pBall->GetNumber() == 3 && numRem > 0)
      {
        ThisPlayerFouls();
      }
    }
  }
}

// New overrides
void RulesUk::OnRoomEntry()
{
  // New frame
  m_playerColour[0] = 0; // i.e. any colour is ok
  m_playerColour[1] = 0; // i.e. any colour is ok
  m_coloursDecided = false;
  m_playerColoursRemaining[0] = 0;
  m_playerColoursRemaining[1] = 0;

  Rules::OnRoomEntry();

  ResetFreeShot();

  m_text = "UK pool";
}

void RulesUk::Reset()
{
  // New shot
  Rules::Reset();
}

void RulesUk::BallHitsRail(PoolBall* pBall)
{
  Rules::BallHitsRail(pBall);
}

void RulesUk::DecShotCounter()
{
  Rules::DecShotCounter();
  // UK rules: if we are on the black, you never get a second shot.
  if (GetNumBallsRemainingForPlayer() == 1)
  {
    m_shotsRemaining = 0;
  }
}

void RulesUk::BallsHaveStopped()
{
  Rules::BallsHaveStopped();
  DecFreeShotCounter();  
}

void RulesUk::GivePlayerFoulAward()
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

  // This is unusual because other games don't give an extra shot.
  // Don't give an extra shot if only the black is left.
  if (GetNumBallsRemainingForPlayer() == 1)
  {
#ifdef _DEBUG
std::cout << "RULES UK: Player is on black so does not get an extra shot.\n";
#endif

    return;
  }

#ifdef _DEBUG
std::cout << "RULES UK: Player is NOT on black so DOES get extra shot."
  " Num balls rem: " << GetNumBallsRemainingForPlayer() << "\n";
#endif

  NextPlayerGetsExtraShot(); // increments no of shots

  m_placeBallHeadstring = true; // can place ball anywhere in "Kitchen"
  m_resetCueBall = true; // so must reset ball to spot

  m_text += " ";
  m_text += GetName();
  m_text += " gets a free shot.";

  AwardFreeShot();
}

void RulesUk::AwardFreeShot()
{
  m_isFreeShot = 2;
#ifdef FREE_SHOT_DEBUG
std::cout << "Setting free shot to 2\n";
#endif
}

void RulesUk::ResetFreeShot()
{
  m_isFreeShot = 0;

#ifdef FREE_SHOT_DEBUG
std::cout << "Setting free shot to ZERO\n";
#endif
}

void RulesUk::DecFreeShotCounter()
{
  // If the player is awarded a free shotm, this counter is set to 2. It is 
  // decremented each time the balls stop, so the flag is still non-zero when
  // the player who has the free shot is playing. If we just use a boolean
  // flag there is nowhere to reset it, as we must remember the state across
  // two shots.
  if (m_isFreeShot > 0)
  {
    m_isFreeShot--;
#ifdef FREE_SHOT_DEBUG
std::cout << "RULES UK: Free shot counter: now " << m_isFreeShot << "\n";
#endif
  }
}

bool RulesUk::IsFreeShot() const
{
  Assert(m_isFreeShot >= 0);
  return m_isFreeShot > 0;
}
}

