/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStatePoolCamera.cpp,v $
Revision 1.1.2.1  2006/08/14 17:50:13  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:25  jay
Moved Pool code from Pool/ to Pool/

Revision 1.3.2.2.4.1  2005/04/11 21:19:03  jay
Ongoing fixes for Pool

Revision 1.3.2.2  2004/10/04 10:04:03  jay
#96 Shot taken in Camera Mode showed player superimposed on ball.
Rearranged the logic which resets the player to the ball pos, and removed
binocs and menu buttons to simplify the no of ways in which the state can
be changed.

Revision 1.3.2.1  2004/10/01 09:58:36  Administrator
Helpful message at bottom of screen

Revision 1.3  2004/09/25 21:12:13  Administrator
Get rid of debug info.. haven't found problem yet though :-(

Revision 1.2  2004/09/21 15:02:50  Administrator
Fix bug #45, player position moves when Camera mode clicked.
Also don't move camera if ball is dragged.

Revision 1.1  2004/09/17 13:57:50  jay
Added camera mode

*/

#include "EngineStatePoolCamera.h"
#include "Engine.h"
#include "Level.h"
// Required by base class (for now)
#include "PowerGuage.h"
#include "PoolSetUpShotBehaviour.h"
#include "ThirdPersonCameraBase.h"
#include "PoolGameState.h"
#include "PoolMisc.h"
#include "TextWriter.h"

namespace Amju
{
const char* EngineStatePoolCamera::Name = "golf_camera";
// Register this State with the Engine.
static const bool registered= Engine::Instance()->
  RegisterEngineState(EngineStatePoolCamera::Name, new EngineStatePoolCamera);

void OnCameraReturn()
{
  PPoolGameObject pGo = Engine::Instance()->GetGameObject(
    Engine::Instance()->GetGameState()->GetCurrentPlayerInfo()->m_id);
  Orientation ballOr = *(GetBall()->GetOrientation());
  pGo->SetOrientation(ballOr);

  Engine::Instance()->ChangeState(EngineStatePoolSetUpShot::Name, Engine::IMMEDIATE);
}

void EngineStatePoolCamera::SetActive(bool active)
{
  m_pCameraButton->SetCommand(&OnCameraReturn);
  SetShowHelp(false);

  ThirdPersonCameraBase* pCam = 
    dynamic_cast<ThirdPersonCameraBase*>(GetCamera().GetPtr());
  Assert(pCam);

  if (active)
  {
    // Bug #45
    // Reset the player position to the ball position.
    // Otherwise the ball and character will move around when Camera State
    // is flipped on and off.
    // See corresponding code in EngineStatePoolSetUpShot.
    PPoolGameObject pGo = GetEngine()->GetGameObject(
      GetEngine()->GetGameState()->GetCurrentPlayerInfo()->m_id);
    Orientation ballOr = *(GetBall()->GetOrientation());
    pGo->SetOrientation(ballOr);
  }

  EngineStatePoolSetUpShot::SetActive(active);

  if (active)
  {
    GetEngine()->SetClearColour(1.0f, 0, 1.0f);
    GetEngine()->SetLetterbox(true);

    pCam->SetMouseDragEnabled(true);
    pCam->SetMouseZoomEnabled(true);
  }
  else
  {
    GetEngine()->SetClearColour(0, 0, 0);
    GetEngine()->SetLetterbox(false);
    
    pCam->SetMouseDragEnabled(false);
    pCam->SetMouseZoomEnabled(false);
  }
}

void EngineStatePoolCamera::MousePos(int x, int y)
{
  EngineStatePoolSetUpShot::MousePos(x, y);

  if (!m_shotIsActive)
  {
    GetCamera()->MousePos(x, y);
  }
}

void EngineStatePoolCamera::MouseButton(bool down, bool a, bool b)
{
  EngineStatePoolSetUpShot::MouseButton(down, a, b);
  
  if (!m_shotIsActive)
  {
    GetCamera()->MouseButton(down, a, b);
  }
}

void EngineStatePoolCamera::DrawOverlays()
{
/*
  std::string text = ToString(*(GetCamera()->GetOrientation()));
  Engine::Instance()->GetTextWriter()->Print(0, 0, text.c_str());

  text = ToString(GetCamera()->GetLookAtPos());
  Engine::Instance()->GetTextWriter()->Print(0, 1, text.c_str());
*/

  if (!m_shotIsActive)
  {
    Engine::Instance()->GetTextWriter()->PrintCentre(15, 
      "Camera mode - drag the mouse to move the camera!");
  }

  // Don't draw binoculars or menu button - so the only way to
  // leave this state is by taking a shot or clicking on the camera again,
  // or with prev/next hole arrows.
  // If we take the shot, don't reset the player pos to the ball pos.
  // If we click on the camera, then do reset the player pos.
  // See SetActive and OnCameraClicked.
  EngineStatePoolBase::DrawOverlays();

  if (m_shotIsActive)
  {
    m_pPowerGuage->Draw();
  }

  m_pCameraButton->Draw();

  DrawPlayerName();

  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ);

  if (m_shotIsActive)
  {
    AmjuGL::PushMatrix();
    AmjuGL::Translate(s_left, s_top, s_z);
    AmjuGL::Translate(13.0f, -17.0f, -1.0f);
    AmjuGL::Scale(0.9f, 0.9f, 0.9f);
    m_pTextPower->Draw();
    AmjuGL::PopMatrix();
  }

  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_READ);
}
}



