/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: AiBehaviourBase.cpp,v $
Revision 1.1  2004/09/08 15:43:01  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "AiBehaviourBase.h"
#include "EngineRunningVisGraph.h"
#include "Engine.h"

namespace Amju
{
AiBehaviourBase::AiBehaviourBase()
{
  m_isTaskCompleted = false;
}

PVisibilityGraph AiBehaviourBase::GetVisibilityGraph()
{
  return EngineRunningVisGraph::GetVisibilityGraph();
}

void AiBehaviourBase::AvoidObstacles()
{
  // TODO just avoid other characters
  ///SeekBehaviourBase::AvoidObstacles();  // TODO TEMP TEST
}

void AiBehaviourBase::Update()
{
  if (m_isTaskCompleted)
  {
    return;
  }
  SeekBehaviourBase::Update();
}
}