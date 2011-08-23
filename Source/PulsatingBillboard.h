/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PulsatingBillboard.h,v $
Revision 1.1  2004/09/08 15:42:55  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_PULSATING_BILLBOARD_H_INCLUDED)
#define SCHMICKEN_PULSATING_BILLBOARD_H_INCLUDED

#include "ExpandingBillBoard.h"

namespace Amju
{
// This kind of billboard expands, then returns to its original
// size.
// TODO This could be generalized by allowing the size to be determined
// by a Function.
class PulsatingBillboard : public ExpandingBillboard
{
public:
  PulsatingBillboard();

  virtual void Draw();
  virtual bool Load(File* pf);

  void Start() { m_isPulsing = true; }
  void Stop() { m_isPulsing = false; m_size = m_originalSize; }

protected:
  float m_originalSize; // copy of m_size
  float m_period; // After this period, size returns to original size.
  float m_periodtimer; // counts up to m_period.

  bool m_isPulsing; // true if pulsing; otherwise always original size.
};
}

#endif
