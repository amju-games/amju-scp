/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: Rules.cpp,v $
Revision 1.1.2.9  2007/07/16 08:24:53  jay
Mark online game as finished: only when the final shot is replayed on the
second client. This is so the final shot is not immediately recognised as the
opponent quitting the game.

Revision 1.1.2.8  2007/07/15 22:02:57  Administrator
"Did not hit a ball" text: was "Did not hit anything" which can be wrong
for trick shots

Revision 1.1.2.7  2007/06/21 18:15:24  jay
Move check for not hitting anything from BallsHaveStopped out to its own
 function. This is so we can check for this condition without moving the
 state on to the next player.

Revision 1.1.2.6  2007/06/17 20:38:40  jay
Route all calls to ThisPlayerFouls via a SetState message, so we can send
this state online

Revision 1.1.2.5  2007/06/10 21:33:25  Administrator
Check for extra states BALL_IN_POCKET and BALL_OUT_OF_BOUNDS

Revision 1.1.2.4  2007/06/10 08:58:04  jay
BallInPocket() is not told which pocket the ball hit. This info is lost
when a State change is used to flag ball/pocket collisions.

Revision 1.1.2.3  2006/08/30 21:12:21  Administrator
Get Pool to build in MSVC

Revision 1.1.2.2  2006/08/14 17:50:25  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:40  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.31  2005/09/29 19:12:25  jay
Allow external events to end the game.

Revision 1.1.2.30  2005/09/25 20:14:55  Administrator
Error msg for debugging - ball off table bug

Revision 1.1.2.29  2005/09/23 19:35:40  jay
Use PoolChangeState

Revision 1.1.2.28  2005/09/20 13:18:20  jay
COMBO: Award player extra hearts

Revision 1.1.2.27  2005/09/16 20:05:47  jay
Changes so derived Trick Shot rules can work.

Revision 1.1.2.26  2005/09/13 23:09:27  jay
Added COMBO counters so we can reward multiple pots etc.

Revision 1.1.2.25  2005/09/06 20:02:09  jay
Add virtual DecShotCount() and override for UK rules, so we never get
two shots on the black.

Revision 1.1.2.24  2005/09/05 20:08:13  jay
Turn off debug output

Revision 1.1.2.23  2005/08/04 19:39:55  jay
Fix Rules bugs for v.1.1

Revision 1.1.2.22  2005/08/02 18:28:05  jay
Remove Break text; it doesn't work with undo.

Revision 1.1.2.21  2005/07/30 12:26:22  jay
Added some debug output

Revision 1.1.2.20  2005/07/06 20:19:13  jay
Wav change

Revision 1.1.2.19  2005/07/05 09:39:50  jay
Added help text

Revision 1.1.2.18  2005/06/29 20:27:17  jay
Ongoing fixes to Rules.

Revision 1.1.2.17  2005/06/24 05:38:27  Administrator
Fixes to get Scene Editor to compile

Revision 1.1.2.16  2005/06/22 21:44:14  jay
Added overrideable function to award player when opponent fouls.

Revision 1.1.2.15  2005/06/19 21:36:16  jay
Move subclasses to separate files

Revision 1.1.2.14  2005/06/13 22:13:04  jay
Draw() draws AI lines

Revision 1.1.2.13  2005/06/04 22:38:56  jay
Added some meat to UK rules; still patchy

Revision 1.1.2.12  2005/05/24 20:45:25  jay
Additions for UK Pool

Revision 1.1.2.11  2005/05/15 17:22:24  jay
Clear big text on Reset.

Revision 1.1.2.10  2005/05/08 09:57:55  jay
Ongoing improvements - check for first ball hit by cue ball

Revision 1.1.2.9  2005/04/21 17:33:51  jay
Add small, explanatory text and big text

Revision 1.1.2.8  2005/04/11 21:24:35  jay
Distinguish between letting the user place the ball behind the headstring
and letting the user place the ball anywhere.

Revision 1.1.2.7  2005/04/03 16:31:25  jay
Added common function for setting balls out of play.

