#include <iostream>
#include "Common.h"
#include "EngineStatePoolSetUpShot.h"
#include "EngineStatePoolShotInPlay.h"
#include "EnginePaused.h"
#include "EngineStatePoolCamera.h"
#include "Engine.h"
#include "LevelServer.h"
#include "PoolGameState.h"
#include "PoolCamera.h"
#include "FreeMovingSolidGameObject.h" 
#include "PowerGuage.h"
#include "RotateHelper.h"
#include "AngleCompare.h"
#include "PoolBehaviour.h"
#include "PoolMisc.h"
#include "PoolTeeBox.h"
#include "PoolHole.h"
#include "Trajectory.h"
#include "PoolCharacter.h"
#include "TextWriter.h"
#include "StringUtils.h"
#include "EngineStatePoolShowShot.h"
#include "GuiButton.h"
#include "TextFactory.h"
#include "TextColourizerColourList.h"
#include "PoolCourseManager.h"
#include "ThirdPersonCameraBase.h"
#include "Frustum.h"
#include "Mouse.h"
#include "StateStrings.h"
#include "Rules.h"
#include "NonHeadArea.h"
#include "EngineStatePoolHelp.h"
#include "PoolSnapshot.h"
#include "PoolOnline.h"
#include "Round.h"
#include "ShotLog.h"
#include "PoolChangeState.h"
#include "EsPoolDisconnect.h"
#include "TimePeriod.h"
#include "PlayerNames.h"
#include "EsPoolOnlineAccept2.h"
#include <Unproject.h>
#include <Line3.h>
#include <Screen.h>
#include "LoadButton.h"

//#define SHOOT_BUTTON
//#define SET_UP_SHOT_DEBUG

