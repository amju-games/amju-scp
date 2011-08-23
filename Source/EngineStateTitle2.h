/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateTitle2.h,v $
Revision 1.1  2004/09/15 09:13:47  Administrator
Extra title state
  
*/

#include "EngineStateText.h"

namespace Amju
{
// A different title, showing the game name close up, scrolling
// from right to left.
class EngineStateTitle2 : public EngineStateText
{
public:
  static const char* Name;

  EngineStateTitle2();
  virtual bool Load();
  virtual void Red(bool);
  virtual void TimerExpired();
  virtual void Draw();
  virtual void DrawOverlays();
  virtual void SetActive(bool active);
  virtual void MouseButton(bool down, bool ctrl, bool shift);
  virtual void MousePos(int x, int y);

protected:
  // X-position of horizontally-scrolling text.
  float m_hScroll;
};
}