Revision 1.1.2.6  2005/04/01 06:03:14  jay
Added 'shot is break' flag
Added Behaviours to Rules: The Rules object creates the AI behaviour
corresponding to the Rules.

Revision 1.1.2.5  2005/03/20 20:13:28  jay
Ongoing work for Pool game

Revision 1.1.2.4  2005/03/19 22:06:13  Administrator
Scene Ed fixes

Revision 1.1.2.3  2005/03/19 17:22:57  jay
Add Scene Ed code - saving, properties.

Revision 1.1.2.2  2005/03/17 23:11:24  Administrator
Get Scene Ed to build

Revision 1.1.2.1  2005/03/17 20:53:40  jay
New classes for Pool game

*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Rules.h"
#include "PoolBall.h"
#include "PoolHole.h"
#include "Engine.h"
#include "EngineStatePoolSetUpShot.h"
#include "EngineStatePoolSetUpShotDemo.h"
#include "EngineStatePoolBallInHole.h"
#include "PoolMisc.h"
#include "LevelServer.h"
#include "EngineStateTitle.h"
#include "PoolGameState.h"
#include "StringUtils.h"
#include "PoolPlayerStats.h"
#include "PoolChangeState.h"
#include "StateStrings.h"
#include "PoolOnline.h"

#ifdef _DEBUG
//#define RULES_DEBUG
#endif