namespace Amju
{
void ReportError(const std::string&);

static bool nonHeadAreaExists = false;
void RefreshNonHeadAreaFlag()
{
  if (GetNonHeadArea(LevelServer::Instance()->GetCurrentLevel().GetPtr()))
  {
    nonHeadAreaExists = true;
  }
  else
  {
    nonHeadAreaExists = false;
  }
}

bool NonHeadAreaExists()
{
  return nonHeadAreaExists;
}

void OnUndo()
{
  Assert(Engine::Instance()->GetGameState()->IsFriendly());

  // Restore the game state to before the last shot. 
  // Then set up for human player to take the shot.
  TheSnapshotManager::Instance()->Undo();

#ifdef SET_UP_SHOT_DEBUG
std::cout << "** UNDO ** reset cue ball or to: " 
  << ToString(*(GetBall()->GetOrientation())) << "\n";
#endif

  // Reset state 
  Engine::Instance()->GetEngineState()->SetActive(true);

#ifdef SET_UP_SHOT_DEBUG
std::cout << "** UNDO ** after SetActive: cue ball or is now: " 
  << ToString(*(GetBall()->GetOrientation())) << "\n";
#endif
}

void OnRedo()
{
  Assert(Engine::Instance()->GetGameState()->IsFriendly());

  // Get the AI to take this shot.
  Engine::Instance()->GetGameState()->RotateToNextPlayer();

  // Reset state 
  Engine::Instance()->GetEngineState()->SetActive(true);
}

bool EngineStatePoolSetUpShot::GetShowTrajectory() const
{
  return m_showTrajectory;
}

void EngineStatePoolSetUpShot::SetShowTrajectory(bool b)
{
  m_showTrajectory = b;
  if (b)
  {
    // Recalc trajectory
    Vec3f vBall = GetBall()->GetOrientation()->GetVertex();
    // POOL: shot speed is set independently of direction, so just use a dummy
    // horizontal vel.
    float hvel = 10.0f; 

    s_pTrajectory->Recalc(vBall, m_shotYRotate, 0 /*m_shotVerticalVel*/, hvel); 
  }

#ifdef SET_UP_SHOT_DEBUG
std::cout << "Show trajectory: flag is now: " << m_showTrajectory << "\n";
#endif
}

static bool s_shootSelected = false;

void EngineStatePoolSetUpShot::OnShootSelected()
{
  // Called each frame button is held down
  if (!s_shootSelected)
  {
    EngineStatePoolSetUpShot* e = (EngineStatePoolSetUpShot*)
      (Engine::Instance()->GetEngineState(
      EngineStatePoolSetUpShot::Name).GetPtr());

    e->TakeShotStart();
  
    // Start cue swinging ??
    e->s_cue.SetSwingVel(10.0f); // TODO TEMP TEST 

    e->s_cue.SetAutoTakeShot(true);

#ifdef SET_UP_SHOT_DEBUG
std::cout << "TAKE SHOT START\n";
#endif
  }
  else
  {
#ifdef SET_UP_SHOT_DEBUG
std::cout << "TAKE SHOT: s_shootSelected == true\n";
#endif
  }
  s_shootSelected = true; 

  // Increase shot power
}

void EngineStatePoolSetUpShot::OnShoot()
{
  s_shootSelected = false;

  EngineStatePoolSetUpShot* e = (EngineStatePoolSetUpShot*)
    (Engine::Instance()->GetEngineState(
    EngineStatePoolSetUpShot::Name).GetPtr());
  
#ifdef SET_UP_SHOT_DEBUG
std::cout << "ON SHOOT, s_shotSelected == false\n";
#endif

  //e->SetShotPower(0.5f);
  e->SetShotParams();
//  e->s_cue.SetAutoTakeShot(true);
  e->TakeShotFinish();

}

void OnTrajectory()
{
  // Show trajectory button toggled
  EngineStatePoolSetUpShot* e = (EngineStatePoolSetUpShot*)
    (Engine::Instance()->GetEngineState(
    EngineStatePoolSetUpShot::Name).GetPtr());

  e->SetShowTrajectory(!e->GetShowTrajectory());
}

void OnPlaceBall()
{
  bool mode = ((EngineStatePoolSetUpShot*)Engine::Instance()->GetEngineState().GetPtr())->
    GetPlaceBallMode();
  mode = !mode; // toggle mode 

#ifdef SET_UP_SHOT_DEBUG
std::cout << "PLACE BALL MODE: " << (mode ? "yes" : "no") << "\n";
#endif

  ((EngineStatePoolSetUpShot*)Engine::Instance()->GetEngineState().GetPtr())->
    SetPlaceBallMode(mode);
}

void OnCamera()
{
  // Change camera mode from 3rd person to birds eye and back.
  bool be = EngineStatePoolBase::IsBirdsEye(); 

#ifdef CAM_DEBUG
std::cout << "CAMERA BUTTON: current state be: " << be << " new state be: " << !be << "\n";
#endif

  EngineStatePoolBase::SetBirdsEye(!be);
}

void OnMenuButton()
{
  Amju::Engine::Instance()->ChangeState(
    Amju::EnginePaused::Name, 
    Amju::Engine::IMMEDIATE);
}

void OnHelpButton()
{
  Amju::Engine::Instance()->ChangeState(
    Amju::EngineStatePoolHelp::Name, 
    Amju::Engine::IMMEDIATE);
}

void OnCloseHelpButton()
{
  ((Amju::EngineStatePoolSetUpShot*)(
    Amju::Engine::Instance()->GetEngineState(
    Amju::EngineStatePoolSetUpShot::Name).GetPtr()))->
   SetShowHelp(false);
}

const char* EngineStatePoolSetUpShot::Name = "poolsetupshot"; 
// Register this State with the Engine.
static const bool registeredgolfsetupshot = Engine::Instance()->
  RegisterEngineState(EngineStatePoolSetUpShot::Name, new EngineStatePoolSetUpShot);

EngineStatePoolSetUpShot::EngineStatePoolSetUpShot()
{
  m_shotYRotate = 0;
  m_shotVerticalVel = 0;
  m_shotHorizontalSpeed = 0;
  m_drag = false;
  m_shotIsActive = false;
  m_activeTime = 0;
  m_maxActiveTime = 4.0f; // TODO CONFIG
  m_maxShotSpeed = 10.0f; // TODO CONFIG
  m_maxYDiff = 100; // TODO CONFIG
  m_shotAccel = 0;

  PLAYER_OFFSET_ANGLE = 180.0f;
  m_cursorOnBall = false;
  m_currentSong = 0;

  m_helpTimer = 0;
  m_maxHelpTime = 10.0f;
  m_showHelpText = true; 
  m_helpId = 0;

  // Reset avoid vectors for each player.
  s_avoidVec.clear();
  s_avoidVec.reserve(4); // TODO max no of players
  Assert(s_avoidVec.empty()); // NB reserve does not create elements
  s_avoidVec.push_back(Vec3f());
  s_avoidVec.push_back(Vec3f());
  s_avoidVec.push_back(Vec3f());
  s_avoidVec.push_back(Vec3f());
  Assert(s_avoidVec.size() == 4);

  m_showTrajectory = false;
  m_serverStatusSetThisGame = false;
}

void EngineStatePoolSetUpShot::SetPlaceBallMode(bool b) 
{
  // Get rid of big text
  SetBigText("");

  m_placeBallMode = b; 
  if (b)
  {
    // Place ball mode only works in birds eye mode. Otherwise the camera moves 
    // to follow the ball, which changes the mouse posision, so the ball moves...
    // It's a mess.

    // TODO remember birds-eye flag value, then restore it after we exit
    // place ball mode.

    SetBirdsEye(true);

    // Disable birds-eye view toggle. If we toggle it while placing the
    // ball things go terribly wrong.
    // Slightly confusingly, we DO want to force birds-eye view on. We
    // just don't want the player to turn it OFF while placing the ball.
    m_pCameraButton->SetEnabled(false);
    m_pHelpButton->SetEnabled(false);
    m_pMenuButton->SetEnabled(false);
  }
  else
  {
    // Can enable birds-eye toggle.
    m_pCameraButton->SetEnabled(true);
    m_pHelpButton->SetEnabled(true);
    m_pMenuButton->SetEnabled(true);

    // TODO restore previous value
    SetBirdsEye(false);

    // Recalc. best target ball and face it  
    PoolGameObject* pTargetToFace = GetRules(m_pLevel.GetPtr())->GetBehaviour()->GetTargetObject();
    Orientation o = *(GetBall()->GetOrientation());
    // Look at the cue ball, from a position on the opposite side
    // of the cue ball to the object ball.
    Vec3f v = o.GetVertex() - pTargetToFace->GetBoundingSphere()->GetCentre();
    v.Normalise();
    // NB +180 required to pull back in the required direction
    o.SetYRot(180.0f + RadToDeg(atan2(v.x, v.z)));
    // Change the direction the cue ball is facing. This will change
    // the direction the camera pulls back.
    GetBall()->SetOrientation(o);
    GetCamera()->SetOrientation(*(GetBall()->GetOrientation()));
    GetCamera()->Reset();
    m_shotYRotate = GetCamera()->GetOrientation()->GetYRot(); 
    SetShotParams();
  }
}

bool EngineStatePoolSetUpShot::GetPlaceBallMode() const
{
  return m_placeBallMode;
}

void EngineStatePoolSetUpShot::MoveCueStart()
{
  // POOL: Reset cue swing position
  s_cue.ResetSwingPos(); 
  m_shotIsActive = true;

  // Make sure the cue is pointing in the direction that the camera is
  // facing. So we can look around with the arrow keys, then click down,
  // and the cue will point where we are looking.
  if (!IsBirdsEye())
  {
    m_shotYRotate = GetCamera()->GetOrientation()->GetYRot(); 
    SetShotParams();
  }

  Mouse::SetCursor(Mouse::FINGER);
}

void EngineStatePoolSetUpShot::MoveCueFinish()
{
  m_shotIsActive = false;
}

void EngineStatePoolSetUpShot::TakeShotStart()
{
  // Can't start shot if placing ball
  if (GetPlaceBallMode())
  {
    return;
  }

  s_cue.SetSwingMode(true); 
}

void EngineStatePoolSetUpShot::TakeShotFinish()
{
  // Set cue vel so we take the shot
  float s = s_cue.GetSwingPower();
  static const float SWING_VEL_SHOT_MULT = Engine::Instance()->
    GetConfigFloat("swing_vel_shot_mult");
  s *= SWING_VEL_SHOT_MULT;

#ifdef SET_UP_SHOT_DEBUG
std::cout << "TAKE SHOT: Mouse button released, set swing vel to: " << s << "\n";
#endif
  // NB If shot strength is below some limit, cancel the shot.
  if (fabs(s) < 1.0f) // TODO TEMP TEST
  {
    // Cancel the shot
    s_cue.SetSwingMode(false); 
    m_shotIsActive = false; // bug 338 fix, camera stuck 

#ifdef SET_UP_SHOT_DEBUG
std::cout << "TakeShotFinish: cancelling shot because swing power is " 
  << s << "\n";
#endif
  }
  else 
  {
    // Take the shot
std::cout << "Set Swing vel: " << s << "\n";

    s_cue.SetSwingVel(s); // NB This is just how fast the cue moves, it 
      // doesn't directly affect the cue ball vel. Set in SetShotParams().
    s_cue.SetAutoTakeShot(true);

#ifdef SET_UP_SHOT_DEBUG
std::cout << "TakeShotFinish: taking shot, swing power is " 
  << s << "\n";
#endif
  }
}

void EngineStatePoolSetUpShot::CancelShot()
{
  // Call to cancel a shot, e.g. if right button is clicked but then
  // left button is clicked too.
  s_cue.SetSwingMode(false); 
  m_shotIsActive = false; // bug 338 fix, camera stuck 
}

void EngineStatePoolSetUpShot::MoveContactPosStart()
{
  m_pContactPoolGui->SetEnabled(true);
}

void EngineStatePoolSetUpShot::MoveContactPosFinish()
{
  m_pContactPoolGui->SetEnabled(false);
}

void EngineStatePoolSetUpShot::SetShotPower(float f)
{
  if (f > 1.0f)
  {
    f = 1.0f;
  }
  if (f < 0)
  {
    f = 0;
  }

  m_pPowerGuage->Set(f);
  if (f == 0)
  {
    // Cancel shot
    //m_shotIsActive = false;
    Mouse::SetCursor(Mouse::STANDARD);
  }

// TODO Golf hangover ?
//  bool highPower =  (f > 0.5f); // TODO CONFIG or another control sets this ?
//  Engine::Instance()->GetGameState()->GetCurrentPlayerInfo()->m_isHighPower = highPower; 

  SetShotParams();
} 

void AdjustPlayerPos(Orientation& o)
{
  // Adjust so club is behind ball, rather than player character.
  float yrotrads = DegToRad(o.GetYRot() - 90.0f);
  static const float LEN = Engine::Instance()->GetConfigFloat("golf_player_adjust_len");
  o.SetX(o.GetX() + sin(yrotrads) * LEN);
  o.SetZ(o.GetZ() + cos(yrotrads) * LEN);
}

bool EngineStatePoolSetUpShot::AddShotAngle(float rot)
{
    if (fabs(rot) > 0)
    {
      GetActivePlayer()->SetActionPoolShuffle();
    }
    else
    {
      GetActivePlayer()->SetActionReady();
    }

    m_shotYRotate += rot;

    // POOL
    Orientation oPlayer(*(GetBall()->GetOrientation()));
// TODO This doesn't look right - we want the player to rotate 
// around the cue ball.
////    Orientation oPlayer(*(GetActivePlayer()->GetOrientation()));

    //Vec3f v1 = oPlayer.GetVertex();

    oPlayer.SetXRot(0);
    oPlayer.SetYRot(0);
    oPlayer.SetZRot(0);
    float yRadians = DegToRad(m_shotYRotate + PLAYER_OFFSET_ANGLE);
    float dx = sin(yRadians);
    float dz = cos(yRadians);
  
    static const float DIST_TO_BALL = GetEngine()->GetConfigFloat("golf_dist_to_ball");

    oPlayer.SetX(oPlayer.GetX() + dx * DIST_TO_BALL);
    oPlayer.SetZ(oPlayer.GetZ() + dz * DIST_TO_BALL);

    oPlayer.SetYRot(m_shotYRotate);

    AdjustPlayerPos(oPlayer);
    bool res = false;

    // Change position of active player to take-shot position: 
    //  we want to do this if this is a local AI player.
    // MAY19
// TODO TEMP TEST - do we need this for pool ?
// Apparently we do, for the AI player.
    PoolGameState::PlayerInfo* pInfo = Engine::Instance()->GetGameState()->GetCurrentPlayerInfo();
    if (pInfo->m_isAi && !pInfo->m_isOnline)
    {
      GetActivePlayer()->SetOrientation(oPlayer);
    }
    res = true;

    SetShotParams();

    return res;
} 

void EngineStatePoolSetUpShot::SetStartPosition(int i /*playerIndex*/)
{
}

Orientation GetTeeSpot()
{
  Level* pLevel = LevelServer::Instance()->GetCurrentLevel().GetPtr();
  Orientation o;
  PoolTeeBox* pTee = GetTeeBox(pLevel);
  const BoundingSphere* bb = pTee->GetBoundingSphere();
  const Vec3f& v = bb->GetCentre();
  o.SetX(v.x);
  o.SetY(v.y);
  o.SetZ(v.z);
  // POOL
  // Move the ball so it's resting on the ground rather than sitting in it.
  float radius = GetBall()->GetBoundingSphere()->GetRadius();
  o.SetY(o.GetY() + radius);

#ifdef SET_UP_SHOT_DEBUG
std::cout << "TEE: " << pTee << "  SPOT: " << ToString(o) << "\n";
#endif

  return o;
}

void EngineStatePoolSetUpShot::SetStartPositions()
{
#ifdef SET_UP_SHOT_DEBUG
std::cout << "SET START POSITIONS\n";
#endif

  // Reset the cue ball to the tee position if the Rules say we should.
  if (GetRules(m_pLevel.GetPtr())->ResetCueBall())
  {
#ifdef SET_UP_SHOT_DEBUG
std::cout << "SET START POSITIONS: Resetting cue bal pos to tee box \n";
#endif

    // Check the cue ball does not intersect another ball. If so we move
    // the cue ball around until we find a good spot for it.
    bool goodSpotFound = false;
    float offset = 0;
    while (!goodSpotFound)
    {
      Orientation o = GetTeeSpot();
      // Add a random offset so breaks are different each time.
      float randOffset = 0; 
      // For online game we absolutely do NOT want any randomness!
      // We want the game state to be completely deterministic.
      if (!IsOnlineGame())
      {
        randOffset = (float)rand() / (float)RAND_MAX * 2.0f - 1.0f;
      }

      // Add an (x, z) offset to the Tee position.
      o.SetX(o.GetX() + offset + randOffset);

      // Must set ball pos before call to CueBallPosOk, because it checks
      // the capsule from old position to new position. We just want to
      // check the sphere.
      GetBall()->SetOrientation(o); 
#ifdef POOL_ONLINE
        ShotLog("place cue ball 5");
#endif
      if (CueBallPosOk(o.GetVertex()))
      {
        goodSpotFound = true;
      }
      offset += 1.0f;
    }
  }

  // Set the orientation for all players.
  int numPlayers = GetEngine()->GetGameState()->GetNumberOfPlayers();
  for (int i = 0; i < numPlayers; i++)
  {  
    PoolGameState::PlayerInfo* pInfo = GetEngine()->GetGameState()->GetPlayerInfo(i);
    int id = pInfo->m_id;
    PPoolGameObject pGo = GetEngine()->GetGameObject(id);

    // This fails if there are not enough player characters in the level :-(
    Assert(pGo.GetPtr());

    pGo->SetState(UNKNOWN);

    static const float AWAY = 60.0f;
    if (id == GetActivePlayer()->GetId())
    {
      // If player is online, make sure player is out from the table
      // while we wait for the opponent to make her shot.
      if (pInfo->m_isOnline)
      {
        // NB Opposite side of table to the non-active player
        pGo->SetOrientation(Orientation(AWAY, 0, -AWAY, 0, 0, 0));
      }
      else
      {
        pGo->SetOrientation(Orientation(20.f, 0, 20.0f, 0, 0, 0));
      }
    }
    else
    {
      // TODO TEMP TEST - get non-active character out of the way of the table.
      pGo->SetOrientation(Orientation(AWAY, 0, AWAY, 0, 0, 0));
    }

    ((CharacterGameObject*)pGo.GetPtr())->RefreshHeightServer();
    ((CharacterGameObject*)pGo.GetPtr())->SetActionIdle();

  }
}

void EngineStatePoolSetUpShot::ResetPlayerFlags()
{
#ifdef SET_UP_SHOT_DEBUG
std::cout << "*** Resetting player flags, including balls potted ****\n";
#endif

  int numPlayers = GetEngine()->GetGameState()->GetNumberOfPlayers();
  for (int i = 0; i < numPlayers; i++)
  { 
    // Fix for #37 when you exit and restart course. Dead players should not
    // be resurected.
    int numballs = GetEngine()->GetGameState()->GetPlayerInfo(i)->m_numBalls;
    if (numballs > 0)
    {
      GetEngine()->GetGameState()->GetPlayerInfo(i)->m_isPlaying = true;
    }
    GetEngine()->GetGameState()->GetPlayerInfo(i)->m_reset = true;//false;
    GetEngine()->GetGameState()->GetPlayerInfo(i)->m_strokes = 0;

    // POOL: Clear the player's list of balls potted
    GetEngine()->GetGameState()->GetPlayerInfo(i)->m_tqs.clear();
  }
}

void EngineStatePoolSetUpShot::OnNewRoom()
{
#ifdef SET_UP_SHOT_DEBUG
  std::cout << "************** On NewRoom\n";
#endif
  m_pLevel = LevelServer::Instance()->GetCurrentLevel();
  int roomId = m_pLevel->GetRoomId();

  // Go to the next song - wrap if there are no more songs.
  m_currentSong = (roomId + 1) % m_songs.size();
  Assert(m_currentSong >= 0);
  Assert(m_currentSong < (int)m_songs.size());
  Engine::Instance()->PlaySong(m_songs[m_currentSong]);

  EngineStatePoolBase::OnNewRoom();
  ResetPlayerFlags();

  // Pick player to break
  static int count = 0;
  count++;

  if (!IsOnlineGame())
  { 
    // TODO!!! HACK because this function is getting called TWICE
    // at the start of each frame!!! WHY is it being called twice ?!?!
    int player = rand() % 2; //(count % 4) >> 1;

#ifdef SET_UP_SHOT_DEBUG
std::cout << "PLAYER " << player << " to break\n";
#endif
    Engine::Instance()->GetGameState()->SetCurrentPlayer(player);
    // NB Trick shot rules will overwrite this player, because the AI should 
    // never start.
  }

#ifdef ONLINE_DEBUG
/////////////////////////////////////////
// ONLINE: WTF is going on with the players ?
//
std::cout << "PLAYERS:\n";
GameState::PlayerInfo* p = Engine::Instance()->GetGameState()->GetPlayerInfo(0);
std::cout << "P1: " << p->m_name.c_str() 
  << " isAI: " << p->m_isAi << " isOnline: " << p->m_isOnline << "\n";

p = Engine::Instance()->GetGameState()->GetPlayerInfo(1);
std::cout << "P2: " << p->m_name.c_str() 
  << " isAI: " << p->m_isAi << " isOnline: " << p->m_isOnline << "\n";

std::cout << "Current player: " 
<< Engine::Instance()->GetGameState()->GetCurrentPlayer() << "\n"; 
//
//
/////////////////////////////////////////
#endif

  // Make sure Rules knows which player is to break.
  GetRules(m_pLevel.GetPtr())->OnRoomEntry();

  // Set the start position of the cue ball. Setting the players
  // positions may not work here (?)
  // NB We must ensure the cue ball is in this room!
  // There is one cue ball per level.
  GetEngine()->GameObjectChangeRoom(GetBall()->GetId(), roomId);

  // MAY19 - shurely not needed here ???!
//  SetStartPositions();

  // Initially standing ready to take shot.
  GetActivePlayer()->SetActionReady(); 

  // Set the camera orientation to the same point as the player. This will be adjusted
  // to the min. distance from the player, but means we start off with the 
  // player in sight.

  // Bad idea: player target not set in camera, so crashes
//  SetBirdsEye(false); // TODO TEST: added so quitting in birds-eye mode does not
    // affect camera in next game

  if (!IsBirdsEye())
  {
    GetCamera()->SetOrientation(*(GetActivePlayer()->GetOrientation()));
  }

  // Start with 3rd person camera. Point it at the Ball.
  // TODO remain in birds eye mode if that's what the player has chosen
  GetCamera()->SetPlayer(GetBall()); 
  GetCamera()->Reset();

  m_shotIsActive = false;

  //GetBall()->SetActivePlatform(0);
  // check if tee is on a platform - this is done in SetActive()

  EngineStatePoolBase::CreateBonusesLeft();

  // Reset any big text
  s_pBigText = 0;
 
  // Reset the undo stack
  TheSnapshotManager::Instance()->Reset();
  // Get the initial state. This is a special call to GetSnapshot.
  // This first snapshot never gets popped off the stack.

  TheSnapshotManager::Instance()->GetSnapshot();

  // Show path/trajectory:
  // If practice game, paths are on; if not a practice game, paths are off.
  // NB They can be switched on/off for all games now - this just changes the
  // default state at start of game.
  SetShowTrajectory(Engine::Instance()->GetGameState()->IsFriendly());

#ifdef POOL_ONLINE 
  if (IsOnlineGame())
  {  
    m_onlineBehaviour.OnNewRoom();
  }
#endif
 
  // Find if there is a Non-head area or not - crazy tables don't. 
  RefreshNonHeadAreaFlag();
}

void EngineStatePoolSetUpShot::OnNewLevel()
{
#ifdef SET_UP_SHOT_DEBUG
std::cout << "OnNewLevel\n";
#endif

  EngineStatePoolBase::OnNewLevel();

  ResetPlayerFlags();
  // Initialises to start of current level.
  m_pLevel = LevelServer::Instance()->GetCurrentLevel();
  // Tell the player character what level we're in.
  GetActivePlayer()->SetLevel(m_pLevel.GetPtr());
  // Set the starting room.
  m_pLevel->SetRoomId(0);
  SetPlayerRoom(m_pLevel->GetStartRoomId());

  m_currentSong = 0;
}

void EngineStatePoolSetUpShot::OnNewGame()
{
#ifdef SET_UP_SHOT_DEBUG
  std::cout << "OnNewGame\n";
#endif

  EngineStatePoolBase::OnNewGame();
  m_pLevel = LevelServer::Instance()->GetCurrentLevel();

  // Set player state, so shadow is shown.
  GetActivePlayer()->SetState(UNKNOWN);

  float bounce = GetEngine()->GetConfigFloat("golf_ball_bounce");
  GetBall()->SetBounceDampen(bounce);

  m_helpTimer = 0;
  m_helpId = 0;
  m_serverStatusSetThisGame = false;
}

bool EngineStatePoolSetUpShot::CueBallPosOk(const Vec3f& newpos)
{
  // Return true if cue ball does not intersect another ball, and does
  // not intersect scene.
  // TODO pass in old, new coords ?

  FreeMovingGameObject* pCueBall = GetBall();

  BoundingSphere bs = *(pCueBall->GetBoundingSphere());
  BoundingSphere newbs(bs);
  newbs.SetCentre(newpos);
  if (pCueBall->GetHeightServer()->Intersects(bs, newbs))
  {
#ifdef SET_UP_SHOT_DEBUG
std::cout << "PLACE BALL: no good, intersects scene\n";
#endif
    return false;
  }

  // Test for ball-cue ball intersection
  int levelId = m_pLevel->GetId();
  int roomId = m_pLevel->GetRoomId();

  // True if player may only place ball behind headstring, not anywhere on the table.
  bool behindHeadstring = 
    GetRules(m_pLevel.GetPtr())->PlayerMayPlaceCueBallBehindHeadstring() && 
    !GetRules(m_pLevel.GetPtr())->PlayerMayPlaceCueBall() &&
    NonHeadAreaExists();

#ifdef SET_UP_SHOT_DEBUG
std::cout << "CUE BALL POS OK: behindHeadstring: " << behindHeadstring << "\n";
#endif

  // Get the game objects which are in the same room as the player.
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);

  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    PPoolGameObject pGo1 = it->second;
    State s = pGo1->GetState();

