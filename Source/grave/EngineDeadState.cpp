/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineDeadState.cpp,v $
Revision 1.1  2004/09/08 15:42:35  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Engine.h"
#include "EngineDeadState.h"
#include "EngineStateTextGameOver.h"
#include "EngineRunning.h"
#include "EngineStateIntro.h"
#include "File.h"
#include "CharacterGameObject.h"
#include "Character.h"

using namespace std;

namespace Amju
{
const std::string EngineDeadState::Name = "dead";
// Register this State with the Engine.
static const bool registered = Engine::Instance()->
  RegisterEngineState(EngineDeadState::Name, new EngineDeadState);

EngineDeadState::EngineDeadState()
{
  m_time = 0;
  m_maxTime = 10.0f; // change state after this time
  m_fadeTime = 5.0f; // fade after this time
  m_fadeRate = 0.2f; // Fade period is 1/(this value).
  m_textTime = 4.0f; // show text after this time.
}

void EngineDeadState::SetActive(bool active)
{
  m_time = 0;
  if (active)
  {
    EngineRunning* pEr = (EngineRunning*)
      GetEngine()->GetEngineState(EngineRunning::Name).GetPtr();
    pEr->SetCameraStyle(EngineRunning::FOLLOW);

    GetEngine()->PlaySong(m_midifile);
  
    GetEngine()->SetLetterbox(true); 
  }
  else
  {
    GetEngine()->SetLetterbox(false); 
  }
}

bool EngineDeadState::Load()
{
  return true; // PORTING
  // Load the text
/*
  string textfile = GetEngine()->GetConfigValue("game_over_text");
  if (!m_text.Load(textfile))
  {
    GetEngine()->ReportError("Couldn't load text.");
    return false;
  }
*/
  // Get 3D letters
  string gecomp = GetEngine()->GetConfigValue("game_over_comp");
  m_pComp = SolidComponent::LoadSolid(gecomp);
  if (!m_pComp.GetPtr())
  {
    GetEngine()->ReportError("Failed to load Game Over state.");
    return false;
  }

  // Load the MIDI file
  m_midifile = GetEngine()->GetConfigValue("game_over_midi");
  
  // Get the timings.
  string strMaxTime = GetEngine()->GetConfigValue("game_over_max");
  string strFadeTime = GetEngine()->GetConfigValue("game_over_fade");
  string strFadeRate = GetEngine()->GetConfigValue("game_over_rate");
  string strTextTime = GetEngine()->GetConfigValue("game_over_text_time");
  m_maxTime = atof(strMaxTime.c_str());
  m_fadeTime = atof(strFadeTime.c_str());
  m_fadeRate = atof(strFadeRate.c_str());
  m_textTime = atof(strTextTime.c_str());

  return true;
}

void EngineDeadState::Draw()
{
  float dt = GetEngine()->GetDeltaTime(); 
  m_time += dt;

  if (m_time > m_maxTime)
  {
    // Go to next state.. back to intro.
    //GetEngine()->ChangeState(EngineStateIntro::Name, Engine::FADE);
    GetEngine()->ChangeState(EngineStateTextGameOver::Name, Engine::FADE);
  }
/*
  // Fade out after m_fadeTime secs
  if (m_time > m_fadeTime)
  {
    float fadeColour = 1.0f - (m_time - m_fadeTime) * m_fadeRate;
    if (fadeColour < 0)
    {
      fadeColour = 0;
    }

    glColor3f(fadeColour, fadeColour, fadeColour);
  }
*/
  // Draw using the running state.
  EngineRunning* pEr = (EngineRunning*)
    GetEngine()->GetEngineState(EngineRunning::Name).GetPtr();
  pEr->GetPlayer()->SetForwardVel(0);
  pEr->GetPlayer()->SetYRotateVel(0);

  glPushMatrix();
  pEr->Draw();
  glPopMatrix();
/*
  glPushMatrix();
  if (m_pComp.GetPtr())
  {
    VertexBase playerpos = pEr->GetPlayer()->GetPosition();
    glTranslatef(playerpos.x, playerpos.y, playerpos.z);
    // Rotate so letters are upright
    //glRotatef(90, 1, 0, 0);
    m_pComp->Draw();
  }
  glPopMatrix();
*/

  // Rotate camera around player.
  float rotvel = 10.0f; // TODO CONFIG degs/sec
  float rot = rotvel * dt;
  // TODO Need a CameraAutoRotate style ??!?!
//  pEr->RotateCameraHoriz(rot);

}

void EngineDeadState::DrawOverlays() 
{
  if (m_time > m_textTime)
  {
    // We don't draw the normal overlays. Draw "Game Over".
    //m_text.Draw();
  }
}

}