namespace Amju
{
extern void ReportError(const std::string&);

void Rules::SetState(State newState)
{
  // TODO Online game: if this client is taking shot, set state to FOUL when
  //  a foul happens. Then in here we call the foul function.
  // If replaying shot, this Rules object gets its state set by messages
  //  from the other client. We use these messages to call the foul function,
  //  rather than detecting fouls locally.
#ifdef RULES_DEBUG
std::cout << "Rules::SetState: object " << GetId() 
  << " new state: " << StateStrings[newState]  << "\n";
#endif

  GameObject::SetState(newState);

  if (newState == FOUL)
  {
    ThisPlayerFouls2(); // really handle the foul
  }
}

std::string Rules::GetName()
{
  return Engine::Instance()->GetGameState()->GetCurrentPlayerInfo()->m_name;
}

HelpText* Rules::GetHelpText()
{
  return &m_helpText;
}

Rules::Rules()
{
  m_helpfilename = "** set in subclass **";
}

bool Rules::Load(File*)
{
  // Default impl - only loads help text.
  return LoadHelpText();
}

void Rules::SetGameOverForExtReason(int winnerId, const std::string& text)
{
  // When the current shot is over we will end the game, with the given player
  // being the winner.
  m_externalWinnerId = winnerId;
  m_externalText = text;
}

void Rules::UpdateStats(int id, bool win, const std::string& opname)
{
  PlayerStats* p = Engine::Instance()->GetGameState()->GetPlayerInfo(id)->GetPlayerStats();
  if (!p)
  {
#ifdef RULES_DEBUG
std::cout << "No player stats for player " << id << "\n";
#endif
    return;
  }
  PoolPlayerStats* pp = dynamic_cast<PoolPlayerStats*>(p);
  Assert(pp);

  // Don't update if this is a friendly game
  if (Engine::Instance()->GetGameState()->IsFriendly())
  {
    return;
  }

  PoolPlayerStats::Result r = PoolPlayerStats::POOL_RESULT_LOSE;
  if (win)
  {
    r = PoolPlayerStats::POOL_RESULT_WIN;
  }

  int level = LevelServer::Instance()->GetCurrentLevel()->GetId();
  int room = LevelServer::Instance()->GetCurrentLevel()->GetRoomId(); 
  pp->Update(r, opname, level, room);

  if (pp->Save())
  {
#ifdef RULES_DEBUG
std::cout << "SAVED PLAYER STATS for " << id << "\n";
#endif
  }
  else
  {
#ifdef RULES_DEBUG
std::cout << "FAILED to save player stats for " << id << "\n";
#endif
  }
}

void Rules::UpdateStats()
{
  // The current player has won.
  int id = Engine::Instance()->GetGameState()->GetCurrentPlayer();
  Assert(id == 0 || id == 1);
  int other = 1 - id;
  
  std::string name = Engine::Instance()->GetGameState()->GetCurrentPlayerInfo()->m_name;
  std::string othername = Engine::Instance()->GetGameState()->GetPlayerInfo(other)->m_name;

#ifdef RULES_DEBUG
std::cout << "UPDATE STATS: winner id: " << id << " loser id: " << other << " (" << name.c_str() << "/" << othername.c_str() << ")\n";
#endif

  UpdateStats(id, true /* win */, othername);
  UpdateStats(other, false /* lose */, name);
}

std::string Rules::GetBigText() const
{
  return m_bigText;
}

std::string Rules::GetExplText() const
{
  return m_text;
}

bool Rules::PlayerMayPlaceCueBall() const
{
  return m_placeBall;
}

void Rules::Draw()
{
#ifdef RULES_DEBUG
  if (m_pBehaviour.GetPtr())
  {
    m_pBehaviour->Draw();
  }
#endif  
}

bool Rules::LoadHelpText()
{
  if (!m_helpText.Load(m_helpfilename))
  {
    std::string s = "Failed to load help text: ";
    s += m_helpfilename;
    ReportError(s);
    return false;
  }
  return true;
}

bool Rules::PlayerMayPlaceCueBallBehindHeadstring() const
{
  return m_placeBallHeadstring;
}

void Rules::SetBallOutOfPlay(PoolBall* p)
{
  p->SetFalling(false); // so we can set vel
  p->SetForwardVel(0);
  p->SetForwardAccel(0);

  Assert(p->GetState() == BALL_IN_POCKET || p->GetState() == BALL_OUT_OF_BOUNDS);

  // POOL_ONLINE: Leave in one of the above states, so when we send object
  // state to server, we don't just send OUT_OF_PLAY.
//  p->SetState(OUT_OF_PLAY);
}

void Rules::BallOutOfBounds(PoolBall* p)
{
#ifdef RULES_DEBUG
std::cout << "RULES: ball out of bounds: " << p->GetId() << "\n";
std::cout << "POS: " << ToString(*(p->GetOrientation())).c_str() << "\n";
#endif

  SetBallOutOfPlay(p);

  if (!IsPoolBall(p))
  {
    // p is a subtype of PoolBall - do nothing.
    return;
  }

  if (IsGameOver())
  {
#ifdef _DEBUG
std::cout << "BallOutOfBounds called but game is over!\n";
#endif
    return;
  }

  m_text = "Foul! ";
  m_text += GetName();
  m_text += " hit a ball off the table.";

  // TODO Does this count as potted, but a foul - i.e. does the ball
  // get listed by the player that bounced it off the table ??

  ThisPlayerFouls();
  // TODO Store balls which should be replaced on the table.  
  if (IsCueBall(p))
  {
    m_resetCueBall = true;
    // Next player may place ball anywhere behind headstring
    // TODO or anywhere, depending on Rules subclass ?
    m_placeBallHeadstring = true; // PLACE_BALL
  }
}

const BoundingSphere* Rules::GetBoundingSphere() const
{
  return &m_bsphere;
}

void Rules::CreateBoundingSphere(const Matrix& cm)
{
}

const char* Rules::GetTypeName() const
{
  // TODO Override this in each subclass, IF we want to create
  // objects with a factory.
  return "rule";
}

void Rules::OnRoomEntry()
{
#ifdef RULES_DEBUG
std::cout << "RULES: OnRoomEntry.\n";
#endif

  // TODO This doesn't work after undo 
  // because m_text is restored, but is no longer correct ??
  // - this text doesn't belong here.
  //m_text = GetName();
  //m_text += " to break.";
  m_text = "";

  m_resetCueBall = true;
  m_shotIsBreak = true;
  m_placeBall = false;
  // NB It's a break - player may place ball anywhere beind headstring.
  // PLACE_BALL
  m_placeBallHeadstring = true;
  m_shotsRemaining = 1;

  m_numBallsPottedThisVisit = 0;
  m_lastBallPotted[0] = -1;
  m_lastBallPotted[1] = -1;

  m_externalWinnerId = -1;
}

void Rules::Reset()
{
#ifdef RULES_DEBUG
std::cout << "RULES: Reset.\n";
#endif

  m_isFoul = false;
  m_gotValidPot = false;
  m_nominatedPocketId = -1;
  m_nominatedBallId = -1;
  m_resetCueBall = false;
//  m_shotIsBreak = false;
  // NB DON'T reset m_placeBall and m_placeBallHeadstring until the shot is taken.
  // It's ok, at this point the shot has been set up, and we have just activated
  //  EngineStatePoolShotInPlay.
  m_placeBall = false;
  m_placeBallHeadstring = false;

  // Reset ID of first object ball to be hit. This gets updated when the cue
  // ball hits the first object ball.
  m_firstBallId = -1;

  // Reset text.
  m_text = "";
  m_bigText = "";

  m_ballsPottedThisShot.clear();

  // Set m_foulLosesGame to true if the current player only has one ball
  // left on the table.
  // DO put this in the base class, so we don't pot the last ball, then
  // foul, then carry on playing.
  Assert(GetNumBallsRemainingForPlayer() > 0);
  m_foulLosesGame = false;
  if (GetNumBallsRemainingForPlayer() == 1)
  {
    m_foulLosesGame = true;
  }

}

bool Rules::ShotIsBreak() const
{
  return m_shotIsBreak;
}

bool Rules::ResetCueBall() const
{
  return m_resetCueBall;
}

bool Rules::IsShotFoul() const
{
  return m_isFoul;
}

void Rules::BallInPocket(PoolBall* pBall)
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
    SetBallOutOfPlay(pBall);
  }
  else
  {
    // For UK pool, check that this ball is the right colour.
    // For other pool games, check this ball has the lowest number.
    SetBallOutOfPlay(pBall);
    ThisPlayerGetsBall(pBall);
  }
}

