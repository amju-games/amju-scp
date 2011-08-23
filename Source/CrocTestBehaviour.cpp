/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: CrocTestBehaviour.cpp,v $
Revision 1.1  2004/09/08 15:42:35  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "CrocTestBehaviour.h"
#include "CharacterGameObject.h"
#include "Common.h"
#include "CrocHome.h"
#include "SchAssert.h"
#include "Engine.h"
#include "Level.h"
#include <iostream>

using namespace std;

namespace Amju
{
void CrocTestBehaviour::Update()
{
  m_pCharacter->SetForwardVel(0.0f);
  m_pCharacter->SetYRotateVel(0.0f);
}
}

