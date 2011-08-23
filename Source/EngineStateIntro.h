/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateIntro.h,v $
Revision 1.1  2004/09/08 15:42:36  jay
Added to repository
  
*/

#if !defined(ENGINE_STATE_INTRO_H_INCLUDED)
#define ENGINE_STATE_INTRO_H_INCLUDED

#include "EngineCutSceneState.h"
#include "ClipSequence.h"
#include "SolidComponent.h"

namespace Amju
{
// This is the Intro state, where a sequence of shots are shown, overlaid
// with the title etc.
class EngineStateIntro : public EngineCutSceneState
{
public:
  static const std::string Name; // ID of this state.

  EngineStateIntro();
  virtual ~EngineStateIntro();

  virtual bool Initialise();

  virtual void SetActive(bool active);

  virtual bool Load();

  virtual void Clear();

  virtual void Draw();

  virtual void DrawOverlays();

  // Intro state responds to the red button being pressed.
  virtual void Red(bool down);

  virtual void FinishCutScene();

protected:
  PClipSequence m_pClipSequence;
  float m_oldTime;
  // Viewable composite superimposed over scene.
  PSolidComponent m_pOverlay;
  // Delay before showing overlay.
  float m_overlayDelay;
  // Count how many times this state has been activated.
  int m_counter;
};
}
#endif

