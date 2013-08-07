/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Menu.cpp,v $
Revision 1.1.10.1  2005/06/04 22:25:42  jay
Change to vertical spacing

Revision 1.1  2004/09/08 15:42:52  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <algorithm>
#include "Menu.h"
#include "CompositeFactory.h"
#include "Common.h"
#include "Engine.h"
#include "TextLeaf.h"
#include "AttribDecorator.h"
#include "Attrib.h"
#include "File.h"
#include "FirstPersonCamera.h"
#include "GuiTextButton.h"
#include "MenuCommand.h"
#include "SchAssert.h"

using namespace std;

namespace Amju
{
Menu::Menu()
{
  m_selected = -1;
  m_menuTop = 0;
  m_topItem = 0;

  m_isPickMode = false;

  m_isVertical = true;
  m_drag = false;
}

Menu::~Menu()
{
}

bool Menu::LoadButtons(File* pf)
{
  // Load the next/previous buttons
  m_pPrevButton = new PoolGuiTextButton;
  if (!m_pPrevButton->Load(pf))
  {
    pf->ReportError("Failed to load previous button.");
    return false;
  }
  m_pNextButton = new PoolGuiTextButton;
  if (!m_pNextButton->Load(pf))
  {
    pf->ReportError("Failed to load next button.");
    return false;
  }

  // TODO load text, set command for each one.
  m_pPrevButton->SetCommand(new MenuCommand(this, -1.0f));
  m_pNextButton->SetCommand(new MenuCommand(this, 1.0f));

  // Set the size and position of each button. The positions
  // will depend on whether the menu is vertical or horizontal.
  m_pPrevButton->SetSize(2.0f, 2.0f); // TODO CONFIG
  m_pPrevButton->SetRelPos(3, 0); // top, left // TODO TEMP TEST
  m_pNextButton->SetSize(2.0f, 2.0f); // TODO CONFIG
  m_pNextButton->SetRelPos(3, 10.0f); // TODO TEMP TEST

  return true;
}

int Menu::Size() const
{
  return m_items.size();
}

void Menu::SetVertical(bool vertical)
{
  m_isVertical = vertical;
}

bool Menu::IsVertical() const
{
  return m_isVertical;
}

void Menu::SetSelected(int selected)
{
  int size = Size();
  Assert(selected < size);
  m_selected = selected;
  EnsureVisible();
}

void Menu::EnsureVisible()
{
  const int MAX_MENU = 2; // TODO CONFIG

  if (m_selected - m_topItem > MAX_MENU)
  {
    while (m_selected - m_topItem > MAX_MENU)
    {
      m_topItem++;
      m_menuTop += 2.0f;
    }
    return;
  }

  if (m_selected < m_topItem)
  {
    while (m_selected < m_topItem)
    {
      m_topItem--;
      m_menuTop -= 2.0f;
    }
  }
}

void Menu::SetPickMode(bool b)
{
  m_isPickMode = b;
}

bool Menu::IsPickMode() const
{
  return m_isPickMode;
}

void Menu::Previous()
{
  if (m_selected > 0)
  {
    m_selected--;
  }

  EnsureVisible();
}

void Menu::Next()
{
  if (m_selected < (Size() - 1))
  {
    m_selected++;
  }

  EnsureVisible();
}

static bool operator !=(const Colour& c1, const Colour& c2)
{
  return (c1.m_r != c2.m_r || c1.m_g != c2.m_g || c1.m_b != c2.m_b || c1.m_a != c2.m_a);
}

void Menu::Draw()
{
  // Picking: If we are in picking mode, check the colour under the 
  // mouse cursor after drawing each item. If the colour changes we 
  // have found the selected item.

  AmjuGL::PushMatrix();

  // Set the top of the menu. This should be set so that the currently selected
  // menu item is always visible!
  if (m_isVertical)
  {
    AmjuGL::Translate(0, m_menuTop, 0);
  }
  else
  {
    AmjuGL::Translate(-m_menuTop, 0, 0);
  }


  for (int i = 0; i < Size(); i++)
  {
    //bool selected = (i == m_selected);
    bool enabled = IsEnabled(i);
    float a = 1.0f;
    if (!enabled)
    {
      a = 0.3f; // TODO CONFIG
    }

    AmjuGL::Disable(AmjuGL::AMJU_DEPTH_WRITE);

    // Unselected colour
    Engine::Instance()->PushColour(0.0f, 0.0f, 0.0f, a);
    DrawItem(i);
    Engine::Instance()->PopColour();

    // TODO Good lord
    Colour c1; // = PickHelper::ReadCursorColour();

    // Draw in selected colour
    Engine::Instance()->PushColour(1.0f, 1.0f, 1.0f, a);
    DrawItem(i);
    Engine::Instance()->PopColour();
    Colour c2; // = PickHelper::ReadCursorColour();

    if (IsPickMode() && c1 != c2)
    {
      // Found selected item
      SetSelected(i);
      //selected = true;//(i == m_selected);
    }
/*
    // Draw in unselected colour again (unless item is selected)
    if (!selected)
    {
      Engine::Instance()->PushColour(0.0f, 0.0f, 0.0f, a);
      DrawItem(i);
      Engine::Instance()->PopColour();
    }
*/
    AmjuGL::Enable(AmjuGL::AMJU_DEPTH_WRITE);

    if (m_isVertical)
    {
      static const float MENU_VERT_SPACE = Engine::Instance()->
        GetConfigFloat("menu_vert_space");
      AmjuGL::Translate(0, MENU_VERT_SPACE, 0);
    }
    else
    {
      AmjuGL::Translate(2.0f, 0, 0);
    }
  }

  AmjuGL::PopMatrix();

  SetPickMode(false);
}

bool Menu::Load(const std::string& filename)
{
  File f;
  if (!f.OpenRead(filename))
  {
    f.ReportError("Couldn't open menu file.");
    return false;
  }
  return Load(&f);
}

bool Menu::Load(File* pf)
{
  if (!LoadButtons(pf))
  {
    return false;
  }
  return true;
}

void Menu::SetMenuTop(float f)
{
  m_menuTop = f;
}

SolidComponent* Menu::GetItem(int index)
{
  Assert(index < (int)m_items.size());
  return m_items[index].GetPtr();
}

bool Menu::IsEnabled(int i)
{
  Assert(i < (int)m_enabled.size());
  return m_enabled[i];
}

void Menu::SetEnabled(int i, bool b)
{
  Assert(i < (int)m_enabled.size());
  m_enabled[i] = b;
}

void Menu::EnableAll(bool b)
{
  for (std::vector<bool>::iterator it = m_enabled.begin(); 
       it != m_enabled.end();
       *it++ = b);
}

void Menu::MousePos(int x, int y)
{
  if (m_pPrevButton.GetPtr())
  {
    m_pPrevButton->MousePos(x, y);
  }
  if (m_pNextButton.GetPtr())
  {
    m_pNextButton->MousePos(x, y);
  }

  if (!m_drag)
  {
    return;
  }

//  m_selected = -1;
  MousePick();
}

void Menu::MouseButton(bool down, bool ctrl, bool shift)
{
  if (m_pPrevButton.GetPtr())
  {
    m_pPrevButton->MouseButton(down, ctrl, shift);
    if (m_pPrevButton->IsSelected())
    {
      m_selected = -1;
    }
  }
  if (m_pNextButton.GetPtr())
  {
    m_pNextButton->MouseButton(down, ctrl, shift);
    if (m_pNextButton->IsSelected())
    {
      m_selected = -1;
    }
  }

  m_drag = down;

  if (down)
  {
    MousePick();
  }
}

void Menu::AddItem(SolidComponent* p)
{
  m_items.push_back(p);

  // Make sure enabled flag exists for the new item.
  m_enabled.push_back(true);
}

void Menu::DeleteItem(int i)
{
  // TODO convert index to iterator 
  //m_items.erase(it);
}

void Menu::MousePick()
{
  // Work out which menu item is under the mouse cursor.
  // If an item is under the cursor, this becomes the selected item.
  // Otherwise the selection is unchanged.
  
  // For each item, get the bounding box.
  // Map the box to screen coords.

  // Or convert mouse pos to ray, and do intersection tests.

  // Or draw menu items one at a time until the colour under the mouse
  // cursor changes.
  SetPickMode(true);
}

void Menu::DrawItem(int i)
{
  m_items[i]->Update();
  m_items[i]->Draw();
}

void Menu::DrawOverlays() 
{  
  // Draw GUI Buttons for Next/Previous.
  // TODO
  if (m_pPrevButton.GetPtr())
  {
    m_pPrevButton->Draw();
  }
  if (m_pNextButton.GetPtr())
  {
    m_pNextButton->Draw();
  }
}

bool Menu::IsChoiceValid() const
{
  bool b = (m_selected != -1);
  return b;
}

float Menu::GetTop() const
{
  return m_menuTop;
}

void Menu::SetTop(float t)
{
  m_menuTop = t;
}
}
