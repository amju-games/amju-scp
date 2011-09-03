/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStatePoolSetUpShot.h,v $
Revision 1.1.2.5  2007/12/12 10:25:22  jay
Poll server when in single-player mode

Revision 1.1.2.4  2007/06/11 09:39:52  jay
Rename place ball button; add easy shoot button

Revision 1.1.2.3  2007/05/16 22:07:32  Administrator
Rename some functions to better match actions.
Try to simlify player position/rotate when state is entered - seems to be
way too complicated now.

Revision 1.1.2.2  2007/03/22 19:05:38  jay
Place ball/birds eye improvements

Revision 1.1.2.1  2006/08/14 17:50:15  jay
Rename "Pool"

Revision 1.1.2.2  2006/08/07 20:42:49  jay
Online Pool - not working yet

Revision 1.1.2.1  2006/07/24 18:20:28  jay
Moved Pool code from Pool/ to Pool/

Revision 1.5.2.1.4.15  2005/11/08 10:30:03  jay
Make 'show trajectory' flag a member; add access functions.

Revision 1.5.2.1.4.14  2005/09/23 19:38:05  jay
Add extra includes for convenience

Revision 1.5.2.1.4.13  2005/08/04 19:39:01  jay
Added CancelShot

Revision 1.5.2.1.4.12  2005/07/30 12:17:42  jay
Added undo/redo buttons and trajectory button - for practice mode only.

Revision 1.5.2.1.4.11  2005/07/17 23:03:04  jay
Use right and middle mouse buttons

Revision 1.5.2.1.4.10  2005/06/19 21:29:27  jay
Add help (show rules) button; move HUD drawing code to base class

Revision 1.5.2.1.4.9  2005/06/13 22:18:00  jay
Move IsUserControlled() to PoolMisc

Revision 1.5.2.1.4.8  2005/06/04 22:31:57  jay
Chop some dead stuff

Revision 1.5.2.1.4.7  2005/05/08 17:28:48  jay
Handle key input for zooming. In future maybe for other functions too.

Revision 1.5.2.1.4.6  2005/04/17 22:04:06  jay
Added english and draw/roll parameters to shot

Revision 1.5.2.1.4.5  2005/04/12 21:03:25  jay
Fix place ball mode so intersections are recognised.

Revision 1.5.2.1.4.4  2005/04/11 21:20:07  jay
Incorporate Place Ball mode into this state.

Revision 1.5.2.1.4.3  2005/04/03 16:35:03  jay
Incorporate Place Ball mode into this State. This was partly just to get it to
work, and also so the camera would not be changed when entering this mode. But
now we switch to birds eye mode if we are not there already, so the camera doesn't
follow the moving ball, causing a bad feedback loop.

Revision 1.5.2.1.4.2  2005/04/01 06:05:23  jay
Added cue contact position code. Needs more work.
Various minor tweaks for Pool.

Revision 1.5.2.1.4.1  2005/03/17 20:58:12  jay
Initial changes from Pool to Pool

Revision 1.5.2.1  2004/10/04 10:42:09  jay
Made m_avoidVec static, so Camera mode will user the same vectors.

Revision 1.5  2004/09/27 11:26:25  Administrator
Fix for #64 - player avoidance should be temporary.

Revision 1.4  2004/09/21 15:00:28  Administrator
Added UpdatePlayerPositions() to move players out of each others' way.

Revision 1.3  2004/09/20 13:33:25  Administrator
Changed FaceHole() so player character can look either at the hole
or another object if specified. This is so AI players can look at
the waypoint they are aiming at.

Revision 1.2  2004/09/17 14:05:06  jay
Changes to GUI buttons

Revision 1.1  2004/09/08 15:43:05  jay
Added to repository
  
*/

#if !defined(ENGINE_STATE_SET_UP_SHOT_H_INCLUDED)
#define ENGINE_STATE_SET_UP_SHOT_H_INCLUDED

#include "EngineStatePoolBase.h"
#include "Camera.h"
#include "GuiElement.h"
#include "PoolSetUpShotBehaviour.h"
#include "GuiContactPos.h"
#include "PowerGuage.h"
#include "Level.h"
#include "PoolBehaviourOnline.h"

namespace Amju
{
class FreeMovingSolidGameObject;

// This Engine State is active when a player is setting up a shot, i.e.
// examining the room, and setting the shot direction/strength etc.
class EngineStatePoolSetUpShot : public EngineStatePoolBase
{
public:
  static const char* Name; // ID of this state.

  EngineStatePoolSetUpShot();

  virtual void Draw();
  virtual void DrawOverlays();

  virtual void SetActive(bool active);
  virtual bool Load();
  virtual void Clear();
  virtual void Update();

  virtual void OnNewRoom();
  virtual void OnNewLevel();
  virtual void OnNewGame();

  // Arrow keys control camera
  virtual void PlusUp(bool);
  virtual void PlusDown(bool);
  virtual void PlusLeft(bool);
  virtual void PlusRight(bool);
 
  // Handle key presses for camera zoom, etc.
  // May use special keys to hit ball with cue, or set cue position.
  virtual void OnKey(char key, bool down);


  // Player adjusts shot angle, height and speed with the mouse.
  // -----------------------------------------------------------
  virtual void MousePos(int x, int y);
  virtual void MouseButton(bool down, bool ctrl, bool shift);
  // Right button: control cue swing ?
  virtual void MouseRButton(bool down, bool ctrl, bool shift);
  // Middle button: control cue contact pos ?
  virtual void MouseMidButton(bool down, bool ctrl, bool shift);

  // Menu
  virtual void Red(bool);

