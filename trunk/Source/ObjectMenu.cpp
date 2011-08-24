/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ObjectMenu.cpp,v $
Revision 1.1  2004/09/08 15:42:53  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Common.h"
#include "ObjectMenu.h"
#include "SchAssert.h"
#include "Engine.h"
#include "PickHelper.h"
#include "File.h"

namespace Amju
{
bool ObjectMenu::Load(File* pf)
{
  if (!Menu::Load(pf))
  {
    return false;
  }
  return true;
}

void ObjectMenu::Draw()
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
    bool selected = (i == m_selected);
    float a = 1.0f;
    if (!selected)
    {
      a = 0.3f; // TODO CONFIG
    }
    float c = 1.0f;
    bool enabled = IsEnabled(i);
    if (!enabled)
    {
      c = 0;
    }


    Colour c1 = PickHelper::ReadCursorColour();
    Engine::Instance()->PushColour(c, c, c, a);
    DrawItem(i);
    Engine::Instance()->PopColour();
    Colour c2 = PickHelper::ReadCursorColour();


    if (IsPickMode() && c1 != c2)
    {
      // Found selected item
      SetSelected(i);
      EnsureVisible();
    }

    if (m_isVertical)
    {
      AmjuGL::Translate(0, -2.0f, 0);
    }
    else
    {
      AmjuGL::Translate(2.0f, 0, 0);
    }
  }

  AmjuGL::PopMatrix();

  SetPickMode(false);
}

void ObjectMenu::SetScale(int i, float f)
{
  if (i < (int)m_scales.size())
  {
    m_scales[i] = f;
  }
  else if (i == (int)m_scales.size())
  {
    m_scales.push_back(f);
  }
  else
  {
    Assert(0);
  }
}

void ObjectMenu::SetPos(int i, float x, float y, float z)
{
  VertexBase v(x, y, z);
  if (i < (int)m_positions.size())
  {
    m_positions[i] = v;
  }
  else if (i == (int)m_positions.size())
  {
    m_positions.push_back(v);
  }
  else
  {
    Assert(0);
  }
}

void ObjectMenu::DrawItem(int i)
{
  const float s = m_scales[i];
  //const VertexBase& pos = m_positions[i];

  AmjuGL::PushMatrix();
//  AmjuGL::Translate(pos.x*s, pos.y*s, pos.z*s);
//  AmjuGL::PushMatrix();
  AmjuGL::Scale(s, s, s);
  Menu::DrawItem(i);
//  AmjuGL::PopMatrix();
  AmjuGL::PopMatrix();
}
}