    if (InPlay(pGo1) && 
        !IsCueBall(pGo1.GetPtr()) && 
        dynamic_cast<PoolBall*>(pGo1.GetPtr()))
    {
      if (pGo1->GetBoundingSphere()->Intersects(newbs))  //Intersects(*pCueBall))
      {
#ifdef SET_UP_SHOT_DEBUG
std::cout << "PLACE BALL: No good, ball intersects: " << pGo1->GetId() << "\n";
#endif
        return false;
      }
    }

    // Check for non-headstring area
    NonHeadArea* pNha = dynamic_cast<NonHeadArea*>(pGo1.GetPtr()); 
    if (behindHeadstring && s != OUT_OF_PLAY && pNha) // NHA, not a ball
    {
      // Check for ball-nonheadarea intersection
      // TODO 
      const HeightServer* pHs = pNha->GetCollisionVol();
      if (pHs->Intersects(bs, newbs)) // TODO before and after spheres
      {
#ifdef SET_UP_SHOT_DEBUG
std::cout << "Ball intersects non head area\n";
#endif
        return false;
      }
    }
  }
  return true;
}

void EngineStatePoolSetUpShot::Draw()
{
  EngineRunningBase::DrawBackground();

/*
  // No point in testing for cursor on ball ..??

  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ);
  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_WRITE);
  Colour c = PickHelper::ReadCursorColour();
  GetBall()->Draw();
  Colour c2 = PickHelper::ReadCursorColour(); 
  m_cursorOnBall = (c != c2);
  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_WRITE);
  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_READ);
*/

  // POOL: Draw the scene (i.e. the table) before UnProjecting the mouse coords.

  // Create the view frustum. This is used in the drawing calls to cull objects
  // out of view.
  Frustum::Instance()->Create();

  // Draw static elements of scene. Get number of polys drawn.
  m_scenePolys = m_pLevel->Draw();

  // If this flag is true, we use gluUnproject to get the mouse coords.
  // This is used either to aim the cue ball, or move it if we are in 
  // "place ball" mode.
  bool getPos = false;
  // Set if we are dragging the mouse and are in "place ball" mode
  if (m_placeBallMode && m_drag) 
  {
    getPos = true;
  }
  // If left-dragging in Birds-eye mode, we are placing the cue. We need to
  // get the mouse pos in 3D space so we can get a line from the cue ball
  // centre to the mouse pos.
  // TODO differentiate between l-drag and r-drag
  if (IsBirdsEye() && m_drag)
  {
    // disallow
    // getPos = true;
  }
/*
  // Don't place ball if mouse is above Place Ball Button.
  if (m_pPlaceBallButton->IsMouseOver() || m_pGoButton->IsSelected())
  {
    getPos = false;
  }
*/

#ifdef SET_UP_SHOT_DEBUG
  if (!getPos)
  {
    GetRules(m_pLevel.GetPtr())->Draw();
  }
#endif

  if (getPos)
  {
    m_newMouseDown = false;

    // Don't draw the ball if we are placing it. We will change the position and
    // end up with 2 balls in different positions.
    if (!m_placeBallMode)
    {
      GetBall()->Draw();
    }

    // Get 3D position from 2D cursor pos

/*
old: read depth from depth buffer!
    GLint viewport[4]; 
    glGetIntegerv(GL_VIEWPORT,viewport);
    y=viewport[3]-y;   
    GLdouble modelview[16],projection[16];
    GLfloat wx=x,wy;
    static GLfloat wz = 0;
    glReadPixels(x,y,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&wz);
    wy=y;
    glGetDoublev(AmjuGL::AMJU_MODELVIEW_MATRIX_MATRIX,modelview);
    glGetDoublev(AmjuGL::AMJU_PROJECTION_MATRIX_MATRIX,projection);
    double ox, oy, oz;
    gluUnProject(wx,wy,wz,modelview,projection,viewport,&ox,&oy,&oz);
//std::cout << "SET UP SHOT: WZ: " << wz << "\n";
*/

// New: unproject at depth 0 and depth 1, giving two points on a line. Find intersection of line and
// plane of table top, y=<table height>, giving (x, z) coord

    Vec3f nearplane, farplane;
    Vec2f mouse((float)Mouse::s_mousex, (float)Mouse::s_mousey); 
    // Convert to -1..1 
    mouse.x = (float)mouse.x / (float)Screen::X() * 2.0f - 1.0f;
    mouse.y = 1.0f - (float)mouse.y / (float)Screen::Y() * 2.0f; // invert y
    Assert(mouse.x >= -1.0f);
    Assert(mouse.x <= 1.0f);
    Assert(mouse.y >= -1.0f);
    Assert(mouse.y <= 1.0f);
 
    Unproject(mouse, 0, &nearplane);
    Unproject(mouse, 1.0f, &farplane); // TODO These might be the wrong way round 
    // Given the y value for the table, Work out value for t in parametric line eq.
    float tableY = 10.0f; // TODO TEMP TEST - how to get real table height??
    float t = (tableY - nearplane.y) / (farplane.y - nearplane.y);
    Assert(t >= 0);
    Assert(t <= 1.0f); // table should be somewhere between near and far!
    Vec3f newpos = LineSeg(nearplane, farplane).GetPoint(t); 

    // store mouse position for moving cue in birds-eye mode
    // TODO Don't allow cue movement in birds eye mode
    m_mousePos = newpos; 

    // If in place ball mode, we move the ball to the coords pointed to.
    if (m_placeBallMode && m_drag)
    {
      Orientation o = *(GetBall()->GetOrientation());
      // TODO Test for collisions
      // Test the height at the new position. This covers intersecting the rails,
      // or being placed in a hole or off the table.
      float x = newpos.x;
      float y = o.GetY();
      float z = newpos.z;
      // Get highest height poly at the new (x, z) coords. 
      const HeightPoly* pHp = GetBall()->GetHeightServer()->GetHeightPoly(x, y + 10.0f, z);
      // If the height is the same as in the old spot, it's ok.
      static const float radius = GetBall()->GetBoundingSphere()->GetRadius();
      // NOT static, this may change between rooms/levels!
      const float tableheight = y - radius;

#ifdef SET_UP_SHOT_DEBUG
if (!pHp)
{
std::cout << "PLACE BALL: No height poly\n";
}
#endif

#ifdef TABLE_HEIGHT_DEBUG
if (pHp)
{
std::cout << "PLACE BALL: height is " << pHp->GetY(x, z) 
  << " - tableheight is " << tableheight<< "\n";
}
#endif

      if (pHp)
      {
        float dh = pHp->GetY(x, z) - tableheight; 
        if (fabs(dh) < 0.001f)
        {
          o.SetX(x);
          o.SetZ(z);
          //Orientation old = *(GetBall()->GetOrientation());
          //GetBall()->SetOrientation(o);

#ifdef SET_UP_SHOT_DEBUG
std::cout << "Place ball: new pos: " << ToString(newpos).c_str() << "\n";
#endif

          if (CueBallPosOk(newpos))
          {
            Orientation o = *(GetBall()->GetOrientation());
            newpos.y = o.GetY(); // ball sunk into table
            o.SetVertex(newpos);
            GetBall()->SetOrientation(o);
#ifdef POOL_ONLINE
        ShotLog("place cue ball 6");
#endif
          }
        }
        else
        {
#ifdef SET_UP_SHOT_DEBUG
std::cout << "Place ball: height no good ? \n";
#endif
        }
      }
    }
  }
  
  // Scene display lists may be stored in a sorted list. 
//  LeafData::GetRenderer()->DrawList();

  DrawBigText();

  AmjuGL::Enable(AmjuGL::AMJU_BLEND);
  DrawGameObjectShadows();
  AmjuGL::Disable(AmjuGL::AMJU_BLEND);

  DrawGameObjects(false); // false => non-translucent

  // Execute GameObject display lists.
//  LeafData::GetRenderer()->DrawList();

  // POOL: Draw the cue IF user is the active player
  if (!m_placeBallMode && IsUserControlled())
  {
    s_cue.Draw();
  }

  AmjuGL::Enable(AmjuGL::AMJU_BLEND);
  DrawGameObjects(true); // true => translucent, or with transparent areas.
  AmjuGL::Disable(AmjuGL::AMJU_BLEND);

  if (GetShowTrajectory() &&
      !m_placeBallMode  // don't draw when placing ball
     ) // && m_shotIsActive ?
  {
    s_pTrajectory->Draw();
  }
}

void DrawKeys()
{
/*
  static TexturedQuad tqKeyBlank;
  static TexturedQuad tqKeyArrowUp;
  static TexturedQuad tqKeyArrowDown;
  static TexturedQuad tqKeyArrowLeft;
  static TexturedQuad tqKeyArrowRight;
  static TexturedQuad tqPlayShot;
  static TexturedQuad tqCamera;

  static bool first = true;
  if (first)
  {
    first = false;

    tqKeyBlank.Load("key-blank.png", "key-a.png");
    tqKeyArrowUp.Load("key-up.png", "key-a.png");
    tqKeyArrowDown.Load("key-down.png", "key-a.png");
    tqKeyArrowLeft.Load("key-left.png", "key-a.png");
    tqKeyArrowRight.Load("key-right.png", "key-a.png");
    tqPlayShot.Load("play-red.png", "pool-exit-a.png");
    tqCamera.Load("camera-red.png", "pool-exit-a.png");
  }
  static const float keySize = Engine::Instance()->GetConfigFloat("pool-gui-key-size");
  static const float keyLeft = Engine::Instance()->GetConfigFloat("pool-gui-key-left");
  static const float keyTop = Engine::Instance()->GetConfigFloat("pool-gui-key-top");

  // Draw play button and camera
  static const float size = Engine::Instance()->GetConfigFloat("pool-gui-big-size");
  static const float camtop = Engine::Instance()->GetConfigFloat("pool-gui-big-cam-top");
  static const float playtop = Engine::Instance()->GetConfigFloat("pool-gui-big-play-top");
////  tqPlayShot.Draw(playtop, keyLeft, playtop + size, keyLeft + size);
  tqCamera.Draw(camtop, keyLeft, camtop + size, keyLeft + size); 

  // Draw arrow key cluster
  tqKeyArrowUp.Draw(keyTop, keyLeft + 1.0f * keySize, keyTop + 1.0f * keySize, keyLeft + 2.0f * keySize); // t l b r
  tqKeyArrowDown.Draw(keyTop + 1.0f * keySize, keyLeft + 1.0f * keySize, keyTop + 2.0f * keySize, keyLeft + 2.0f * keySize); // t l b r
  tqKeyArrowLeft.Draw(keyTop + 0.5f * keySize, keyLeft, keyTop + 1.5f * keySize, keyLeft + keySize); // t l b r
  tqKeyArrowRight.Draw(keyTop + 0.5f * keySize, keyLeft + 2.0f * keySize, keyTop + 1.5f * keySize, keyLeft + 3.0f * keySize); // t l b r

  // Draw zoom keys
  static float zoomintop = Engine::Instance()->GetConfigFloat(
    "pool-gui-zoomintop"); 
  static float zoominleft = Engine::Instance()->GetConfigFloat(
    "pool-gui-zoominleft");
  static float zoomouttop = Engine::Instance()->GetConfigFloat(
    "pool-gui-zoomouttop");
  static float zoomoutleft = Engine::Instance()->GetConfigFloat(
    "pool-gui-zoomoutleft");

  tqKeyBlank.Draw(zoomintop, zoominleft, zoomintop + keySize, zoominleft + keySize);
  tqKeyBlank.Draw(zoomouttop, zoomoutleft, zoomouttop + keySize, zoomoutleft + keySize);

  // Draw contact pos key
  static float contacttop = Engine::Instance()->GetConfigFloat("pool-gui-contact-key-top");
////  tqKeyBlank.Draw(contacttop, keyLeft, contacttop + keySize, keyLeft + keySize);

  // Draw play key - i.e. swing cue
  static float playkeytop = Engine::Instance()->GetConfigFloat("pool-gui-play-key-top");
////  tqKeyBlank.Draw(playkeytop, keyLeft, playkeytop + keySize, keyLeft + keySize);
  
  // Draw letters
  static const float lox = Engine::Instance()->GetConfigFloat(
    "pool-gui-letter-offset-x");
  static const float loy = Engine::Instance()->GetConfigFloat(
    "pool-gui-letter-offset-y");
  
  Engine::Instance()->PushColour(0.2f, 0.2f, 0.2f, 1.0f);
////  Engine::Instance()->GetTextWriter()->Print(keyLeft + lox, contacttop + loy, "a");
  Engine::Instance()->GetTextWriter()->Print(zoominleft + lox, zoomintop + loy, "z");
  Engine::Instance()->GetTextWriter()->Print(zoomoutleft + lox, zoomouttop + loy, "x");
////  Engine::Instance()->GetTextWriter()->Print(keyLeft + lox, playkeytop + loy, "s");
  Engine::Instance()->PopColour();
*/
}

