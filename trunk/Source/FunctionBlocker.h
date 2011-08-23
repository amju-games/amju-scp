/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: FunctionBlocker.h,v $
Revision 1.1  2004/09/08 15:42:50  jay
Added to repository
  
*/

#if !defined(FUNCTION_BLOCKER_H_INCLUDED)
#define FUNCTION_BLOCKER_H_INCLUDED

#include "Function.h"
#include "Orientation.h"

namespace Amju
{
// Takes before and after orientations, and a duration. 
// Typically moves a Blocker from one position to another
// when the message to move is received by the Blocker.
class FunctionBlocker : public Function
{
public:
  static const char* Name;

  virtual bool Load(File* pf);
  virtual void Recalc(Orientation* p);

protected:
  Orientation m_before;
  Orientation m_after;
  // From the duration, get the velocity for each Orientation member.
  // Stored as another Orientation, but m_x, m_y etc are now velocities.
  Orientation m_vels;
  // Time taken to move from Before to After Orientations.
  float m_duration;
  // Timer for repeating sound when blocker moves.
  float m_soundTime;
  // If false, do earthquake etc when function reaches its final
  // state. 
  bool m_quake;

#if defined(SCENE_EDITOR)
public:
  virtual const char* GetTypeName() const;
  virtual bool Save(File*);

  Orientation* GetOrBefore() { return &m_before; }
  Orientation* GetOrAfter() { return &m_after; }

  float GetPeriod() const { return m_duration; }
  void SetPeriod(float p) { m_duration = p; }

#endif
};
}

#endif

