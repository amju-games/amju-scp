/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineDeadState.h,v $
Revision 1.1  2004/09/08 15:42:35  jay
Added to repository
  
*/

#if !defined(ENGINE_DEAD_STATE_H_INCLUDED)
#define ENGINE_DEAD_STATE_H_INCLUDED

// DeadState.h
// The engine goes into this state when player is dead.
#include "EngineState.h"
#include "TextLeaf.h"

namespace Amju
{
class EngineDeadState : public EngineState
{
public:
  static const std::string Name; // ID of this state.

  EngineDeadState();

  virtual void Draw();
  virtual void DrawOverlays();
  virtual bool Load();
  virtual void SetActive(bool active);

protected:
  float m_fadeTime; // fade after this time..
  float m_fadeRate; // ..at this rate.
  float m_textTime; 

  // "Game Over" text.
  //TextLeaf m_text;
  PSolidComponent m_pComp;

  // MIDI filename
  std::string m_midifile;
};
}
#endif

