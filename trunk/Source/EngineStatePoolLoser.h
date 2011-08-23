/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStatePoolLoser.h,v $
Revision 1.1.2.1  2006/08/14 17:50:14  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:27  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1  2004/09/08 15:43:04  jay
Added to repository
  
*/

#if !defined(ENGINE_STATE_GOLF_LOSER_H_INCLUDED)
#define ENGINE_STATE_GOLF_LOSER_H_INCLUDED

#include "EngineStatePoolText.h"

namespace Amju
{
class EngineStatePoolLoser : public EngineStatePoolText
{
public:
  static const char* Name;

  virtual bool Load();
  virtual void SetActive(bool active);

protected:
  virtual void TimerExpired();

protected:
  // We only want to check for hi score etc once!
  bool m_isFading;

};
}

#endif

