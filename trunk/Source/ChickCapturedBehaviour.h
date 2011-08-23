/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ChickCapturedBehaviour.h,v $
Revision 1.1  2004/09/08 15:42:34  jay
Added to repository
  
*/

#if !defined(CHICKCAPTUREDBEHAVIOUR_H_INCLUDED)
#define CHICKCAPTUREDBEHAVIOUR_H_INCLUDED

#include "Behaviour.h"

namespace Amju
{
// This behaviour controls a Chick when it has been captured by a baddie.
class ChickCapturedBehaviour : public Behaviour
{
public:
  ChickCapturedBehaviour() : m_yOffset(0) {}

  virtual void Update();

  void SetHeight(float h) { m_yOffset = h; }

protected:
  // The height above ground level which the Chick sits at while captured.
  // This lets one NPC capture a number of chicks, which can float at 
  // different heights above the NPC.
  float m_yOffset;
};

}

#endif
