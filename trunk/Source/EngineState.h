/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineState.h,v $
Revision 1.2.6.2  2005/10/29 10:15:10  jay
Add support for OS-specific special events.
E.g. Paste command.

Revision 1.2.6.1  2005/07/17 23:01:16  jay
Handle right and middle mouse buttons

Revision 1.2  2004/09/15 09:17:09  Administrator
Removed some confusing old code

Revision 1.1  2004/09/08 15:42:48  jay
Added to repository
  
*/

#if !defined(ENGINE_STATE_H_INCLUDED)
#define ENGINE_STATE_H_INCLUDED

#include <string>
#include "Camera.h"
#include "RCPtr.h"
#include "SpecialEvent.h"

namespace Amju
{
class Message;
class Engine;

class EngineState : public RefCounted
{
public:
  EngineState();
  virtual ~EngineState() {}

  // Helper function, returns owning Engine.
  static Engine* GetEngine();

  // Hopefully unambiguous function names for a new game and new level.  
  virtual void OnNewLevel() {}
  virtual void OnNewGame() {}
  // Called when a new rooom becomes the current room.
  virtual void OnNewRoom() {}

  // Call with parameter true to make this state the currently active state;
  // with false to make this state inactive.
  virtual void SetActive(bool active);

  // Call to load up state-specific data.
  virtual bool Load() { return true; }

  virtual void Clear() {}

  // Draw scene. Called by Engine::Draw() for the current State.
  virtual void Draw() = 0;

  // Called by Engine::Update() for the current State.
  virtual void Update();

  // Draw anything to be overlaid on top of everything else. E.g. Title, some text,
  // stats, etc.
  virtual void DrawOverlays() {}

  // Show game stats for debugging. E.g. number of polygons, etc.
  virtual void ShowStats() {}

  virtual void ClearScrollText();

  virtual PCamera GetCamera() { return 0; }

  virtual void ReceiveMessage(const Message& m);

  virtual void CallbackSongFinished(const std::string& file) {}

  // Called when a Cut Scene ends.
  virtual void FinishCutScene() {}

  // Called when player is dead.
  virtual void GameOver();


  // Input
  // -----
  // Based on an N64 controller. Each button has a function to call when it is
  // pressed or released. There are also two functions for setting the state
  // of each of the analogue joystick axes.

  // Controller Buttons: true means button has been pressed; false
  // means button has been released.
  virtual void Red(bool) {}
  virtual void Blue(bool) {}
  virtual void Green(bool) {}
  virtual void YellowUp(bool) {}
  virtual void YellowDown(bool) {}
  virtual void YellowLeft(bool) {}
  virtual void YellowRight(bool) {}

  // Trigger.
  virtual void Z(bool) {} 

  // '+' - shaped pad:
  virtual void PlusUp(bool) {}
  virtual void PlusDown(bool) {}
  virtual void PlusLeft(bool) {}
  virtual void PlusRight(bool) {}

  // Analogue joystick X position: 0 is centred; -1 is fully left;
  //  +1 is fully right.
  virtual void JoyX(float f) {}

  // Analogue Y position: 0 is centred; -1 is fully back; +1 is fully forward.
  virtual void JoyY(float f) {}

  virtual void MousePos(int x, int y) {}

  // Default implementation calls Red(true) if mouse button is pressed and
  //  released within the same state.
  virtual void MouseButton(bool down, bool ctrl, bool shift);

  // Right mouse button: added for POOL.
  // MACOSX: ctrl is used to simulate right button, so this parameter should
  // not be used.
  virtual void MouseRButton(bool down, bool ctrl, bool shift) {}

  // Mouse middle button: added for POOL.
  virtual void MouseMidButton(bool down, bool ctrl, bool shift) {}

  // Keyboard input.
  virtual void OnKey(char key, bool down);

  // Special Event has occurred.
  virtual void OnSpecialEvent(SpecialEvent*) {}

  virtual void AddObject(PPoolGameObject) {}

  // Return true if the Controls should be shown while this state is active.
  bool ShowControls() const;
/*
  void GoToNextState();

  typedef void (*Func)();
  void SetNextStateFunction(Func);
*/
  void SetMaxTime(float);

protected:
  // Called when m_time is greater than m_maxTime on each call to Update().
  virtual void TimerExpired() {}

protected:
  bool m_showControls;
  // This function is called by GoToNextState() if set.
  typedef void (*Func)();
  Func m_func;

  // m_time is set to 0 by SetActive(), and is incremented in Update().
  // Once m_maxTime is reached, TimerExpired() is called.
  float m_time, m_maxTime;

  // True when mouse button is pressed down; false when released.
  // This is so we can check that a mouse click was started and finished
  // within the same Engine State.
  bool m_mouseDown;
};

typedef RCPtr<EngineState> PEngineState;
}
#endif

