/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStatePoolText.h,v $
Revision 1.1.2.1  2006/08/14 17:50:18  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:30  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1  2004/09/08 15:43:05  jay
Added to repository
  
*/

#if !defined(ENGINE_STATE_GOLF_TEXT_H_INCLUDED)
#define ENGINE_STATE_GOLF_TEXT_H_INCLUDED

#include "EngineStatePoolBase.h"
#include "Level.h"

namespace Amju
{
class EngineStatePoolText : public EngineStatePoolBase
{
public:
  virtual bool Load();
  virtual void DrawOverlays();
  virtual void Update();

protected:
  PSolidComponent m_pText;
  float m_bihLeft, m_bihTop, m_bihZ;

};
}

#endif

