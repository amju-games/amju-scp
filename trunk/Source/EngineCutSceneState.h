/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineCutSceneState.h,v $
Revision 1.1  2004/09/08 15:42:48  jay
Added to repository
  
*/

#if !defined(ENGINE_CUT_SCENE_STATE_H_INCLUDED)
#define ENGINE_CUT_SCENE_STATE_H_INCLUDED

#include "EngineRunningVisGraph.h"
#include "ClipSequence.h"
#include "Camera.h"

namespace Amju
{
// Engine State showing a cut scene. 
class EngineCutSceneState : public EngineRunningVisGraph
{
public:
  static const std::string Name; // ID of this state.

  EngineCutSceneState();

  // Override all the input functions. Cut scenes behave differently to input.
  virtual void YellowUp(bool);
  virtual void JoyX(float f);
  virtual void JoyY(float f);
  virtual void Blue(bool);
  virtual void Green(bool);
  virtual void Red(bool);
  virtual void PlusUp(bool);
  virtual void PlusDown(bool);
  virtual void PlusLeft(bool);
  virtual void PlusRight(bool);

  virtual void SetActive(bool active);
  virtual bool Load();
  virtual void Clear();
  virtual void Draw();
  virtual void DrawOverlays();
  virtual void FinishCutScene();

  virtual PCamera GetCamera();

  void SetSequence(PClipSequence pSeq);

protected:
  // Do "LookAt" function. This is overridden for Cut Scenes, where a camera
  // path is defined for each clip in the scene.
  virtual void DoCameraWork();

  PClipSequence m_pSeq;
};
}
#endif
