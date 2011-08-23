/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateProgressBar.cpp,v $
Revision 1.1.10.2  2006/08/30 21:09:35  Administrator
Get Pool to build in MSVC

Revision 1.1.10.1  2006/03/08 18:33:17  jay
Remove unused Mutex

Revision 1.1  2004/09/08 15:42:48  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EngineStateProgressBar.h"
#include "Engine.h"
#include "TextWriter.h"
#include "StringUtils.h"

namespace Amju
{
EngineStateProgressBar::EngineStateProgressBar()
{
  m_progressCount = 0;
  m_maxProgressCount = 0;
}

void EngineStateProgressBar::ResetProgressCount()
{
  m_progressCount = 0; 
}

void EngineStateProgressBar::IncProgressCount()
{
  ++m_progressCount;
}

void EngineStateProgressBar::SetMaxProgressCount(int m)
{
  m_maxProgressCount = m;
}

int EngineStateProgressBar::GetProgressCount()
{
  return m_progressCount;
}

bool EngineStateProgressBar::IsProgressFinished()
{
  bool finished = (m_progressCount >= m_maxProgressCount);
  return finished;
}

void EngineStateProgressBar::DrawProgressBar()
{
  float v = (float)m_progressCount / (float)m_maxProgressCount;
  
  m_pGuage->Set(v);
  m_pGuage->Update();
  m_pGuage->Draw();
}
}