//#define SHOW_CAMERA_INFO

void EngineStatePoolSetUpShot::DrawOverlays()
{
  EngineStatePoolBase::DrawOverlays();

  // Draw undo/redo buttons. 
  if (Engine::Instance()->GetGameState()->IsFriendly())
  {
    m_pUndoButton->SetEnabled(TheSnapshotManager::Instance()->CanUndo());
    m_pUndoButton->Draw();
    m_pRedoButton->Draw();
  }

  // If swinging the cue, show power guage
  if (s_cue.IsSwingMode()) 
  {
    m_pPowerGuage->Draw();
  }

  DrawHudBall();

  if (GetPlaceBallMode())
  {
    // Just show "done" button
    m_pPlaceBallDoneButton->Draw();
  }
  else
  {
    // Allow user to see paths even if not a friendly game.
    // But not an online game
    if (!IsOnlineGame())
    {
      m_pTrajectoryButton->Draw();
    }

    m_pHelpButton->Draw();
    m_pMenuButton->Draw();

    // Place cue ball button 
    m_pPlaceBallButton->Draw();
    // Camera mode button
    m_pCameraButton->Draw();

    // GUI for setting cue/ball contact pos.
    m_pContactPoolGui->Draw();

#ifdef SHOOT_BUTTON
    m_pShootButton->Draw();
#endif
  }

  // Show number of players online
  if (false) // TODO
  {
    PoolFont* pFont = Engine::Instance()->GetTextWriter()->GetDefaultFont();
    float s = pFont->GetSize();
    pFont->SetSize(s * 0.75f);
    Engine::Instance()->PushColour(0, 0, 0, 1.0f);
    pFont->PrintNoBlend(2.0f, 0.1f, m_playersOnlineStr.c_str());
#ifdef POOL_ONLINE
    pFont->PrintNoBlend(2.0f, 0.6f, ThePoolOnlineManager::Instance()->GetLastStatusMsg().c_str());
#endif
    Engine::Instance()->PopColour();
    pFont->SetSize(s);
  }

#ifdef POOL_ONLINE_BETA_TEST
  // Debug stuff: show game info for investigating bad games
  if (IsOnlineGame())
  {
    Font* pFont = Engine::Instance()->GetTextWriter()->GetDefaultFont();
    float s = pFont->GetSize();
    pFont->SetSize(s * 0.75f);
    Engine::Instance()->PushColour(0, 0, 0, 1.0f);

    // This is possibly useful even for release ?
    std::string str = "Opponent disconnect time: ";
    int secs = ThePoolOnlineManager::Instance()->OpponentDisconnectTime();
    str += TimePeriod(secs).ToString();
    pFont->PrintNoBlend(2.0f, 2.5f, str.c_str());

    // This is debug/beta only info
    str = "Game ID: ";
    str += ToString(ThePoolOnlineManager::Instance()->GetGameId());
    str += " Player ID: ";
    str += ToString(ThePoolOnlineManager::Instance()->GetPlayerId());
    str += " Opponent ID: ";
    str += ToString(ThePoolOnlineManager::Instance()->GetOpponentId());
    str += " Shot: ";
    str += ToString(ThePoolOnlineManager::Instance()->GetShotId());
    pFont->PrintNoBlend(2.0, 3.25f, str.c_str());

    Engine::Instance()->PopColour();
    pFont->SetSize(s);
  }
#endif

#if defined(SHOW_CAMERA_INFO)
  std::string s = "Camera: or: ";
  s += ToString(*(GetCamera()->GetOrientation()));
  s += " look at: ";
  s += ToString(GetCamera()->GetLookAtPos());
  GetEngine()->GetTextWriter()->Print(0, 15, s.c_str());
#endif

#if defined(_DEBUG) && defined(SHOW_PLATFORM_INFO)
  std::string s2 = "Platform: ";
  if (GetBall()->GetActivePlatform())
  {
    s2 += "ACTIVE" + ToString(GetBall()->GetActivePlatform()->GetId());
  }
  else
  {
    s2 += "None";
  }
  GetEngine()->GetTextWriter()->Print(0, 14, s2.c_str());

  std::string s3 = "Player platform: ";
  if (GetActivePlayer()->GetActivePlatform())
  {
    s3 += "ACTIVE"  + 
      ToString(GetActivePlayer()->GetActivePlatform()->GetId());
  }
  else
  {
    s3 += "None";
  }
  GetEngine()->GetTextWriter()->Print(0, 15, s3.c_str());

#endif

#ifdef SHOW_HELP_TEXT
  // TODO - Intl8n, different help text, timed.
  // Show help if this is the first course.
  if (IsUserControlled() && !GetPlaceBallMode())
  {
    if (m_showHelpText)
    {
      GetEngine()->GetTextWriter()->PrintCentre(11.0f, 
        m_helpText[m_helpId].c_str());

      m_helpTimer += Engine::Instance()->GetDeltaTime();
      if (m_helpTimer > m_maxHelpTime)
      {
        m_helpTimer = 0;
        m_helpId++;
        if (m_helpId >= (int)m_helpText.size())
        {
          m_helpId = 0;
        }
      }
      // Display button to close the help
      // TODO
      m_pCloseHelp->Draw();
    }
  }
#endif // SHOW_HELP_TEXT

  // If in place ball mode, tell user if ball can be placed anywhere
  // or behind headstring only ("kitchen" ??)
  Rules* pRules = GetRules(m_pLevel.GetPtr());
  if (GetPlaceBallMode())
  {
    float y = Engine::Instance()->GetConfigFloat("place_ball_text_y");
    if (pRules->PlayerMayPlaceCueBallBehindHeadstring() &&
        !pRules->PlayerMayPlaceCueBall() &&
        NonHeadAreaExists() )
    {
      Engine::Instance()->GetTextWriter()->PrintCentre(y, "You may place the cue ball anywhere behind the headstring.");
    }
    else
    {
      Assert(pRules->PlayerMayPlaceCueBall() || !NonHeadAreaExists() );
      Engine::Instance()->GetTextWriter()->PrintCentre(y, "You may place the cue ball anywhere on the table.");
    }
  }

#ifdef POOL_ONLINE
  if (IsOnlineGame())
  {
    float y = Engine::Instance()->GetConfigFloat("place_ball_text_y");
    // Move this text down if we are drawing the text above.
    if (GetPlaceBallMode())
    {
      y += 0.8f; // TODO CONFIG
    }
    Engine::Instance()->GetTextWriter()->PrintCentre(y,
      ThePoolOnlineManager::Instance()->GetLastStatusMsg().c_str());
  }
#endif
}

void EngineStatePoolSetUpShot::SetActive(bool active)
{
  m_pLevel = LevelServer::Instance()->GetCurrentLevel();

  m_shotIsActive = false;
  m_newMouseDown = false;
  m_placeBallMode = false;

  // Reset the cue contact pos.
  // TODO Check that the contact pos does not cause the cue to intersect
  // balls or table. If it does, raise the cue elevation and contact pos.
  m_pContactPoolGui->ResetContactPos();
  s_cue.Reset();
  if (s_cue.SetContactPos(0, 0)) // TODO find a valid position
  {
    // TODO

#ifdef SET_UP_SHOT_DEBUG
std::cout << "CONTACT POS: cue intersects, need to raise elev. and contact pos.\n";
#endif
  }

  if (active)
  {
#ifdef POOL_ONLINE
    ShotLog("**************** NEW SHOT ****************");
#endif

    // For non-birds eye view, the camera target is always the cue ball.
    GetCamera()->SetPlayer(GetBall());

    // Start off in 3rd-person view....?
    EngineStatePoolBase::SetBirdsEye(false);

    // POOL: Find out from Rules if the player is allowed to place the cue ball.
    // If so go to "place ball" mode.
    Rules* pRules = GetRules(m_pLevel.GetPtr());
    bool place = pRules->PlayerMayPlaceCueBall() || 
                 pRules->PlayerMayPlaceCueBallBehindHeadstring();

    if (place && 
        IsUserControlled() && 
        !pRules->ShotIsBreak())
    {
      SetPlaceBallMode(true);
    }
    // in addition to: 
    m_pPlaceBallButton->SetEnabled(place); 

#ifdef SET_UP_SHOT_DEBUG
if (place)
{
  std::cout << "PLACE BALL MODE is allowed.\n";
}
else
{
  std::cout << "PLACE BALL MODE is NOT allowed.\n";
}
#endif

    CreatePlayerName(GetEngine()->GetGameState()->GetCurrentPlayerInfo()->m_name);

    s_pTrajectory->Clear();

    GetActivePlayer()->SetState(UNKNOWN);
    int roomId = m_pLevel->GetRoomId();

#ifdef SET_UP_SHOT_DEBUG
std::cout << "SET ACTIVE: Room ID: " << roomId << "\n";
#endif

    GetEngine()->GameObjectChangeRoom(GetActivePlayer()->GetId(), roomId);
    GetEngine()->GameObjectChangeRoom(GetBall()->GetId(), roomId);

    SetStartPositions();

    // Avoidance: Restore players to their original positions 
    int numPlayers = GetEngine()->GetGameState()->GetNumberOfPlayers();

    Assert((int)s_avoidVec.size() >= numPlayers);
    for (int i = 0; i < numPlayers; i++)
    {  
      int id = GetEngine()->GetGameState()->GetPlayerInfo(i)->m_id;
      PPoolGameObject pGo = GetEngine()->GetGameObject(id);

      // While we are here, make sure Engine list of objects has this
      // player character in the right room
      // This fixes #15
      Engine::Instance()->GameObjectChangeRoom(pGo->GetId(), m_pLevel->GetRoomId());
      pGo->SetState(UNKNOWN);
      ((PoolCharacter*)pGo.GetPtr())->SetAlpha(1.0f);
      
      // Reset the avoid vec for this player.
      s_avoidVec[i] = Vec3f();
    }

    Orientation ballOr = *(GetBall()->GetOrientation()); // POOL
    // POOL
    // Adjust y-coord so ball centred at origin does not sit halfway through
    // the floor.
    float radius = GetBall()->GetBoundingSphere()->GetRadius();
    ballOr.SetY(ballOr.GetY() + radius); // + 0.1f); ?
   
    GetBall()->SetFalling(false);
    GetBall()->SetVerticalVel(0);
    GetBall()->SetForwardAccel(0);
    GetBall()->SetForwardVel(0);
    GetBall()->SetYRotateVel(0);
    float bounce = GetEngine()->GetConfigFloat("golf_ball_bounce");
    GetBall()->SetBounceDampen(bounce);
    GetBall()->SetState(UNKNOWN);

    // Set active platforms for game objects
    SetUpPlatforms();

    // If not user controlled, set the AI for the NPC.
    // NB This has moved to before the call to FaceTarget.
    // This is so we can work out the shot direction and then
    // turn to face the direction.
    if (IsUserControlled())
    {
      // POOL
      // Hide the active player character, so we can see the cue and table ?
      // Or set translucent ??
      int id = GetEngine()->GetGameState()->GetCurrentPlayerInfo()->m_id;
      PPoolGameObject pGo = GetEngine()->GetGameObject(id);
      ((PoolCharacter*)pGo.GetPtr())->SetAlpha(0);
    }
    else
    {
      GetEngine()->SetLetterbox(true);
      // Get the currently active NPC
      int id = GetEngine()->GetGameState()->GetCurrentPlayerInfo()->m_id;
      PPoolGameObject pGo = GetEngine()->GetGameObject(id);
      ((PoolCharacter*)pGo.GetPtr())->SetAlpha(1.0f);

      GetRules(m_pLevel.GetPtr())->GetBehaviour()->SetNpc((CharacterGameObject*)pGo.GetPtr());
    }
#ifdef SET_UP_SHOT_DEBUG
std::cout << "Activate AI\n";
#endif
    // Set up rails etc, useful whether AI or user.
    GetRules(m_pLevel.GetPtr())->GetBehaviour()->OnActivated();

#ifdef POOL_ONLINE
    if (IsOnlineGame())
    {
      // Poll server for opponent shot - only if we are 
      //  not taking the shot!
      if (IsUserControlled())
      {
std::cout << "Is user controlled.. setting status msg\n";

        ThePoolOnlineManager::Instance()->SetLastStatusMsg("Waiting for you to play...");
      }
      else
      {
std::cout << "Not user controlled ? WTF ?\n";

        m_onlineBehaviour.OnEsActivated();
      }
    }
#endif

    // Face the object ball, from the opposite side of the cue ball.
    PoolGameObject* pTargetToFace = 0;

#ifdef POOL_ONLINE
    //if (IsNonLocalPlayer())
    //{
      // Don't face the ghost ball, just face in the shot direction.
      // But we don't know the shot direction yet!
    //}
    //else
#endif

    if (!IsUserControlled())
    {
      if (GetRules(m_pLevel.GetPtr())->PlayerMayPlaceCueBall() &&
          !GetRules(m_pLevel.GetPtr())->PlayerMayPlaceCueBallBehindHeadstring() &&
          !GetRules(m_pLevel.GetPtr())->ShotIsBreak())
      {
        Vec3f vCue = GetRules(m_pLevel.GetPtr())->GetBehaviour()->
          GetPlaceCueBallPos();
        Orientation o = *(GetBall()->GetOrientation());
        o.SetVertex(vCue);
        GetBall()->SetOrientation(o);
#ifdef POOL_ONLINE
        ShotLog("place cue ball 1");
#endif
      }
      // For AI (+ non-local human ?) we aim at the ghost ball, or a point
      //  on a rail.
      pTargetToFace = GetRules(m_pLevel.GetPtr())->GetBehaviour()->GetAimTarget();
    }
    else
    {
      // Reuse the AI Behaviour code to work out the object to aim for.
      pTargetToFace = GetRules(m_pLevel.GetPtr())->GetBehaviour()->GetTargetObject();
      Assert(pTargetToFace);
#ifdef SET_UP_SHOT_DEBUG
std::cout << "***AIM AT TARGET: " << pTargetToFace->GetId() << "\n";
#endif
    }
 
    // Make the ball face the target object (ball or point on rail etc)
    // This _is_ needed for pool
    FaceTarget(pTargetToFace);

    Orientation o = *(GetBall()->GetOrientation());
    if (!IsBirdsEye())
    {
      if (pTargetToFace && IsUserControlled())
      {
#ifdef SET_UP_SHOT_DEBUG
std::cout << "CAMERA: Setting position to see cue ball and object ball\n";
#endif

        // Look at the cue ball, from a position on the opposite side
        // of the cue ball to the object ball.
        Vec3f v = o.GetVertex() - pTargetToFace->GetBoundingSphere()->GetCentre();
        v.Normalise();
        // TODO NB +180 required to pull back in the required direction
        o.SetYRot(180.0f + RadToDeg(atan2(v.x, v.z)));
        // POOL
        // Change the direction the cue ball is facing. This will change
        // the direction the camera pulls back.
        GetBall()->SetOrientation(o);
#ifdef POOL_ONLINE
        ShotLog("place cue ball 2");
#endif
      }
      // If AI is taking shot, set camera pos so it's not stuck behind/inside
      // the character.
      if (!IsUserControlled())
      {
        Orientation o = *(GetActivePlayer()->GetOrientation());
        Orientation o2 = *(GetBall()->GetOrientation());
        o2.SetYRot(o.GetYRot() + 180.0f);
        GetBall()->SetOrientation(o2);
#ifdef POOL_ONLINE
        ShotLog("place cue ball 3");
#endif
      }
      GetCamera()->SetOrientation(*(GetBall()->GetOrientation()));
    }
    GetCamera()->Reset();
    ((ThirdPersonCamera2*)GetCamera().GetPtr())->QuickSwingBehind();
    ((PoolCamera*)GetCamera().GetPtr())->SetPoolPullBackRequired(false);

    GetActivePlayer()->SetActionReady(); 

    SetShotPower(0.5f); // Does this have any effect ??

    GetActivePlayer()->SetState(UNKNOWN);

    // Make all characters look at the cue ball.
    // This _is_ required for Pool
    AllFaceCueBall();

    Engine::Instance()->GetGameState()->GetCurrentPlayerInfo()->
      m_golfStroke.m_cueElevationDegs = 5.0f; // TODO default position
  }
  else
  {
    // Going inactive
    GetEngine()->SetLetterbox(false);
  }
}

