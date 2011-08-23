/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateObjectMenu.cpp,v $
Revision 1.1  2004/09/08 15:42:36  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Common.h"
#include "EngineStateObjectMenu.h"
#include "EngineStateLevelDesign.h"
#include "Engine.h"
#include "ObjectMenu.h"
#include "ObjectMenuItemCreate.h"
#include "SolidSize.h"
#include "TextMenu.h"
#include "SchAssert.h"

namespace Amju
{
const char* EngineStateObjectMenu::Name = "object_menu"; 
// Register this State with the Engine.
static const bool registered = Engine::Instance()->
  RegisterEngineState(EngineStateObjectMenu::Name, new EngineStateObjectMenu);

void EngineStateObjectMenu::SetActive(bool active)
{
  float z = GetEngine()->GetConfigFloat("object_menu_z");
  m_pCam->SetOrientation(Orientation(0, 0, z, 0, 180, 0));

}

void EngineStateObjectMenu::Draw()
{
  EngineMenuState::Draw();

  /*
  EngineMenuState::Draw();
  glPushMatrix();
  m_pCam->Update();
  m_pMenu2->Draw();
  glPopMatrix();
  
  glPushMatrix();

  // Don't write to the z-buffer, so background won't obscure things
  // drawn by subclasses.
  glDepthMask(GL_FALSE);
  // Draw the background - go off the screen 
  m_pScroller->Draw(); // Scroll the texture matrix
  m_bgQuad.Draw(-5, -5, 20, 25); // t, l, b, r
  m_pScroller->Restore();
  glDepthMask(GL_TRUE);

  // Do camera work
  m_pCam->Update();
  m_pCam->Draw();

  // Lighting
  m_lighting.DrawLight();

  glPushMatrix();
  m_pMenu->Draw();
  glPopMatrix();
  glTranslatef(0, -2.0f, -4.0f); // TODO TEMP TEST
  m_pMenu2->Draw();
  glPopMatrix();
  */

}

void EngineStateObjectMenu::DrawOverlays()
{
  m_greet.Draw();
  m_pMenu->DrawOverlays();
  //m_pMenu2->DrawOverlays();
}

void EngineStateObjectMenu::Red(bool down)
{
  if (!down)
  {
    return;
  }

  // Go to Level Design state.
  // We must do this BEFORE creating the new object so the Level Design
  // state is notified of the new object.
  GetEngine()->ChangeState(
    EngineStateLevelDesign::Name, Engine::IMMEDIATE);

  int choose = m_pMenu->Choose();
  if (choose == 0)
  {
    // Cancel
    return;
  }

  m_objectItems[choose - 1]->OnSelect();
}

bool EngineStateObjectMenu::Load()
{
  if (!EngineMenuState::Load())
  {
    return false;
  }

  m_pMenu = new TextMenu; //ObjectMenu; 
  //std::string menuFile = GetEngine()->GetConfigValue("new_object_menu_text"); //"object_menu");
  //if (!m_pMenu->Load(menuFile))
  //{
  //  return false;
  //}

  //m_pMenu->SetVertical(false);
  // TODO 
  // Add Object Menu Items for each kind of object we want to be able to
  // add to the room.
  // TODO TEMP TEST

  // new-style crates with grooves
  // simple block with groove
  AddItem(new ObjectMenuItemCreate("crate", "groove1.comp", "groove1.txt"));
  // sloping
  AddItem(new ObjectMenuItemCreate("crate", "groove2.comp", "groove2.txt"));
  // curved
  AddItem(new ObjectMenuItemCreate("crate", "groove3.comp", "groove3.txt"));
  // groove4 is a short grooved slope
  // 5 bowl
  AddItem(new ObjectMenuItemCreate("crate", "groove5.comp", "groove5.txt"));
  // 6 bowl with hole in bottom
  AddItem(new ObjectMenuItemCreate("crate", "groove6.comp", "groove6.txt"));
  // 7 cross
  AddItem(new ObjectMenuItemCreate("crate", "groove7.comp", "groove7.txt"));
  // old-style crates
  AddItem(new ObjectMenuItemCreate("crate", "cube2.comp", ""));
  AddItem(new ObjectMenuItemCreate("crate", "ramp2_2_4.comp", ""));
  AddItem(new ObjectMenuItemCreate("crate", "block4_2_2.comp", ""));
  // bonus
  AddItem(new ObjectMenuItemCreate("bonus", "melon.comp", ""));

/*
  if (!m_greet.Load("object_menu.text"))
  {
    return false;
  }

  // Set up second menu, which has text items "cancel" and "select".
  m_pMenu2 = new TextMenu;
  std::string menuFile2 = GetEngine()->GetConfigValue("object_text_menu");
  if (!m_pMenu2->Load(menuFile2))
  {
    return false;
  }

  m_pMenu->SetVertical(false);
*/
  return true;
}

void EngineStateObjectMenu::AddItem(ObjectMenuItem* p)
{
  m_objectItems.push_back(p);
/*
  Assert(p->GetMenuSolid());
  m_pMenu->AddItem(p->GetMenuSolid());
  int index = m_pMenu->Size() - 1;
  // Work out the scale for the menu solid.
  // Get the bounding box, and scale by the width.
  float x, y, z;
  GetSolidSize(p->GetMenuSolid(), &x, &y, &z);
  float s = 1.0f / x;
  ((ObjectMenu*)m_pMenu.GetPtr())->SetScale(index, s);
  ((ObjectMenu*)m_pMenu.GetPtr())->SetPos(index, -x * 0.5f, -y * 0.5f, -z * 0.5f);
*/
}

void EngineStateObjectMenu::MousePos(int x, int y)
{
  m_pMenu->MousePos(x, y);
//  m_pMenu2->MousePos(x, y);
}

void EngineStateObjectMenu::MouseButton(bool down, bool ctrl, bool shift)
{
  m_pMenu->MouseButton(down, ctrl, shift);
//  m_pMenu2->MouseButton(down, ctrl, shift);

  if (down)
  {
    m_mouseDown = true;
//    m_downChoice = m_pMenu2->Choose();
  }
/*  
  if (m_mouseDown && !down && m_pMenu2->IsChoiceValid()
           && m_pMenu2->Choose() == m_downChoice)
  {
    std::cout << "Selecting menu item.\n";
    Red(true); // select the current menu item
  }
*/
  if (!down)
  {
    m_mouseDown = false;
  }
}
}

