/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: CharacterMenu.cpp,v $
Revision 1.1  2004/09/08 15:42:46  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Common.h"
#include "CharacterMenu.h"

namespace Amju
{
CharacterMenu::CharacterMenu()
{
  SetVertical(false);
}

void CharacterMenu::Clear()
{
  m_chars.clear();
}

void CharacterMenu::DrawItem(int i)
{
  AmjuGL::PushMatrix();
  AmjuGL::Translate(0, -2.0f, 8.0f); // TODO CONFIG
  m_chars[i]->SetYRotateVel(20.0f); // TODO CONFIG
  m_chars[i]->Recalculate();
  m_chars[i]->Draw();
  AmjuGL::PopMatrix();
}

void CharacterMenu::PickDrawItem(int i)
{
  BoundingBox bb = m_chars[i]->GetCollisionVol()->GetBoundingBox();
  bb.DrawSolid();
}

int CharacterMenu::Size() const
{
  return m_chars.size();
}

void CharacterMenu::AddCharacter(CharacterGameObject* c)
{
  m_chars.push_back(c);
}
}