void EngineStatePoolSetUpShot::SetUpPlatforms()
{
  EngineRunningBase::SetUpPlatforms();

/*
  if (GetActivePlayer()->GetActivePlatform())
  {
#ifdef SET_UP_SHOT_DEBUG
    std::cout << "Player is on platform\n";
#endif
  }

  GetActivePlayer()->SetActivePlatform(GetBall()->GetActivePlatform());
*/
}

bool EngineStatePoolSetUpShot::Load()
{
  EngineStatePoolBase::Init();

  EngineStatePoolBase::Load();

  // Load power guage
  m_pPowerGuage = new PowerGuage;
  m_pPowerGuage->SetVertical(true);
//  m_pPowerGuage->SetAnchor(1.0f);

  File ftimer;
  if (!ftimer.OpenRead(Engine::Instance()->GetConfigValue("powertimer")))
  {
    GetEngine()->ReportError("Bad timer file.");
    return false;
  }
  if (!m_pPowerGuage->Load(&ftimer))
  {
    GetEngine()->ReportError("Failed to load timer.");
    return false;
  }

  m_maxShotSpeed = GetEngine()->GetConfigFloat("golf_max_shot_speed");
  m_shotAccel = Engine::Instance()->GetConfigFloat("golf_ball_accel");

  // Arrow above player showing shot direction
  std::string arrowSolid = GetEngine()->GetConfigValue("golf_arrow_solid");
  m_pArrow = SolidComponent::LoadSolid(arrowSolid);

  // "Go" button => show flyby from hole to player
  m_pPlaceBallButton = new PoolGuiButton;
  std::string goButtonFile = GetEngine()->GetConfigValue("golf_go_button");
  if (!m_pPlaceBallButton->Load(goButtonFile))
  {
    Engine::Instance()->ReportError("Failed to load place ball button.");
    return false;
  }
  static const float BUTTON_SIZE = 1.5f;
  static const float BUTTON_SPACE = 1.75f;
  static const float BUTTON_LEFT = 22.0f;
  static const float BUTTON_TOP = 9.0f;

  SetButtonLayout(m_pPlaceBallButton, "placeball");
//  m_pPlaceBallButton->SetSize(BUTTON_SIZE, BUTTON_SIZE);
//  m_pPlaceBallButton->SetRelPos(BUTTON_TOP + 2.0f * BUTTON_SPACE, BUTTON_LEFT);
  m_pPlaceBallButton->SetCommand(&OnPlaceBall);

  // Menu button
  m_pMenuButton = new PoolGuiButton;
  std::string menuButtonFile = GetEngine()->GetConfigValue("golf_menu_button");
  if (!m_pMenuButton->Load(menuButtonFile))
  {
    return false;
  }
  SetButtonLayout(m_pMenuButton, "pause");
//  m_pMenuButton->SetSize(BUTTON_SIZE, BUTTON_SIZE); // w, h
//  m_pMenuButton->SetRelPos(BUTTON_TOP + 1.0f * BUTTON_SPACE, BUTTON_LEFT);
  m_pMenuButton->SetCommand(&OnMenuButton);

  // Help button
  m_pHelpButton = new PoolGuiButton;
  std::string helpButtonFile = GetEngine()->GetConfigValue("help_button");
  if (!m_pHelpButton->Load(helpButtonFile))
  {
    return false;
  }
  SetButtonLayout(m_pHelpButton, "help");
//  m_pHelpButton->SetSize(BUTTON_SIZE, BUTTON_SIZE); // w, h
//  m_pHelpButton->SetRelPos(BUTTON_TOP + 0.0f * BUTTON_SPACE, BUTTON_LEFT);
  m_pHelpButton->SetCommand(&OnHelpButton);

  m_pCameraButton = new PoolGuiButton; 
  std::string cameraFile = GetEngine()->GetConfigValue("golf_camera_file");
  if (!m_pCameraButton->Load(cameraFile))
  {
    ReportError("Failed to load camera button.");
    return false;
  }
  SetButtonLayout(m_pCameraButton, "birdseye");
//  m_pCameraButton->SetSize(BUTTON_SIZE, BUTTON_SIZE); // same as Binocs
//  m_pCameraButton->SetRelPos(BUTTON_TOP + 3.0f * BUTTON_SPACE, BUTTON_LEFT);
  m_pCameraButton->SetCommand(&OnCamera); 

  // Practice buttons
  static const float PRAC_BTN_X = Engine::Instance()->GetConfigFloat(
    "prac_btn_x");
  static const float PRAC_BTN_Y = Engine::Instance()->GetConfigFloat(
    "prac_btn_y");
  float mult = 0.6666f;
  static const float PRAC_BTN_SIZE = BUTTON_SIZE * mult; 
  static const float PRAC_BTN_SPACE = BUTTON_SPACE * mult;

  m_pUndoButton = new PoolGuiButton;
  std::string undoFile = GetEngine()->GetConfigValue("pool_undo_button");
  if (!m_pUndoButton->Load(undoFile))
  {
    ReportError("Failed to load undo button.");
    return false;
  }
  SetButtonLayout(m_pUndoButton, "undo");
//  m_pUndoButton->SetSize(2.0f * PRAC_BTN_SIZE, PRAC_BTN_SIZE);
//  m_pUndoButton->SetRelPos(PRAC_BTN_Y, PRAC_BTN_X + 4.0f * PRAC_BTN_SPACE); 
  m_pUndoButton->SetCommand(&OnUndo);
 
  m_pRedoButton = new PoolGuiButton;
  std::string redoFile = GetEngine()->GetConfigValue("pool_redo_button");
  if (!m_pRedoButton->Load(redoFile))
  {
    ReportError("Failed to load redo button.");
    return false;
  }
  SetButtonLayout(m_pRedoButton, "redo");
//  m_pRedoButton->SetSize(2.0f * PRAC_BTN_SIZE, PRAC_BTN_SIZE);
//  m_pRedoButton->SetRelPos(PRAC_BTN_Y, PRAC_BTN_X + 6.0f * PRAC_BTN_SPACE); 
  m_pRedoButton->SetCommand(&OnRedo);

  m_pTrajectoryButton = new PoolGuiButton;
  std::string trajectoryFile = GetEngine()->GetConfigValue("pool_traj_btn");
  if (!m_pTrajectoryButton->Load(trajectoryFile))
  {
    ReportError("Failed to load trajectory toggle button.");
    return false;
  }
  SetButtonLayout(m_pTrajectoryButton, "showpath");
//  m_pTrajectoryButton->SetSize(4.0f * PRAC_BTN_SIZE, PRAC_BTN_SIZE);
//  m_pTrajectoryButton->SetRelPos(PRAC_BTN_Y, PRAC_BTN_X + 0.0f * PRAC_BTN_SPACE);
  m_pTrajectoryButton->SetCommand(&OnTrajectory);
  
  m_pShootButton = new PoolGuiButton;
#ifdef SHOOT_BUTTON
  if (!m_pShootButton->Load("pool-shoot-button.txt"))
  {
    ReportError("Failed to load shootbutton.");
    return false;
  }
  m_pShootButton->SetSize(3.0f, 3.0f);
  m_pShootButton->SetRelPos(12.0f, 1.0f);
  m_pShootButton->SetCommand(&OnShoot);
#endif

  PLAYER_OFFSET_ANGLE = Engine::Instance()->GetConfigFloat("golf_player_offset_angle");

  //TextColourizerColourList tc;
  //tc.AddColour(Colour(1.0f, 0.5f, 0.5f, 1.0f));
  //tc.AddColour(Colour(0.3f, 0.3f, 1.0f, 1.0f)); 
  ///m_pTextPower = TextFactory::Instance()->Create("power", &tc); 
  //m_pTextDirection = TextFactory::Instance()->Create("direction", &tc);
  //m_pTextGo = TextFactory::Instance()->Create("go!", &tc);

  // Load music
  // TODO This should be done by the LEVEL!
  File f;
  if (!f.OpenRead("music.txt"))
  {
    f.ReportError("Failed to open music list.");
    return false;
  }
  int numSongs = 0;
  if (!f.GetInteger(&numSongs))
  {
    f.ReportError("Expected number of songs.");
    return false;
  }
  int i;
  for (i = 0; i < numSongs; i++)
  {
    std::string s;
    if (!f.GetDataLine(&s))
    {
      f.ReportError("Expected song name.");
      return false;
    }
    m_songs.push_back(s);
  }

  // Get time to display help messages
  m_maxHelpTime = Engine::Instance()->GetConfigFloat("golf_help_time");

  // TODO Platform-specific help text
  std::string helpFile = Engine::Instance()->GetConfigValue("golf_help_file_mac1");
/*
  // Get the messages
  // This default file is for mac and windows, 3 button mouse
  std::string helpFile = Engine::Instance()->GetConfigValue("golf_help_file");
  // If mac with one-button mouse, load this file
#ifdef MACOSX
  // TODO Check for one-button mouse
  helpFile = Engine::Instance()->GetConfigValue("golf_help_file_mac1");
#endif
*/
  File f2;
  // Prepend language directory.
  std::string lang = Engine::Instance()->GetConfigValue("language");
  std::string dash = "-"; 
  std::string langFileName = lang + dash + helpFile;

  if (!f2.OpenRead(langFileName))
  {
    f2.ReportError("Couldn't open file.");
    return false;
  }
  int numMessages = 0;
  if (!f2.GetInteger(&numMessages))
  {
    f2.ReportError("Expected number of help lines.");
    return false;
  } 
  for (i = 0; i < numMessages; i++)
  {
    std::string s;
    if (!f2.GetDataLine(&s))
    {
      f2.ReportError("Expected help message.");
      return false;
    }
    m_helpText.push_back(s);
  }
  // Close Help button
  m_pCloseHelp = new PoolGuiButton;
  std::string closeButtonFile = 
    GetEngine()->GetConfigValue("golf_close_help");
  if (!m_pCloseHelp->Load(closeButtonFile))
  {
    f2.ReportError("Failed to load close help button.");
    return false;
  }
  m_pCloseHelp->SetSize(6.0f, 0.75f);
  m_pCloseHelp->SetRelPos(11.8f, 9.0f);
  m_pCloseHelp->SetCommand(&OnCloseHelpButton);

/*
  // Load cross on ball
  m_pBallCross = SolidComponent::LoadSolid(Engine::Instance()->GetConfigValue(
    "ball_cross_bb"));
  if (!m_pBallCross.GetPtr())
  {
    return false;
  }
*/

  // POOL: Load cue ball contact gui.
  m_pContactPoolGui = new PoolGuiContactPos;
  if (!m_pContactPoolGui->Load())
  {
    ReportError("Failed to load contact GUI.");
    return false;
  }

  // Load place ball "done" button
  m_pPlaceBallDoneButton = new PoolGuiButton;
  if (!m_pPlaceBallDoneButton->Load("place-ball-done-button.txt"))
  {
    ReportError("Failed to load place ball done button");
    return false;
  }
  SetButtonLayout(m_pPlaceBallDoneButton, "placeballdone");
//  m_pPlaceBallDoneButton->SetSize(3.0f, 1.5f);
//  m_pPlaceBallDoneButton->SetRelPos(
//    BUTTON_TOP + 2.0f * BUTTON_SPACE + 1.0f, BUTTON_LEFT - 1.0f); // TODO CONFIG
  m_pPlaceBallDoneButton->SetCommand(&OnPlaceBall); 

  return true;
}

