/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: FrozenBehaviour.cpp,v $
Revision 1.1  2004/09/08 15:42:38  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "FrozenBehaviour.h"
#include "CharacterGameObject.h"
#include "SchAssert.h"

namespace Amju
{
void FrozenBehaviour::Update()
{
  Assert(m_pCharacter);

  m_pCharacter->SetForwardVel(0);
  m_pCharacter->SetYRotateVel(0);
}
}