void Rules::CueBallHitsBall(PoolBall* pBall)
{
  m_shotIsBreak = false;

  // If this _doesn't_ happen between Reset and BallsHaveStopped, 
  // this shot is a foul. Otherwise it may be ok, depending on the
  // ball the cue hits first.

#ifdef RULES_DEBUG
std::cout << "RULES: cue ball hit ball: " << pBall->GetId() << "\n";
#endif

  if (m_firstBallId == -1)
  {
    m_firstBallId = pBall->GetId();
  }
}

void Rules::BallHitsRail(PoolBall* pBall)
{
  // For some rules, a break must cause some number of balls to hit
  // the rails.
}

int Rules::GetNumBallsRemainingForPlayer() 
{
  // Default impl: count all balls left on the table, 
  // not counting the cue ball. UK pool needs a different counting method.

  Level* pLevel = LevelServer::Instance()->GetCurrentLevel().GetPtr();
  Assert(pLevel);
 
  int count = 0; 
  // Get the game objects which are in the same room as the player.
  int levelId = pLevel->GetId();
  int roomId = pLevel->GetRoomId();
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    PGameObject pGo = it->second;
    // Only count PoolBalls - not special subclassed objects.
    if (IsPoolBall(pGo.GetPtr()) && !IsCueBall(pGo.GetPtr()))
    {
      Assert(dynamic_cast<PoolBall*>(pGo.GetPtr()));
      if (InPlay(pGo))
      {
        ++count;
      }
    }
  }

#ifdef RULES_DEBUG
std::cout << "RULES: Number of balls remaining for player: " << count << "\n";
#endif
  return count; 
}

void Rules::DecShotCounter()
{
  if (m_shotsRemaining > 0)
  {
    m_shotsRemaining--;
  }
}

void Rules::Update()
{
  // Test to see if an external event has ended the game.
  if (m_externalWinnerId > -1)
  {
    // TODO Win/Lose game - depends if winnerId is current player.
    ThisPlayerLoses(); // TODO

    // Now set this back to -1 so we don't keep doing this every Update.
    m_externalWinnerId = -1;
    // We set some text here explaining what happened.
    m_text = m_externalText;
  }
}

