/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: StopBehaviour.cpp,v $
Revision 1.1  2004/09/08 15:42:41  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "StopBehaviour.h"
#include "CharacterGameObject.h"
#include "HeightServer.h"
#include "Common.h"

namespace Amju
{
void StopBehaviour::Update()
{
  m_pCharacter->SetForwardAccel(0); 
  m_pCharacter->SetYRotateVel(0);
  m_pCharacter->SetForwardVel(0);
}

}