  // Take the shot. This is overridden in subclasses to go to a different
  // next state.
  virtual void TakeShotNow(float yRot, float verticalVel, float horVel, float accel,
    float english, float drawRoll);

  // Set shot power: 0 to 1 (full power)
  void SetShotPower(float);
  // Set shot angle: ADD the given angle to the current angle.
  // Returns true if new player position is valid.
  bool AddShotAngle(float degs);

  void SetShowHelp(bool b) { m_showHelpText = b; }

  void SetPlaceBallMode(bool b);
  bool GetPlaceBallMode() const;

  void SetShowTrajectory(bool b);
  bool GetShowTrajectory() const;

  // Shoot button
  static void OnShootSelected();
  static void OnShoot();

protected:
  // Update player characters so they don't intersect, if possible.
  void UpdatePlayerPositions();

  void SetStartPositions();
  void SetStartPosition(int playerIndex);

  // Make sure active player faces the ball.
  void FaceBall();

  // Make sure cue ball faces object ball or point on rail etc.
  void FaceTarget(PoolGameObject* pAnotherTarget);

  // Make the non-active character(s) look at the cue ball.
  void AllFaceCueBall();

  // Set up shot speed etc.
  void SetShotParams();

  void ResetPlayerFlags();

  // Called when this state is made active - set the active platform
  // for any object which is on one.
  void SetUpPlatforms();

  // POOL: Check new position for cue ball is ok.
  // (Checks that ball can move from current pos to newpos.)
  bool CueBallPosOk(const VertexBase& newpos);

  void MoveCueStart();
  void MoveCueFinish();
  void TakeShotStart();
  void TakeShotFinish();
  void CancelShot();
  void MoveContactPosStart();
  void MoveContactPosFinish();

  // Check for invite from another online player
  void CheckForInvite();

  // Only update status for a real game, not for demo
  virtual void UpdateOnlineStatus();

  void InitOnlineStatus();

public:

  // TODO this should go in golf misc.

  // Called by TakeShotNow() and all subclassed versions.
  static void TakeShotNowImpl(float yRot, float verticalVel, float horVel, float accel,
    float english, float drawRoll);

protected:

  // Shot direction, strength and height parameters
  // ----------------------------------------------

  // TODO maybe in Player<n> struct, one for each player
  float m_shotYRotate;
  float m_shotVerticalVel;
  float m_shotHorizontalSpeed;

  // If true, mouse is being dragged. We ignore non-drag mouse movements.
  bool m_drag;

  // Shot power meter.
  SharedPtr<PowerGuage> m_pPowerGuage;

  // If true, shot is active, i.e. user is controlling the shot.
  // If false, user is viewing the scene (possible moving the camera) but not
  // adjusting shot parameters.
  bool m_shotIsActive;
  // Time (s) shot has been active
  float m_activeTime;
  // If active time exceeds the max active time, the shot becomes inactive.
  // The idea is that is how the user can cancel the active state.
  float m_maxActiveTime;

  // This difference in mouse y-coord corresponds to max power of 
  // a shot.
  int m_maxYDiff;
  // This is the max speed the ball can be made to travel.
  float m_maxShotSpeed;
  // Acceleration of ball when it hits the ground - should be negative!
  float m_shotAccel;

  float PLAYER_OFFSET_ANGLE;

  // Behaviour for AI-players
  SharedPtr<PoolSetUpShotBehaviour> m_pBehaviour;

  // Arrow showing shot direction
  PSolidComponent m_pArrow;

  // GUI
  // ---
  PGuiElement m_pPlaceBallButton; // place ball mode
  PGuiElement m_pCameraButton; // click for camera mode
  PGuiElement m_pMenuButton;
  PGuiElement m_pHelpButton;
  // Undo/redo (play) shot - practice mode only
  PGuiElement m_pUndoButton;
  PGuiElement m_pRedoButton;
  PGuiElement m_pTrajectoryButton;

  // Simple Shoot button
  PGuiElement m_pShootButton;

  // GUI Element for controlling the contact point on the cue ball.
  SharedPtr<GuiContactPos> m_pContactGui;

  // Labels for GUI
  PSolidComponent m_pTextPower;
  PSolidComponent m_pTextDirection;
  PSolidComponent m_pTextGo;

  // Set if mouse cursor is above ball.
  bool m_cursorOnBall;

  // Song filenames
  std::vector<std::string> m_songs;
  // Currently playing song
  int m_currentSong;

  // Help text
  // ---------
  std::vector<std::string> m_helpText;
  float m_helpTimer;     // time this help message has been shown for
  float m_maxHelpTime;   // time each message is displayed for
  bool m_showHelpText;   // only show help if this is set
  int m_helpId;          // Index of current help message
  // Button - click to turn off help
  PGuiElement m_pCloseHelp;

  // Vector by which each player is moved to avoid collision with
  // any other player.
  // We need to store these vectors so we can restore the players
  // to their original positions when we leave this state.
  std::vector<VertexBase> s_avoidVec; 

  // True when mouse button clicked - then reset until next time.
  // This is used so we only get the cue contact pos. when the mouse button is
  // first pressed.
  bool m_newMouseDown;

  // If true, clicking and dragging the mouse places the cue ball.
  // By not changing state we don't reset the camera.
  bool m_placeBallMode;

  // Position of mouse cursor in 3D space.
  VertexBase m_mousePos;

  // Show or hide trajectory (the white dots showing ball paths)
  bool m_showTrajectory;

  // Polls server for opponent shot.
  PoolBehaviourOnline m_onlineBehaviour;

  // Button for when place ball mode is finished
  PGuiElement m_pPlaceBallDoneButton;

  // Set when server status has been set up for this game.
  bool m_serverStatusSetThisGame;
};
}

#endif