void EngineStatePoolSetUpShot::Clear()
{
}

void PlayerMoveAwayFrom(PoolGameObject* p1, PoolGameObject* p2)
{
  // p2 is stationary. We must move p1 away from p2 so they
  // do not intersect.
  // We must have a notional move away velocity.
  static const float MOVE_AWAY_VEL = 2.0f;
  const float dt = Engine::Instance()->GetDeltaTime();
  // So, we can move away this much.
  float dist = dt * MOVE_AWAY_VEL;

  // Get a good direction. 
  // Ideally we want to move directly away from p2.
  // TODO
  // yRads is direction we want to go in.
  // NB The character doesn't face this direction, we just change the
  // position in this direction.
  Vec3f v1 = p1->GetOrientation()->GetVertex();
  Vec3f v2 = p2->GetOrientation()->GetVertex();

  float yRads = AngleCompare::TurnToFace(v1.x, v1.z, v2.x, v2.z);

  bool looking = true;
  Orientation p1or = *(p1->GetOrientation());
  Orientation newOr = p1or;

// MSVC workaround
#ifndef M_PI
  static const float M_PI = pi;
#endif

  float alternativeDirection[] = 
  { 
    (float)M_PI * 0.1f, 
    (float)M_PI * 0.2f, 
    (float)M_PI * 0.3f, 
    (float)M_PI * 0.4f, 
    (float)M_PI * 0.5f, 

    (float)-M_PI * 0.1f, 
    (float)-M_PI * 0.2f,
    (float)-M_PI * 0.3f, 
    (float)-M_PI * 0.4f, 
    (float)-M_PI * 0.5f 
  };
  static const int lastAlternative = sizeof(alternativeDirection)/sizeof(float);

  int counter = 0; // count of directions we have tried so far.

  // We don't check that p1 is a VisibleGameObject - we know it's
  // always a Character.
  const HeightServer* pHs = ((VisibleGameObject*)p1)->GetHeightServer();

  while (looking)
  {
    // Get the new coords for p1
    float dx = dist * sin(yRads);
    float dz = dist * cos(yRads);

    newOr = p1or;
    newOr.SetX(newOr.GetX() + dx);
    newOr.SetZ(newOr.GetZ() + dz);
    float x = newOr.GetX();
    float z = newOr.GetZ();
    float ymax = newOr.GetY() + 1.0f;
    float ymin = newOr.GetY() - 1.0f;

    // Test the ground height at the new coords.
    // This is so we change height correctly on a slope, and also 
    // to check if we just shuffled off a cliff or something.
    // I.e. we must make sure we move somewhere where ground exists.
    bool badHeight = true;
    const HeightPoly* pHp = pHs->GetHeightPoly(x, ymax, ymin, z);
 
    // Height is ok if it is close to the original height of p1.
    if (pHp)
    {
      // Got a height poly within the requested range, so height must
      // be ok.
      // BUT we must make sure we haven't hit a wall!
      BoundingSphere before = *(p1->GetBoundingSphere());
      BoundingSphere after(before);
      after.SetCentre(newOr.GetVertex());

      typedef std::vector<WallPoly> WallVector;
      WallVector walls;
      bool collided = pHs->Intersects(before, after, &walls);
      if (!collided)
      {
        badHeight = false;
      }
    }  

    if (badHeight)
    {
      // Try a different direction for yRads. 
      if (counter >= lastAlternative)
      {
        // Give up - we can't find a safe place to move p1 to.
        return;
      }
      yRads += alternativeDirection[counter];
      ++counter;
    }
    else
    {
      // If the ground is good we can stop looking.
      // Make sure the newOr Y-value is on top of the ground.
      float y = pHp->GetY(x, z);
      newOr.SetY(y);
      looking = false;
    }
  }
  // Move p1 in the direction we have chosen.
  p1->SetOrientation(newOr);

  // Assuming this is a character, change state to walking.
  CharacterGameObject* pc = dynamic_cast<CharacterGameObject*>(p1);
  if (pc)
  {
    pc->SetActionWalk();
  }
}

void EngineStatePoolSetUpShot::UpdatePlayerPositions()
{
  // TODO TEMP TEST - players hopping up and down!
  return;
/*
  int numPlayers = GetEngine()->GetGameState()->GetNumberOfPlayers();
  if (numPlayers < 2)
  {
    return;
  }

  int activePlayer = GetActivePlayer()->GetId();

  // Update player characters so they don't intersect, if possible.
  // Get the player characters.
  std::vector<GameObject*> players;
  players.reserve(4); // TODO max no of players
  Assert(players.empty()); // NB watch out for this reserve gotcha

  int i;
  for (i = 0; i < numPlayers; i++)
  {  
    int id = GetEngine()->GetGameState()->GetPlayerInfo(i)->m_id;
    bool isAlive = GetEngine()->GetGameState()->GetPlayerInfo(i)->m_isPlaying;
    PPoolGameObject pGo = GetEngine()->GetGameObject(id);
    if (isAlive)
    {
      players.push_back(pGo.GetPtr());
    }
  }
 
  // For 1- or 2-player, one of the players should be the active one!
 
  // Nested loop.
  // For each character, find any intersecting character(s).
  // For an intersecting character, move away if there is a floor in the
  // direction you want to go.
  numPlayers = players.size(); // revised number after dead players removed
  for (i = 0; i < numPlayers; i++)
  {
    for (int j = i + 1; j < numPlayers; j++)
    {
      if (players[i]->Intersects(*(players[j])))
      {
#ifdef UPDATE_PLAYER_POS_DEBUG
std::cout << "Player " << i << " and player " << j << " intersect!\n"; 
#endif

        // If one of these is the active player, they don't move.
        if (players[i]->GetId() != activePlayer)
        {
          // Move player i away. Store the change in position so we can
          // restore it later.
          Orientation o1 = *(players[i]->GetOrientation());
          PlayerMoveAwayFrom(players[i], players[j]);
          Orientation o2 = *(players[i]->GetOrientation());
          Orientation oDiff = o2 - o1;
          Vec3f vDiff = oDiff.GetVertex();
          s_avoidVec[i] += vDiff;
        }        
        else
        {
#ifdef UPDATE_PLAYER_POS_DEBUG
std::cout << "Player " << i << " is active, so will not be moved.\n";
#endif
        }
        if (players[j]->GetId() != activePlayer)
        {
          Orientation o1 = *(players[j]->GetOrientation());
          PlayerMoveAwayFrom(players[j], players[i]);
          Orientation o2 = *(players[j]->GetOrientation());
          Orientation oDiff = o2 - o1;
          Vec3f vDiff = oDiff.GetVertex();
          s_avoidVec[j] += vDiff;
        }        
        else
        {
#ifdef UPDATE_PLAYER_POS_DEBUG
std::cout << "Player " << j << " is active, so will not be moved.\n";
#endif
        }
      }
    }
  }
*/
}

void EngineStatePoolSetUpShot::CheckForInvite()
{
#ifdef POOL_ONLINE
  // Check current games to see if we have been invited to an online game.
  const PoolOnline::Players& players = ThePoolOnlineManager::Instance()->GetPlayersOnline();
  const PoolOnline::OnlineGames& games = ThePoolOnlineManager::Instance()->GetGames();

  int thisPlayerId = ThePoolOnlineManager::Instance()->GetPlayerId();
#ifdef _DEBUG
std::cout << "CheckForInvite: This player ID: " << thisPlayerId << "\n";
#endif

  if (thisPlayerId == -1)
  {
    return;
  }

  Assert(thisPlayerId > -1);

  if (games.empty())
  {
#ifdef _DEBUG
std::cout << "CheckForInvite: No games\n";
#endif
  }
  else
  {
    // If there are any active games, there must be players, right ?
    Assert(!players.empty());
  }

  for (
    PoolOnline::OnlineGames::const_iterator it = games.begin();
    it != games.end();
    ++it)
  {
    const PoolOnline::OnlineGame& g = *it;
    std::string s;

    //const PoolOnline::OnlinePlayer& p1 =
    //  const_cast<PoolOnline::Players&>(players)[g.m_playerId[0]];

    const PoolOnline::OnlinePlayer& p2 =
      const_cast<PoolOnline::Players&>(players)[g.m_playerId[1]];

    if (g.m_playerId[0] == thisPlayerId && g.m_playerId[1] == -1)
    {
      // This is us, waiting for someone to join. Don't display.
      // Remember this game ID, because if we join another game we will
      // want to delete this unused game.
      // TODO 
      //m_unusedGameId = g.m_id;

      continue;
    }
    else if (g.m_playerId[0] == thisPlayerId && g.IsWaitingForAccept() )
    {
#ifdef _DEBUG
std::cout << "CheckForInvite ** has found an invite!! **\n";
#endif

      // Player2 has requested to join this game. We now accept or decline.
      ThePoolOnlineManager::Instance()->SetGameId(g.m_id);
      const std::string& p2name = p2.m_name;
      ((EsPoolOnlineAccept2*) Engine::Instance()->GetGameState(EsPoolOnlineAccept2::Name).GetPtr())->
        Set(thisPlayerId, g.m_playerId[1], p2name, g.m_id);
      Engine::Instance()->ChangeState(EsPoolOnlineAccept2::Name, Engine::IMMEDIATE);

      // Play a wav - we have been invited to play!
      Engine::Instance()->PlayWav("bonus_points.wav");

      return;
    }
  }
#endif // POOL_ONLINE
}

void EngineStatePoolSetUpShot::InitOnlineStatus()
{
#ifdef POOL_ONLINE
  if (!m_serverStatusSetThisGame)
  {
    // Call at start of a new NON-online game. This sets up a row in server 
    // side DB.
    // TODO only need to do this once when game starts
    std::string playerName = ThePlayerNames::Instance()->GetName(0);
    int playerId = ThePlayerNames::Instance()->GetId(0);

    ThePoolOnlineManager::Instance()->SetPlayerName(playerName, playerId, true /* get ID if not set */); 
  
    if (ThePoolOnlineManager::Instance()->GetPlayerId() != -1)
    {
      ThePoolOnlineManager::Instance()->CreateNewGame();
      m_serverStatusSetThisGame = true;
    }
  }
#endif
}

