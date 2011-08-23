/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateDemo.h,v $
Revision 1.1  2004/09/08 15:42:36  jay
Added to repository
  
*/

#if !defined(ENGINE_STATE_DEMO_H_INCLUDED)
#define ENGINE_STATE_DEMO_H_INCLUDED

#include "EngineRunningVisGraph.h"

namespace Amju
{
class Engine;

// This kind of State runs the game but in demo mode: the player cannot
// be controlled (or seen ?)
// The camera is a TP camera following a GameObject.
class EngineStateDemo : public EngineRunningVisGraph
{
public:
  static const std::string Name; // ID of this state.

  EngineStateDemo();
  virtual ~EngineStateDemo() {}

  // Draw and update timer
  virtual void Draw();
  virtual void DrawOverlays();

  virtual void SetActive(bool active);

  virtual void Red(bool down);

  virtual PCamera GetCamera();

  virtual void DrawPlayer();

  virtual void GameOver();


protected:
  virtual void DoCameraWork();

  // Select a focus for the camera.
  void SetCameraTarget();

protected:
  PCamera m_pCamera;

};
}
#endif

