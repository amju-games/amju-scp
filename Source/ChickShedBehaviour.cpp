/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ChickShedBehaviour.cpp,v $
Revision 1.1  2004/09/08 15:42:34  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "ChickShedBehaviour.h"
#include "CharacterGameObject.h"
#include "HeightServer.h"
#include "Common.h"
#include "Level.h"
#include "Engine.h"
#include "Chick.h"
#include "Shed.h"
#include "GameState.h"
#include "VisibilityGraph.h"
#include "SchAssert.h"

namespace Amju
{
PVisibilityGraph ChickShedBehaviour::s_visgraph;

void ChickShedBehaviour::OnActivated()
{
  m_vel = 1.0f; // TODO CONFIG
}

void ChickShedBehaviour::ReachedTarget()
{
  Assert(m_pCharacter);
  Chick* pChick = dynamic_cast<Chick*>(m_pCharacter);
  Assert(pChick);
  pChick->Win();
}

void ChickShedBehaviour::SetVisibilityGraph(PVisibilityGraph visgraph)
{ 
  s_visgraph = visgraph; 
}

PVisibilityGraph ChickShedBehaviour::GetVisibilityGraph()
{
  return s_visgraph;
}

}


