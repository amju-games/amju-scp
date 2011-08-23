/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStatePoolShowShot.cpp,v $
Revision 1.1.2.9  2007/07/22 12:43:25  jay
Turn off debug output in release mode

Revision 1.1.2.8  2007/06/26 09:12:00  jay
Fix camera: was not showing online shots replayed from server.
Reason was camera was not being reset - also ball Y pos must be set as this
 changes the forward vector, which is used in ThirdPersonCamera2 to decide
 which direction to Pull Back.

Revision 1.1.2.7  2007/06/24 22:27:04  Administrator
Fix MSVC warning

Revision 1.1.2.6  2007/06/23 11:31:02  jay
Camera: not fixed, but not as broken as it was

Revision 1.1.2.5  2007/03/12 23:42:56  Administrator
Consistency fix: don't update shot in this Engine State, wait till
 EngineStatePoolShotInPlay is active.

Revision 1.1.2.4  2007/03/09 21:50:41  jay
Online game - ongoing fixes

Revision 1.1.2.3  2006/08/26 21:28:37  jay
Ongoing changes for Online game; all Online features are off unless
POOL_ONLINE is defined

Revision 1.1.2.2  2006/08/17 18:15:42  jay
Ongoing work for Online Pool

Revision 1.1.2.1  2006/08/14 17:50:17  jay
Rename "Pool"

Revision 1.1.2.2  2006/08/12 07:43:08  jay
Ongoing fixes for Online Pool: currently trying to get both clients to always
be consistent.

Revision 1.1.2.1  2006/07/24 18:20:30  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.8.11  2005/09/23 19:41:37  jay
Use PoolChangeState

Revision 1.1.8.10  2005/09/12 20:40:49  jay
Fixed Update so balls drop down to the table, etc.

Revision 1.1.8.9  2005/09/12 17:23:47  jay
Override Update() to avoid possibility of setting balls in motion, then calling
Update twice before testing for collisions

Revision 1.1.8.8  2005/08/26 21:31:56  jay
Add mouse camera control

Revision 1.1.8.7  2005/07/05 09:37:20  jay
Show valid shots for AI debugging

Revision 1.1.8.6  2005/06/22 21:39:59  jay
Get HUD to draw consistently

Revision 1.1.8.5  2005/06/19 21:31:30  jay
Override DrawOverlays so HUD is consistent... needs work

Revision 1.1.8.4  2005/06/13 22:19:17  jay
Integrate AI animations

Revision 1.1.8.3  2005/05/24 20:41:25  jay
Stop player camera movement - doesn't work properly ?

Revision 1.1.8.2  2005/05/15 17:16:45  jay
Go immediately to next state

Revision 1.1.8.1  2005/04/17 22:04:06  jay
Added english and draw/roll parameters to shot

Revision 1.1  2004/09/08 15:43:05  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EngineStatePoolShowShot.h"
#include "Engine.h"
#include "EngineStatePoolShotInPlay.h"
#include "Level.h"
#include "LevelServer.h"
#include "PoolMisc.h"
#include "EngineStatePoolSetUpShot.h"
#include "GameState.h"
#include "PoolCharacter.h"
#include "SoundFxManager.h"
#include "Frustum.h"
#include "PoolCamera.h"
#include "PoolChangeState.h"

#ifdef _DEBUG
//#define SHOW_VALID_SHOTS
#endif

