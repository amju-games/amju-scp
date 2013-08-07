/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineRunning.h,v $
Revision 1.1  2004/09/08 15:42:36  jay
Added to repository
  
*/

#if !defined(ENGINE_RUNNING_H_INCLUDED)
#define ENGINE_RUNNING_H_INCLUDED

#include <string>
#include "EngineRunningBase.h"
#include "SolidComponent.h"
#include "TexturedQuad.h"
#include "CharacterGameObject.h"
#include "Colour.h"
#include "PowerGuage.h"
#include "ControlStyle.h"

namespace Amju
{
class Engine;
class HeightServer;
class TextWriter;
class Message;
class WallPoly;
class HeightServer;
class Takeable;

// Engine state: game is being played and is running (i.e. isn't paused).
class EngineRunning : public EngineRunningBase
{
public:
  static const std::string Name; // ID of this state.

  EngineRunning();
  ~EngineRunning();

  virtual void SetActive(bool active);
  virtual bool Load();
  virtual void Clear();
  virtual void Draw();
  virtual void DrawOverlays();
  virtual void Update();
  virtual void GameOver();

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
  virtual void Z(bool);   // Trigger.

  virtual void MousePos(int x, int y);
  virtual void MouseButton(bool down, bool ctrl, bool shift);

  void SetControlStyle(ControlStyle* pCs);
  enum ControlStyleEnum { AMJU_ROTATE_KEYS,  AMJU_MOUSE_PICK,  AMJU_JOYSTICK,  AMJU_DIRECTIONAL_KEYS};
  void SetControlStyle(ControlStyleEnum cs);
  void SetControlStyleFromConfig();

  void SetCameraStyle(Camera* pCam);
  enum CameraEnum { AMJU_BIRDS_EYE,  AMJU_FOLLOW,  AMJU_FIRST_PERSON,  AMJU_FIXED};
  void SetCameraStyle(CameraEnum c);
  void SetCameraStyleFromConfig();

  virtual void ShowStats();

  virtual PCamera GetCamera() { return m_pCamera; }

  virtual void CallbackSongFinished(const std::string& file);

  // Yechh.. this should be a property of all characters.
  void MultPlayerSpeed(float speedMult);
  void RestorePlayerSpeed();

  // Hopefully unambiguous function names for a new game and new level.  
  virtual void OnNewLevel();
  virtual void OnNewGame();

  bool IsFirstPerson() const;

  void IncreaseLevelTime(float secs);

  void IncreaseScore(int i);

protected:

  virtual void DrawPlayer();

  void FlipPlayer(float degs);
  void RotatePlayer(float degsSec);
  
  // Draw one heart icon
  void DrawHeart(int position);

  void HandleFirstPersonCameraOnPlatform();
  void Joystick();
  void JoystickXHelper();
  void JoystickYHelper();

  // Called on new level/game, so no Chicks are running around.
  //void ResetChicks();

    // Test for player falling off/through floor 
  void PlayerFellOffWorldTest();

  // Move player back if b.sphere intersects a wall after a flip.
  void ExtractPlayerFromWall();


protected:
  // Points to the current camera.
  PCamera m_pCamera;

  // Flag - set if camera is FIRST_PERSON.
  bool m_firstPerson;

  // Remember height from which the player fell.
  float m_heightFallenFrom;

  // Usual player max forward velocity.
  float m_maxForwardVel;
  // Player walking speed.
  float m_walkForwardVel;

  // Acceleration from walk to run (kb/d pad)
  float m_playerAccel;

  // Current max player forward vel.
  float m_currentMaxForwardVel;
  float m_playerRotateVel;


  // Player health icon overlay - probably a heart ?
  TexturedQuad m_heart;

  // Window size in pixels. This is held in the Engine, storing it here as well
  // is a speed optimization.
  int m_windowX, m_windowY;


  // Display Remaining Time for Player
  RCPtr<PowerGuage> m_pTimeGuage;

  // Control style: this interprets user input into game actions.
  RCPtr<ControlStyle> m_pControlStyle;
};
}
#endif


