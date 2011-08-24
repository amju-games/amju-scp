/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ChickCapturedBehaviour.cpp,v $
Revision 1.1.10.1  2005/06/05 00:25:44  Administrator
Hacks to get this to compile with const Orientations.

Revision 1.1  2004/09/08 15:42:34  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "ChickCapturedBehaviour.h"
#include "CharacterGameObject.h"
#include "Engine.h"
#include "SchAssert.h"

namespace Amju
{
void ChickCapturedBehaviour::Update()
{
  Assert(m_pCharacter); // the Chick which this behaviour controls
  Assert(m_pTarget.GetPtr()); // the thing which has captured the Chick

  // As the Chick has been captured, it follow the Target, which is the thing 
  // that has captured the chick.
  m_pCharacter->SetForwardVel(0); 
  m_pCharacter->SetYRotateVel(0);

  const Orientation* pChickOrientation = m_pCharacter->GetOrientation();

  // (x, z) Chick coords match the target..
  //pChickOrientation->SetX(pCgo->GetOrientation()->GetX());
  //pChickOrientation->SetZ(pCgo->GetOrientation()->GetZ());

  // y-coord is above the target.
  //pChickOrientation->SetY(pCgo->GetOrientation()->GetY() + m_yOffset); // TODO CONFIG

  // Slowly rotate
  float yrot = pChickOrientation->GetYRot();
  yrot += Engine::Instance()->GetDeltaTime() * 45.0f; // TODO CONFIG
  //pChickOrientation->SetYRot(yrot);
}
}