void Rules::CheckForNoBallHit()
{
  // Nothing hit => foul
  // NB If the wrong ball was hit first, foul flag will already have been set
  // by subclass code.
  // NB Another foul could have happened, e.g. cue ball in pocket. If so don't
  // overwrite the existing text.
  if (m_firstBallId == -1 && m_text.empty())
  {
    m_text = "Foul! ";
    m_text += GetName();
    m_text += " didn't hit a ball.";

    ThisPlayerFouls();
  }
}

void Rules::BallsHaveStopped()
{
#ifdef RULES_DEBUG
std::cout << "RULES: Balls have stopped.\n";
#endif

  // Decrement no of shots remaining for player
  DecShotCounter();

  // Make decision about the shot - was it legal, no of points, etc.

  if (IsShotFoul())
  {
#ifdef RULES_DEBUG
std::cout << "RULES: shot was foul...\n";
#endif

/*
    // This logic has a flaw - if you pot the penultimate ball, then pot the
    //  cue ball, this will lose the game.
    // This test has been replaced by setting m_foulLosesGame to true in
    // Reset(), only if the player is on the last ball at the start of
    // the shot.

    // If no balls lefts on the table, this player loses.
    if (GetNumBallsRemainingForPlayer() < 2) // TODO TEMP TEST bug 245
    {
#ifdef RULES_DEBUG
std::cout << "RULES: Foul on last ball for player, so this player loses.\n";
#endif

      ThisPlayerLoses();
      return;
    }
*/

    // Check if foul loses the game.
    // This flag may be set e.g. if we are on the final ball.
    if (FoulLosesGame())
    {
#ifdef RULES_DEBUG
std::cout << "RULES: Foul loses game.\n";
#endif
      ThisPlayerLoses();
      return;
    }
  }
  
  // Subclasses: potted ball may win or lose the game.
  // E.g. UK Pool: potted black before all other colours->lose game
  // N-Ball: potting highest ball without fouling->win game
  //
  // NB This is now called when shot may be foul, and no balls may
  // have been potted.
  if (PotEndsGame())
  {
#ifdef RULES_DEBUG
std::cout << "RULES: In BallsHaveStopped(), PotEndsGame() returned true. Problem ?\n";
#endif

    return;
  }

  if (IsShotFoul())
  {
    GoToNextPlayer();
    GivePlayerFoulAward();
  }
  else if (GotValidPot())
  {
    m_shotsRemaining++;

#ifdef RULES_DEBUG
std::cout << "RULES: Got valid pot, so play on, shots remaining: "
  << m_shotsRemaining << "\n";
#endif

    // Check no of balls left for this player. 
    // If none left, this player has won.
    if (GetNumBallsRemainingForPlayer() == 0)
    {
      ThisPlayerWins();
      return;
    }
  }
  else
  {
    GoToNextPlayer();
  }

  // Change state to SetUpShot state.
  // TODO The next state will depend on the Rules state.
  // If someone has won, the state will be different.
  ChangeStateToSetUpShot();
}

void Rules::GoToNextPlayer()
{
#ifdef RULES_DEBUG
std::cout << "RULES: go to next player.\n";
if (m_shotsRemaining > 0)
{
  std::cout << "  ..DON'T, because this player has " << m_shotsRemaining << " shots left.\n";
}
#endif

  Assert(m_shotsRemaining >= 0);
  if (m_shotsRemaining < 1)
  {
    // COMBO
    m_numBallsPottedThisVisit = 0;

    NextPlayer(); // PoolMisc
    // Now this player gets one shot, which may be bumped up to 2 if this
    // shot was a foul.
    m_shotsRemaining = 1;
  }
}

void Rules::ThisPlayerWins()
{
  m_bigText = "";

  if (IsDemoMode())
  {
    Engine::Instance()->ChangeState(EngineStateTitle::Name, Engine::IMMEDIATE);
  }
  else
  {
    // Update player stats
    // TODO
    // For 2-player online game, the stats may not be local!
    UpdateStats();
 
    Engine::Instance()->ChangeState(EngineStatePoolBallInHole::Name, Engine::IMMEDIATE);

#ifdef POOL_ONLINE
    // Mark game as finished if we have just replayed the final shot.
    // (If we made the shot on this client, don't finish yet - let the 
    // opponent replay it.)
    if (IsOnlineGame() && !IsUserControlled())
    {
      ThePoolOnlineManager::Instance()->SendGameFinished();
    }
#endif
  }
}

