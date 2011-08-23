/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Clip.cpp,v $
Revision 1.1  2004/09/08 15:42:46  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Clip.h"
#include "Engine.h"

namespace Amju
{
bool Clip::IsActive()
{
  if (m_elapsedTime >= m_startTime && m_elapsedTime < m_endTime)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void Clip::Update()
{
  m_elapsedTime += Engine::Instance()->GetDeltaTime(); 
}

void Clip::Stop()
{
  //Engine::Instance()->FinishCutScene();
}
}

