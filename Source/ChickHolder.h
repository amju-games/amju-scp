/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ChickHolder.h,v $
Revision 1.1  2004/09/08 15:42:34  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_CHICK_HOLDER_H_INCLUDED)
#define SCHMICKEN_CHICK_HOLDER_H_INCLUDED

#include "Chick.h"

namespace Amju
{
// Mix-in class for Characters which can hold Chicks.
// This means bad NPCs and the Player.
class ChickHolder
{
public:
  ChickHolder();

  // Drop all references to Chicks. Call this on new Level etc.
  // for all ChickHolders.
  void Reset();

  // Clear Chicks from ChickList, and set them free!
  void FreeChicks();

  // Capture a Chick
  void CaptureChick(Chick*);

  bool HasChicks() const;

  // Remove the given Chick from the list. Used when the chick is saved.
  void ClearChick(Chick*);

  void SetBaseHeight(float);

protected:
  // Reassign vertical positions to the chicks in the list.
  // This is called when a chick is cleared or added, or the base height changes.
  void RecalcChickHeights();

protected:
  typedef std::list<Chick*> ChickList;
  ChickList m_chicklist;

  // Height above player of bottom chick.
  float m_baseHeight;
};
}

#endif