void EngineStatePoolSetUpShot::UpdateOnlineStatus()
{
#ifdef POOL_ONLINE
  Assert(!IsOnlineGame());

  // Not an online game: periodically tell the server that we are alive,
  // so other players can see us and invite us to play.
  // Then check for game invites from other online players.

  // NB If playing 2 player non-online, we don't want to do this - we are
  // already playing someone.
  if (NumHumanPlayers() == 2)
  {
    return;
  }

  InitOnlineStatus();
  if (ThePoolOnlineManager::Instance()->GetPlayerId() == -1)
  {
    return;
  }

  static const float REFRESH_PERIOD = ThePoolOnlineManager::Instance()->
    GetConstant(PoolOnline::SINGLE_PLAYER_REFRESH_PERIOD);

  static float refreshTime = 0;
  refreshTime += Engine::Instance()->GetDeltaTime();
  if (refreshTime > REFRESH_PERIOD)
  {
    refreshTime = 0;
    // Request list of players AND GAMES -- we need to know when we
    // have been invited.
    ThePoolOnlineManager::Instance()->RequestGamesListRefresh();

//      RequestAllOnlinePlayerListRefresh();
    // Can't use RequestGamesListRefresh() because it assumes we are in 
    //  Table state...?

    // Update message on screen showing number of players online
    int numPlayersOnline = ThePoolOnlineManager::Instance()->GetPlayersOnline().size();
    m_playersOnlineStr = "Number of players online: ";
    m_playersOnlineStr += ToString(numPlayersOnline); 

    // Check if our game has an invite
    CheckForInvite();
  }
#endif
}

void EngineStatePoolSetUpShot::Update()
{
  EngineStatePoolBase::Update();

#ifdef POOL_ONLINE
  // v.2.1: check server for new games even if this is
  //  a non-online game
  ThePoolOnlineManager::Instance()->Update();

  if (IsOnlineGame())
  {
    // Demo mode is NOT an online game ever!
    Assert(!IsDemoMode());

    // Check for opponent disconnected
    if (ThePoolOnlineManager::Instance()->OpponentHasDisconnected())
    {
      Engine::Instance()->ChangeState(EsPoolDisconnect::Name, Engine::IMMEDIATE);
    }
    
    // This should now NOT give a false Quit message if the opponent 
    // ends the game.
    if (ThePoolOnlineManager::Instance()->OpponentHasQuit())
    {
      // TODO Set text depending on quit/disconnect
      Engine::Instance()->ChangeState(EsPoolDisconnect::Name, Engine::IMMEDIATE);
    }
  }
  else
  {
    // Not an online game, but we can still be invited to a game
    UpdateOnlineStatus();
  }
    
#endif // POOL_ONLINE

#ifdef SHOOT_BUTTON
  if (m_pShootButton->IsSelected())
  {
    OnShootSelected();
  }
#endif

  s_cue.Update(); // Should do this in above call ?

  // TODO TEMP TEST
  // Stop swinging cue back and forth if no power, so can move L/R again
  if (m_drag)
  {
    float s = s_cue.GetSwingPower();
    if (s <= 0)
    {
      s_cue.SetSwingMode(false); 
    }
  }

  if (s_cue.HitsCueBall() && IsUserControlled())
  {
    // Take shot.
#ifdef SET_UP_SHOT_DEBUG
std::cout << "TAKING SHOT NOW with pool cue swing\n";
#endif

    m_drag = false;
    Mouse::SetCursor(Mouse::STANDARD);
    GetActivePlayer()->SetActionReady();
    m_shotIsActive = false;
    SetShotParams();

    GetEngine()->GetGameState()->GetCurrentPlayerInfo()->m_totalStrokes++;
    Engine::Instance()->ChangeState(EngineStatePoolShowShot::Name, Engine::IMMEDIATE);

    // Round the shot info so it's the same for this client and all others!
    PoolGameState::PlayerInfo* pInfo = 
      GetEngine()->GetGameState()->GetCurrentPlayerInfo();

    Round(pInfo);

    // ONLINE: Send shot info to server
    // DO THIS AFTER SHOT COMPLETES!
  }

  // If the ball is on a platform it's imposssible to click on it.
  // Try moving the camera with the ball.
/*
  Platform* pPlat = GetBall()->GetActivePlatform();
  if (pPlat)
  {
    Orientation oCam = *(GetCamera()->GetOrientation());
    Orientation oDelta;
    pPlat->GetOrientationDelta(&oDelta);
    if (oDelta.GetYRot() != 0)
    {
      // On a rotating platform
      // Copied from VisibleGameObject for handling rotating platforms..
      const float charx = GetBall()->GetOrientation()->GetX();
      const float charz = GetBall()->GetOrientation()->GetZ();

      const float platx = pPlat->GetRotationCentre().x;
      const float platz = pPlat->GetRotationCentre().z;
    
      float addx = 0;
      float addz = 0;

      RotateHelper(platx, platz, charx, charz, oDelta.GetYRot(), &addx, &addz);
      oDelta.SetX(oDelta.GetX() + addx);
      oDelta.SetZ(oDelta.GetZ() + addz);

      float yrot = oDelta.GetYRot();

      if (m_shotIsActive)
      {
        // Add rotate angle to shot trajectory
        m_shotYRotate += yrot;
        SetShotParams();
      }

      oDelta.SetYRot(0);
      oCam += oDelta;
      GetCamera()->SetOrientation(oCam);
    }
    else
    {
      // Moving platform
      oCam += oDelta;
      GetCamera()->SetOrientation(oCam);
    }
  }
*/
  // Make sure trajectory is correct on a moving platform
  // POOL: Not required
  //s_pTrajectory->SetBallPos(GetBall()->GetOrientation()->GetVertex());
  //s_pTrajectory->Update();

  if (IsUserControlled())
  {
    // Update arrow
    Orientation o = *(GetBall()->GetOrientation());
    o.SetY(o.GetY() + 2.0f);
    o.SetYRot(m_shotYRotate);
    m_pArrow->SetOrientation(o);

  }
  else
  {
    // TODO TEMP because we will have a new Engine State for waiting for
    // online opponent to play
    // Don't update AI, we don't want it to take the shot! We are
    // waiting for onlin opponent.
    if (!Engine::Instance()->GetGameState()->GetCurrentPlayerInfo()->m_isOnline)
    {
      // Update AI controlling current player.
      GetRules(m_pLevel.GetPtr())->GetBehaviour()->Update();
    }

#ifdef POOL_ONLINE
    if (IsOnlineGame())
    {
      // Poll server for opponent shot
      m_onlineBehaviour.Update();
    }
#endif
  }

  // Move players so they don't intersect.
  // NB This may be the wrong place for the call.
  UpdatePlayerPositions();
}

void EngineStatePoolSetUpShot::AllFaceCueBall()
{
  int numPlayers = GetEngine()->GetGameState()->GetNumberOfPlayers();
#ifdef ALLFACE_DEBUG
  std::cout << "AllFace: num players: " << numPlayers << "\n";
#endif
 
  Orientation o = *(GetBall()->GetOrientation());

  float playerx = o.GetX();
  float playerz = o.GetZ();

  for (int i = 0; i < numPlayers; i++)
  {
    int id = GetEngine()->GetGameState()->GetPlayerInfo(i)->m_id;

    PPoolGameObject pGo = GetEngine()->GetGameObject(id); 
 
    // Get the current player Or: we only change the yRot here.
    Orientation to = *(pGo->GetOrientation()); 

    float d = RadToDeg(AngleCompare::TurnToFace(to.GetX(), to.GetZ(), playerx, playerz));
    to.SetYRot(180.0f + d);
    pGo->SetOrientation(to); // Ok, Not putting character is stupid position
  }
}

void EngineStatePoolSetUpShot::FaceTarget(PoolGameObject* pAnotherTarget)
{
  // Rotate ball to face target - this is so the camera looks towards the
  // target from behind the ball.

  Assert(pAnotherTarget);
  Assert(m_pLevel.GetPtr());
  Assert(GetPoolHole(m_pLevel.GetPtr()));
  Assert(GetBall());
  Assert(GetBall()->GetOrientation());

  Vec3f vHole = pAnotherTarget->GetBoundingSphere()->GetCentre();

  Orientation oBall = *(GetBall()->GetOrientation());
  // Get the angle required for ball to travel towards hole
  float yRadians = AngleCompare::TurnToFace(oBall.GetX(), oBall.GetZ(), vHole.x, vHole.z );

  m_shotYRotate = RadToDeg(yRadians) + PLAYER_OFFSET_ANGLE - 10.0f;
  //static const int NUM_TRIES = 36;
  //for (int i = 0; i < NUM_TRIES; i++)
  //{

  // ?? What's this for, to set shot params ?
  AddShotAngle(10.0f);
}

void EngineStatePoolSetUpShot::SetShotParams()
{
  float shotSpeed = m_pPowerGuage->Get();
  if (shotSpeed < 0)
  {
    shotSpeed = 0;
  }
  if (shotSpeed > 1.0f)
  {
    shotSpeed = 1.0f;
  }

  // We don't want linear power, we want exponential shaped graph.
  // This is so low power shots have more control.

  // ????? OR DO WE ????
// v.2.1 Try not doing this: min power is too low
//   shotSpeed *= shotSpeed; // Exponential curve
  Assert(shotSpeed <= 1.0f);
  Assert(shotSpeed >= 0);

  m_shotHorizontalSpeed = shotSpeed * m_maxShotSpeed; 

  // *** TODO POOL ***
  // For pool, the ball almost always travels horizontally.
  // But for certain shots, the cue ball can jump - when the cue is
  // unusually high/off centre ?
  m_shotVerticalVel = 0; 
  // ***

  // Work out draw/roll/jump
  // If the cue contact pos is above or below some threshold, the cue ball
  // will jump.
  // Draw/roll is -1 for bottom, 0 for centre, +1 for top.

  float drawRoll = 0;
  float english = 0;
  m_pContactPoolGui->GetContactPos(&english, &drawRoll);
  Assert(english >= -1.0f);
  Assert(english <= 1.0f);
  Assert(drawRoll >= -1.0f);
  Assert(drawRoll <= 1.0f);
 
#ifdef DRAW_ROLL_DEBUG
std::cout << "DRAW/ROLL: " << drawRoll << "\n";
#endif
 
  static const float drawRollLimit = Engine::Instance()->GetConfigFloat("pool_drlimit");
  Assert(drawRollLimit >= 0);
  Assert(drawRollLimit < 1.0f);

  if (fabs(drawRoll) > drawRollLimit)
  {
    // Jump is proportional to difference between drawRoll and drawRollLimit.
    static const float jump = Engine::Instance()->GetConfigFloat("pool_jump");
    float p = (fabs(drawRoll) - drawRollLimit) / (1.0f - drawRollLimit);
    if (p > 1.0f)
    {
      p = 1.0f;  
    } 
    if (p < 0)
    {
      p = 0;
    }

#ifdef JUMP_DEBUG 
std::cout << "JUMP: p: " << p << "\n";
#endif

    m_shotVerticalVel = -m_shotHorizontalSpeed * jump * p; 
    // Decrease horizontal speed so ball doesn't fly away.
    static const float JUMP_HOR_SPEED_MULT = Engine::Instance()->GetConfigFloat(
      "jump_hor_speed_mult");
    m_shotHorizontalSpeed *= JUMP_HOR_SPEED_MULT;
 
    // The idea here is that if we are taking a jump shot, we shouldn't go 
    // crazy with backspin too.
    drawRoll = -0.5f;
  }

#ifdef SET_UP_SHOT_DEBUG
//std::cout << "DRAW/ROLL: " << drawRoll << " limit: " << drawRollLimit << "\n";
#endif

  // Give a bit of backspin
  if (drawRoll == 0)
  {
    drawRoll = -0.2f; // TODO CONFIG
  }

  PoolGameState::PlayerInfo* pInfo = 
    GetEngine()->GetGameState()->GetCurrentPlayerInfo();
  float elev = pInfo->m_golfStroke.m_cueElevationDegs;
  pInfo->m_golfStroke =
    PoolGameState::PlayerInfo::PoolStroke(m_shotYRotate, m_shotVerticalVel, 
    m_shotHorizontalSpeed, m_shotAccel,
    english,  
    drawRoll);  
  pInfo->m_golfStroke.m_cueElevationDegs = elev;
  // ONLINE: Set the cue ball start position
  pInfo->m_golfStroke.SetCueBallPos(GetBall()->GetOrientation()->GetVertex());

  // If friendly game, make sure trajectory is pointing the right way.
  if (GetShowTrajectory())
  {
    Vec3f vBall = GetBall()->GetOrientation()->GetVertex();
//  vBall.y += 0.25f; // make sure a float trajectory will be visible
    // POOL: shot speed is set independently of direction, so just use a dummy
    // horizontal vel.
    float hvel = 10.0f; 

    s_pTrajectory->Recalc(vBall, m_shotYRotate, 0 /*m_shotVerticalVel*/, hvel); 
  }
}

