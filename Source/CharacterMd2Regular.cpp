/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: CharacterMd2Regular.cpp,v $
Revision 1.1  2004/09/08 15:42:46  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "CharacterMd2Regular.h"
#include "Texture.h"
#include "SchAssert.h"

namespace Amju
{
Character* CharacterMd2Regular::Clone() const
{
  return new CharacterMd2Regular(*this);
}

void CharacterMd2Regular::CreateSkinBinding()
{
  Assert(m_skins.size() == 1);
  m_skins[0]->CreateBinding(Texture::REGULAR);
}

void CharacterMd2Regular::BindSkin()
{
  Assert(m_skins.size() == 1);
  m_skins[0]->Bind();
}
}