void Rules::ThisPlayerLoses()
{
  // TODO any reason why this shouldn't work..?
  // Just flip players, then the current player has won!
  NextPlayer();
  ThisPlayerWins();
}

void Rules::ShowText(const std::string&)
{
}

void Rules::ThisPlayerGetsPoints(int points)
{
}

void Rules::ThisPlayerGetsBall(PoolBall* pBall)
{
  m_gotValidPot = true;
  // TODO increase points for player, or list of potted balls.
  // For UK pool, if no player has potted a ball yet, this decides
  // which colour each player has from now on.

  // POOL COMBO
  // Reward multiple pots in one visit.

  if (!IsShotFoul())
  {
    ++m_numBallsPottedThisVisit;
    // If more than one ball has been potted this visit, show the number.
    if (m_numBallsPottedThisVisit > 1)
    {
      m_bigText = ToString(m_numBallsPottedThisVisit);
      // Reward Combo. I.e. give more hearts.
      int numHearts = 10 * (1 << (m_numBallsPottedThisVisit - 2));
  
      int id = Engine::Instance()->GetGameState()->GetCurrentPlayer();
      Assert(id == 0 || id == 1);
      EngineStatePoolBase::AddExtraHearts(id, numHearts);

#ifdef _DEBUG
std::cout << "COMBO: Hearts: " << numHearts << "\n";
#endif
      // TODO Show this combo in HUD ?
    }
    else
    {
      static int n = 0;
      n %= 3;
      switch (n)
      {
      case 0:
        m_bigText = "cool!";
        break;
      case 1:
        m_bigText = "sweet!";
        break;
      case 2:
        m_bigText = "wow!";
        break;
      }
      ++n;
    }
  }
  m_ballsPottedThisShot.push_back(pBall);
}

void Rules::ThisPlayerFouls()
{
  // Online game:
  // If this client is taking shot, set the state. If replaying shot,
  //  we will set state from message sent by the other client.
  if (IsOnlineGame())
  {
#ifdef RULES_DEBUG
std::cout << "ONLINE GAME FOUL!\n";
#endif

    if (IsUserControlled())
    {
#ifdef RULES_DEBUG
std::cout << "FOUL: Set state to FOUL for object " << GetId() << "\n";
#endif

      SetState(FOUL);
      // Set flag so we send the state of this obj to server
      SetSendState(true);
    }    
    else
    {
      // Do nothing - state message will trigger foul
#ifdef RULES_DEBUG
std::cout << "..But not user controlled!\n";
#endif
    }
  }
  else
  {
    ThisPlayerFouls2();
  }
}

void Rules::ThisPlayerFouls2()
{
#ifdef RULES_DEBUG
std::cout << "RULES: FOUL!\n";
#endif

  // golf_wav_18
  Engine::Instance()->PlayWav(Engine::Instance()->GetConfigValue("golf_wav_18"));

  m_isFoul = true;
  m_shotsRemaining = 0;
  m_bigText = "foul!";
}

void Rules::NextPlayerGetsExtraShot()
{
  // Next player gets a free shot - but not if there is just one 
  // player, practising.
  m_shotsRemaining++;
}

bool Rules::GotValidPot()
{
  return m_gotValidPot;
}

bool Rules::FoulLosesGame()
{
  return m_foulLosesGame;
}

std::string Rules::GetExplanation() const
{
  return m_text;
}

#if defined(SCENE_EDITOR)
bool Rules::Save(File* pf)
{
  return true;
}

GameObject::PropertyMap Rules::GetProperties() const
{
  // TODO
  PropertyMap pm;
  return pm;
}

void Rules::SetProperties(const GameObject::PropertyMap& pm)
{
  // TODO
}
#endif

PoolBehaviour* Rules::GetBehaviour()
{
  return m_pBehaviour.GetPtr();
}
}