void EngineStatePoolSetUpShot::MousePos(int x, int y)
{
  static int oldx = x;
  static int oldy = y;

  int xdiff = x - oldx;
#ifdef MOUSE_DIFF_DEBUG
std::cout << "Mouse X diff: " << xdiff << "\n";
#endif

  int ydiff = y - oldy;
  oldx = x;
  oldy = y;

// TODO
//#ifdef IPHONE  
//std::cout << "Y diff: " << ydiff << "\n";
  if (ydiff > 0 && ydiff > abs(xdiff) * 2  && m_drag)
  {
//std::cout << "Dragging down\n";
    TakeShotStart();
  }
//#endif
  // Always call this so the previous mouse pos is up-to-date.
  // It only has an effect if we are in swing mode.
  s_cue.MousePos(x, y);

  if (m_drag)
  {
    // Swing cue if in swing mode.
//std::cout << "Swing power: " << s_cue.GetSwingPower() << "\n";

    SetShotPower(s_cue.GetSwingPower());

    // need this here to keep power guage steady
    if (s_shootSelected)
    {
      SetShotPower(0.5f);
    }
  }

  GetCamera()->MousePos(x, y);
  
  if (m_pContactPoolGui->IsEnabled())
  {
    // Store old contact pos in case new pos is invalid.
    float oldcpx, oldcpy;
    m_pContactPoolGui->GetContactPos(&oldcpx, &oldcpy);

    m_pContactPoolGui->MousePos(x, y);
    float cpx, cpy;
    m_pContactPoolGui->GetContactPos(&cpx, &cpy);
    // Is the new contact pos valid ? It isn't if the cue now intersects
    // the table or a ball.
    if (!s_cue.SetContactPos(-cpx, -cpy))
    {
      // New pos is bad - restore old contact pos
      m_pContactPoolGui->SetContactPos(oldcpx, oldcpy);
    }
  }

/*
  if (!m_drag)
  {
    oldx = x;
    oldy = y;
    return;
  }
*/

#ifdef WIN32
  // Windows feature: if you drag the mouse off the left side of the 
  // window, the diff is around 65535!
  if (fabs((float)xdiff) > 600)
  {
    xdiff = 0;
  }
#endif

  
  // NB Moved from above, is this ok ?
  if (!m_drag)
  {
    oldx = x;
    oldy = y;
    return;
  }

  // Try to avoid Birds Eye View cue wobble
  if (m_pCameraButton->IsSelected())
  {
    return;
  }
 
  if (m_shotIsActive)
  {
//std::cout << "Shot is active\n";

    // POOL Don't move the cue left/right/up/down if swing mode OR
    // contact pos mode
    if (!s_cue.IsSwingMode() && !m_pContactPoolGui->IsEnabled())
    {
      Mouse::SetCursor(Mouse::FINGER);

      // Cue control is different if we are in birds eye mode.
      if (IsBirdsEye())
      { 
        // Don't move cue if user is selecting a button.
        // Definitely don't rotate if we are swinging the cue!
        if (!m_pCameraButton->IsSelected() &&
            !m_pPlaceBallButton->IsSelected() &&
            !m_pMenuButton->IsSelected() &&
            !m_pHelpButton->IsSelected() &&
            !m_pUndoButton->IsSelected() &&
            !m_pRedoButton->IsSelected() &&
            !m_pTrajectoryButton->IsSelected() &&
            !s_cue.IsSwingMode() ) 
        {
          // Swing the cue around so that it's on the line from the cue
          // ball centre through the mouse cursor.
          // Elevation can't be set in this mode: just set it to some
          // default value.
        
          Vec3f vMouse = m_mousePos; // set in Draw()
          Vec3f vCue = GetBall()->GetOrientation()->GetVertex();
          vMouse.y = vCue.y;
          Vec3f vDiff = vCue - vMouse;
          float rads = atan2(vDiff.x, vDiff.z);
          float degs = RadToDeg(rads);
          m_shotYRotate = degs; 
          SetShotParams();

          Engine::Instance()->GetGameState()->GetCurrentPlayerInfo()->
            m_golfStroke.m_cueElevationDegs = 5.0f; 
        }
      }
      else
      { 
        PoolCamera* pCam = dynamic_cast<PoolCamera*>(GetCamera().GetPtr());
        Assert(pCam);

        // Rotation depends on left-right mouse movement.
        static float sensitivity = Engine::Instance()->GetConfigFloat(
          "golf_sensitivity"); 
        static const float MAX_X_DIFF = Engine::Instance()->GetConfigFloat(
          "pool_max_xdiff");
        
        float rot = (float)xdiff / MAX_X_DIFF;
        // Get exponential control - so small changes are more precise.
        float sign = 1.0f;
        if (rot < 0) sign = -1.0f;

        if (fabs(rot) > 1.0f)
        {
          rot = 1.0f;
        }
        rot *= rot; // Exponential
        rot *= sign;
        rot *= sensitivity;
        AddShotAngle(rot);
  
        // v.1.1 "fix" the camera to the cue, so it's always pointing ahead.
        static const bool fixCameraToCue = true; // TODO
        if (fixCameraToCue)
        {
          pCam->GoDirectlyBehindCueNow(m_shotYRotate);
        }

        // Cue elevation depends on up-down mouse movement.
        float diff = (float)ydiff / (float)m_maxYDiff;
        diff *= 100.0f;
 
        // If cue elevation is changing, tell the camera to follow the
        // height.
        if (ydiff != 0)
        {
          pCam->SetAdjustToCueHeight(true);
        }

#ifdef ELEV_DEBUG
std::cout << "CUE ELEVATION: diff: " << diff << "\n";
#endif

        float elev = Engine::Instance()->GetGameState()->
          GetCurrentPlayerInfo()->m_golfStroke.m_cueElevationDegs;
        static const float ELEV_SENSITIVITY = Engine::Instance()->
          GetConfigFloat("pool_elev_sensitivity");
        static const float MAX_ELEV = Engine::Instance()->
          GetConfigFloat("pool_max_elev");

        elev -= diff * ELEV_SENSITIVITY; 
        if (elev < 5.0f)
        {
          elev = 5.0f; // TODO lowest val
        }
        else if (elev > MAX_ELEV)
        {
          elev = MAX_ELEV; 
        }
        Engine::Instance()->GetGameState()->GetCurrentPlayerInfo()->
          m_golfStroke.m_cueElevationDegs = elev;
      }
    }
  }
}

void EngineStatePoolSetUpShot::MouseRButton(bool down, bool ctrl, bool shift)
{
  // TODO Control cue swing
#ifdef SET_UP_SHOT_DEBUG
std::cout << "RIGHT MOUSE BUTTON " << (down ? "down" : "up") << "\n";
#endif

  m_drag = down; // TODO do we need r-drag and l-drag ? 

  if (!IsUserControlled())
  {
    return;
  }

  // Cue swing
  if (down)
  {
    TakeShotStart();
  }
  else 
  {
    TakeShotFinish();
  }
}

void EngineStatePoolSetUpShot::MouseMidButton(bool down, bool ctrl, bool shift)
{
  // TODO Control cue contact pos
#ifdef SET_UP_SHOT_DEBUG
std::cout << "MID MOUSE BUTTON " << (down ? "down" : "up") << "\n";
#endif

  if (down)
  {
    CancelShot();
    MoveContactPosStart();
  }
  else
  {
    MoveContactPosFinish();
  }
  m_drag = down;
}

void EngineStatePoolSetUpShot::MouseButton(bool down, bool ctrl, bool shift)
{
#ifdef SET_UP_SHOT_DEBUG
std::cout << "LEFT MOUSE BUTTON " << (down ? "down" : "up") << "\n";
#endif

// TODO
//#ifdef IPHONE
  if (!down && s_cue.IsSwingMode()) // in take shot mode ?
  {
#ifdef _DEBUG
std::cout << "WHOOOAAAA! TAKING SHOT!!!!\n";
#endif
    TakeShotFinish();
    // return ???
  }
//#endif

  // Don't call if help turned off, you still get Clicked notifications
  if (m_showHelpText)
  {
    m_pCloseHelp->MouseButton(down, false, false);
  }
  if (GetPlaceBallMode())
  {
    m_pPlaceBallDoneButton->MouseButton(down, ctrl, shift);
  }
  else
  {
    m_pHelpButton->MouseButton(down, ctrl, shift);
    m_pMenuButton->MouseButton(down, ctrl, shift);
    m_pPlaceBallButton->MouseButton(down, ctrl, shift);
    m_pCameraButton->MouseButton(down, ctrl, shift);
#ifdef SHOOT_BUTTON
    m_pShootButton->MouseButton(down, ctrl, shift);
#endif
  }

  if (Engine::Instance()->GetGameState()->IsFriendly())
  {
    m_pUndoButton->MouseButton(down, false, false);
    m_pRedoButton->MouseButton(down, false, false);
  }

  if (!IsOnlineGame())
  {
    m_pTrajectoryButton->MouseButton(down, false, false);
  }

  if (!IsUserControlled())
  {
    return;
  }

  // POOL: no mouse camera control
  //GetCamera()->MouseButton(false, false, false);


  // Place ball mode: if we click on the Done button, don't place the cue
  //  ball under the mouse cursor!
  if (!GetPlaceBallMode() ||
      !m_pPlaceBallDoneButton->IsSelected())
  {
    m_newMouseDown = down; // POOL - flag set when button clicked.
    m_drag = down;
  }

#ifdef SET_UP_SHOT_DEBUG
std::cout << "Mouse button: " << down << "\n";
std::cout << "m_cursorOnBall: " << m_cursorOnBall << "\n";
std::cout << "m_drag: " << m_drag << "\n";
std::cout << "m_placeBallMode: " << m_placeBallMode << "\n";
#endif

  if (m_placeBallMode)
  {
    return;
  }

  if (!down)
  {
    Mouse::SetCursor(Mouse::STANDARD);
  }
  m_cursorOnBall = false;

  if (down)
  {
    MoveCueStart();
  }
  else
  {
    MoveCueFinish();
  }

#ifdef SHOOT_BUTTON
  // Check for shoot button cancel
  if (!down && s_shootSelected && !m_pShootButton->IsSelected())
  {
    s_shootSelected = false;
    //SetShotPower(0);
//    TakeShotFinish();

    CancelShot();
std::cout << "CANCEL SHOT, s_shootSelected = false\n";
  }
#endif
}

void EngineStatePoolSetUpShot::Red(bool down)
{
  if (!down)
  {
    return;
  }

  // Normal behaviour - go to PAUSED state.
  Engine::Instance()->ChangeState(EnginePaused::Name, Engine::IMMEDIATE);
}

void EngineStatePoolSetUpShot::TakeShotNowImpl(
  float yRot, float verticalVel, float horVel, float accel, float english, float drawRoll)
{
  // Inc no of strokes for this hole
  Engine::Instance()->GetGameState()->GetCurrentPlayerInfo()->m_strokes++;

  // TODO Set stroke parameters. Send a timed message to character
  // to start moving the ball when the club hits it.

  PoolBall* pBall = dynamic_cast<PoolBall*>(GetBall());
  Orientation o = *(pBall->GetOrientation());
  o.SetYRot(yRot);
  pBall->SetOrientation(o);
#ifdef POOL_ONLINE
        ShotLog("place cue ball 4");
#endif
  pBall->SetFalling(false); // to make sure we can set the forward vel..
  pBall->SetForwardVel(horVel); 
  pBall->SetFalling(true);
  pBall->SetVerticalVel(verticalVel); 
  pBall->SetForwardAccel(accel);

  // Fix for #62 - ball must forget about the platform
  // it used to be on. Not 100% sure this fixes the bug.
  ////pBall->SetActivePlatform(0);

  // POOL: Set english/ draw/roll
  static const float ENGLISH_VEL = Engine::Instance()->GetConfigFloat("pool_english_vel");
  static const float DRAW_ROLL_VEL = Engine::Instance()->GetConfigFloat("pool_drawroll_vel");
  // Convert english from -1 .. 1 to velocity in degs/sec.
  pBall->SetEnglishVel(english * ENGLISH_VEL);
  // Convert draw/roll from -1 .. 1 to vel in degs/sec
  // NB Need - so cue position below centre gives backspin.
  pBall->SetDrawRollVel(-drawRoll * DRAW_ROLL_VEL); 

//std::cout << "Taking shot: ENGLISH: " << english << "->" << english*ENGLISH_VEL << "deg/sec\n";
}

void EngineStatePoolSetUpShot::TakeShotNow(
  float yRot, float verticalVel, float horVel, float accel, float english, float drawRoll)
{
  TakeShotNowImpl(yRot, verticalVel, horVel, accel, english, drawRoll);
}

void EngineStatePoolSetUpShot::PlusUp(bool d)
{
  GetCamera()->PlusUp(d);
}

void EngineStatePoolSetUpShot::PlusDown(bool d)
{
  GetCamera()->PlusDown(d);
}

void EngineStatePoolSetUpShot::PlusLeft(bool d)
{
  GetCamera()->PlusLeft(d);
}

void EngineStatePoolSetUpShot::PlusRight(bool d)
{
  GetCamera()->PlusRight(d);
}

void EngineStatePoolSetUpShot::OnKey(char key, bool down)
{
  // ESC goes to Pause menu
  if (key == 27 /* ESC */ && !down)
  {
    OnMenuButton();
    return;
  }

  // TODO TEMP TEST
  // TOSO use key map so user can configure the keys.

  if (down && (key == 'z' || key == 'Z'))
  {
    GetCamera()->ZoomIn();
  }
  if (down && (key == 'x' || key == 'X'))
  {
    GetCamera()->ZoomOut();
  }

/*
  if (key == 's' || key == 'S')
  {
    // Cue swing
    if (down && m_drag && IsUserControlled())
    {
      s_cue.SetSwingMode(true); 
    }
    else if (!down)
    {
      s_cue.SetSwingMode(false); 
      m_shotIsActive = false; // bug 338 fix, camera stuck 
    }
  }

  // Contact GUI - if enabled, moving the mouse moves the contact spot.
  if (key == 'a' || key == 'A')
  {
    m_pContactPoolGui->SetEnabled(down);
  }
*/
}

}

