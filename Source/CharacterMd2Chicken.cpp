/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: CharacterMd2Chicken.cpp,v $
Revision 1.1  2004/09/08 15:42:34  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "CharacterMd2Chicken.h"
#include "Texture.h"
#include "SchAssert.h"

namespace Amju
{
Character* CharacterMd2Chicken::Clone() const
{
  return new CharacterMd2Chicken(*this);
}

void CharacterMd2Chicken::CreateSkinBinding()
{
  // Two skins, normal and blinking
  Assert(m_skins.size() == 2);
  // Closed eyes
  m_skins[0]->CreateBinding(Texture::REGULAR);
  // Open eyes
  m_skins[1]->CreateBinding(Texture::REGULAR);
}

void CharacterMd2Chicken::BindSkin()
{
  Assert(m_skins.size() == 2);

  // Only blink if this animation has more than some minimum no of frames.
  // Otherwise the blinking will look really crazy.
  int numberOfFrames = 21; // TODO

  bool eyesopen = !(numberOfFrames > 20 && (m_frame == 8 || m_frame == 15));
  
  if (eyesopen)
  {
    m_skins[1]->Bind();
  }
  else
  {
    m_skins[0]->Bind();
  }
}
}

