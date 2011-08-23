/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateProgressBar.h,v $
Revision 1.1.10.1  2006/03/08 18:33:17  jay
Remove unused Mutex

Revision 1.1  2004/09/08 15:42:48  jay
Added to repository
  
*/

#ifndef ENGINE_STATE_PROGRESS_BAR_H_INCLUDED
#define ENGINE_STATE_PROGRESS_BAR_H_INCLUDED

#include "EngineState.h"
#include "Guage.h"

namespace Amju
{
class EngineStateProgressBar : public EngineState
{
public:
  EngineStateProgressBar(); 

  void ResetProgressCount();
  void IncProgressCount();
  void SetMaxProgressCount(int);
  int GetProgressCount();
  bool IsProgressFinished();

protected:
  void DrawProgressBar();

protected:
  int m_progressCount;
  int m_maxProgressCount;

  SharedPtr<Guage> m_pGuage;
};
}

#endif