namespace Amju
{
const char* EngineStatePoolShowShot::Name = "golf_show_shot";
// Register this State with the Engine.
static const bool registeredgolfshowshot = Engine::Instance()->
  RegisterEngineState(EngineStatePoolShowShot::Name, new EngineStatePoolShowShot);

void EngineStatePoolShowShot::Update()
{
  if (m_time < m_maxTime)
  {
    // POOL_ONLINE:
    // DON'T call EngineStatePoolBase::Update();
    // If the timer expires, the shot will be started, but we DON'T want to update
    //  the shot in this state, for consistency with the other (user-controlled) client.

    // If a moving ball is outside of the view frustum, pull the camera back.
    ((PoolCamera*)s_pCamera.GetPtr())->SetPoolPullBackRequired(s_movingBallNotInFrustum);
    s_movingBallNotInFrustum = false;

    GetCamera()->Update();

    if (!IsBirdsEye())
    {
      // Update the camera so it is always above the ball
      float camY = GetCamera()->GetOrientation()->GetY();
      float ballY = GetBall()->GetOrientation()->GetY();
      ballY += 1.0f; // TODO CONFIG
      if (camY < ballY)
      {
        Orientation o = *(GetCamera()->GetOrientation());
        o.SetY(ballY);
        GetCamera()->SetOrientation(o);
      }
    }
    // May call TimerExpired.....
    EngineState::Update();

    if (m_time < m_maxTime)
    {
      Assert(m_pLevel.GetPtr());
      GetEngine()->GetDayNightSky()->Update();
      m_pLevel->GetScene()->Update();
      UpdateGameObjects();
    }
  }
  else
  {
    // DON'T update game objects
    EngineState::Update();
  }
}

void EngineStatePoolShowShot::SetActive(bool active)
{
  if (active)
  {
    GetEngine()->SetLetterbox(true);

    m_pLevel = LevelServer::Instance()->GetCurrentLevel();

    // The "take shot" animation expects the ball to be at the (x, z) origin.
    Orientation o = *(GetActivePlayer()->GetOrientation());
    VertexBase v = GetBall()->GetOrientation()->GetVertex();
    o.SetX(v.x);
    o.SetZ(v.z);

    GetActivePlayer()->SetOrientation(o); 

    if (IsOnlineGame() && 
        Engine::Instance()->GetGameState()->GetCurrentPlayerInfo()->m_isOnline) // TEST
    {
      // Player character was hidden, so unhide
      GetActivePlayer()->SetAlpha(1.0f); 
    }
    GetActivePlayer()->SetActionGo();

    Assert(GetCamera().GetPtr());    
   
    // For testing, allow user to cycle through all valid shots. 
    // Don't play on until key is pressed.
#ifdef SHOW_VALID_SHOTS
    SetMaxTime(99999.9f);
    // Debug print info on current shot
    m_validShotNum = -1;
    Blue(true);
#else
    // POOL - immediately play shot, as the use has already swing the cue.
    // But do wait if player is not user controlled, so we see the swing
    // animation.
    if (IsUserControlled())
    {
      SetMaxTime(0); 
    }
    else
    {
      SetMaxTime(GetEngine()->GetConfigFloat("golf_swing_time"));
      // Pull the camera back so we can see where the AI player is aiming.
      Camera* pCam = GetCamera().GetPtr();
      PoolCamera* pPoolCam = dynamic_cast<PoolCamera*>(pCam);
      if (pPoolCam)
      {
        pPoolCam->SetIsAiPlayer(true);
      }
      // If online player is taking shot we want the camera to be looking
      // at the player character, not behind her.
      if (IsOnlineGame())
      {
#ifdef _DEBUG
std::cout << "*** IS ONLINE, setting camera...\n";
#endif

        Orientation o = *(GetActivePlayer()->GetOrientation());
        Orientation o2 = *(GetBall()->GetOrientation());
        o2.SetYRot(o.GetYRot() + 180.0f); // look at from front
        GetBall()->SetOrientation(o2);
        GetCamera()->Reset();
      }
    }
#endif
    m_time = 0;
  }
  else
  {
    GetEngine()->SetLetterbox(false);
  }
}

void EngineStatePoolShowShot::Blue(bool down)
{
#ifdef SHOW_VALID_SHOTS
  if (!down)
  {
    return;
  }

  // Go to next valid shot. If at the end, go back to start.
  ++m_validShotNum;

  int maxValidShot = GetRules(m_pLevel.GetPtr())->GetBehaviour()->
    GetNumValidShots();
  if (m_validShotNum >= maxValidShot)
  {
    m_validShotNum = 0;
  }
  std::cout << "SHOWING VALID SHOT " << m_validShotNum + 1 << " OF "
    << maxValidShot 
    << ": "
    << GetRules(m_pLevel.GetPtr())->GetBehaviour()->
        ValidShotString(m_validShotNum).c_str() 
    << "\n";
#endif
}

void EngineStatePoolShowShot::Green(bool down)
{
#ifdef SHOW_VALID_SHOTS
  // Play on
  std::cout << "PLAY ON \n";
  TimerExpired();
#endif
}

void EngineStatePoolShowShot::OnKey(char c, bool down)
{
#ifdef SHOW_VALID_SHOTS
  if (c == 'q' || c == 'Q')
  {
    Blue(down);
  }
  else if (c == 'w' || c == 'W')
  {
    Green(down);
  }
  else
  {
    EngineStatePoolBase::OnKey(c, down);
  }
#else
  EngineStatePoolBase::OnKey(c, down);
#endif
}

void EngineStatePoolShowShot::TimerExpired()
{
  // Play wav: cue hits ball
  // TODO find decent wav
  static const std::string sfx = Engine::Instance()->
    GetConfigValue("golf_wav_11");
  SoundFxManager::Instance()->PlayWav(sfx.c_str());

  const GameState::PlayerInfo::PoolStroke& gs = 
    GetEngine()->GetGameState()->GetCurrentPlayerInfo()->m_golfStroke;
 
  EngineStatePoolSetUpShot::TakeShotNowImpl(gs.m_yRot, gs.m_vertVel, gs.m_horVel, gs.m_accel,
    gs.m_english, gs.m_drawRoll);

  // Stop user-controlled camera movement
  GetCamera()->PlusUp(false);
  GetCamera()->PlusDown(false);
  GetCamera()->PlusLeft(false);
  GetCamera()->PlusRight(false);

  ChangeStateToShotInPlay();
}

void EngineStatePoolShowShot::Draw()
{
#ifdef SHOW_VALID_SHOTS
  EngineStatePoolBase::DrawBackground();
//  GetActivePlayer()->DrawSwish();
  Frustum::Instance()->Create();
  m_scenePolys = m_pLevel->Draw();

  GetRules(m_pLevel.GetPtr())->GetBehaviour()->DrawValidShot(m_validShotNum);
  DrawGameObjects(false); // false => non-translucent
#else
  EngineStatePoolBase::Draw();
#endif
}

void EngineStatePoolShowShot::DrawOverlays()
{
  EngineStatePoolBase::DrawOverlays();
  DrawHudBall();
}

void EngineStatePoolShowShot::MousePos(int x, int y)
{
  GetCamera()->MousePos(x, y);
}

void EngineStatePoolShowShot::MouseButton(bool down, bool ctrl, bool shift)
{
  GetCamera()->MouseButton(down, ctrl, shift);
}

}


