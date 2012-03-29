/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineMenuState.cpp,v $
Revision 1.1  2004/09/08 15:42:48  jay
Added to repository
 
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <iostream>
#include "Common.h"
#include "EngineMenuState.h"
#include "Engine.h"
#include "FirstPersonCamera.h"
#include "SolidSize.h"

namespace Amju
{
EngineMenuState::EngineMenuState()
{
  m_pCam = new FirstPersonCamera;  
  m_pCam->SetOrientation(Orientation(0, 0, 8, 0, 180, 0));
  m_verticalPos = 1.2f; // for 2 lines of text
  m_initialMenuTop = 0;
}

void EngineMenuState::FitMenuToScreen()
{
  float widest = 0;
  int numItems = m_pMenu->Size();
  for (int i = 0; i < numItems; i++)
  {
    SolidComponent* p = m_pMenu->GetItem(i);
    float w = GetSolidXSize(p);
    if (w > widest)
    {
      widest = w;
    }
  }

  if (widest == 0)
  {
    Assert(0);
  }

  // TODO adjust this if required
  m_pCam->SetOrientation(Orientation(0, 0, widest, 0, 180, 0));

}

bool EngineMenuState::Load()
{
  
  if (!m_bgQuad.Load(GetEngine()->GetConfigValue("file_bg"), ""))
  {
    return false;
  }
/*
  if (!m_boxQuad.Load(GetEngine()->GetConfigValue("box"), 
                      GetEngine()->GetConfigValue("box_alpha")))
  {
    return false;
  }
*/
  float x = Engine::Instance()->GetConfigFloat("file_bg_x");
  float y = Engine::Instance()->GetConfigFloat("file_bg_y");
  m_pScroller = new TextureScrollAttrib(x, y, 0); 

  float menuz = Engine::Instance()->GetConfigFloat("menu_z");
  m_pCam->SetOrientation(Orientation(0, 0, menuz, 0, 180, 0));

  // TODO Work out widest menu item and adjust z accordingly

  return true;
}

void EngineMenuState::DrawOverlays()
{
  m_greet.Draw();
  m_pMenu->DrawOverlays();
}

void EngineMenuState::Draw()
{
  AmjuGL::PushMatrix();

  // Don't write to the z-buffer, so background won't obscure things
  // drawn by subclasses.
  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_WRITE);
  // Draw the background - go off the screen 
  m_pScroller->Draw(); // Scroll the texture matrix
  m_bgQuad.Draw(-5, -5, 20, 25); // t, l, b, r
  m_pScroller->Restore();
  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_WRITE);

  // Do camera work
  m_pCam->Update();
  m_pCam->Draw();

  // Lighting
  m_lighting.DrawLight();

  m_pMenu->Draw();

  AmjuGL::PopMatrix();
}

void EngineMenuState::JoyY(float f)
{
  // If f goes up beyond threshold, call previous/next on the menu. Joystick 
  // must then return to zero.
  if (fabs(f) < SMALLEST)
  {
    // Returned to zero position.
    m_zeroed = true;
  }
  else
  {
    // If we are not zeroed, do nothing.
    if (!m_zeroed)
    {
      return;
    }
    m_zeroed = false;
    // We were zeroed, so go to the next item. 
    // +ve f is UP, so we go to the PREVIOUS item. 
    if (f > 0)
    {
      m_pMenu->Previous();
    }
    else
    {
      m_pMenu->Next();
    }
  }
}

void EngineMenuState::PlusUp(bool b)
{
  JoyY(b ? 1.0f : 0);
}

void EngineMenuState::PlusDown(bool b)
{
  JoyY(b ? -1.0f : 0);
}

void EngineMenuState::PlusLeft(bool b)
{
  JoyX(b ? -1.0f : 0);
}

void EngineMenuState::PlusRight(bool b)
{
  JoyX(b ? 1.0f : 0);
}

void EngineMenuState::MousePos(int x, int y)
{
  m_pMenu->MousePos(x, y);
}

void EngineMenuState::MouseButton(bool down, bool ctrl, bool shift)
{
  m_pMenu->MouseButton(down, ctrl, shift);

  if (down)
  {
    m_mouseDown = true;
    m_downChoice = m_pMenu->Choose();
  }
  
  if (m_mouseDown && !down && m_pMenu->IsChoiceValid() )
  {
#ifdef _DEBUG
    std::cout << "Selecting menu item.\n";
#endif
    Red(true); // select the current menu item
  }

  if (!down)
  {
    m_mouseDown = false;
  }
}

void EngineMenuState::SetGreet(const TextLeaf& t)
{
  m_greet = t;
}

}

