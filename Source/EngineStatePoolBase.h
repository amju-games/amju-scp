/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStatePoolBase.h,v $
Revision 1.1.2.2  2007/12/11 19:42:40  jay
Clean up; add string for online status msgs

Revision 1.1.2.1  2006/08/14 17:50:13  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:25  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.8.9  2006/04/11 17:02:30  jay
Fix bad "m_" on static member to "s_"

Revision 1.1.8.8  2005/09/20 13:14:53  jay
Add Extra hearts to players' totals

Revision 1.1.8.7  2005/08/26 21:18:12  jay
Added SGetCamera() - so camera is available to all code.

Revision 1.1.8.6  2005/06/19 21:28:28  jay
Move HUD drawing code to here

Revision 1.1.8.5  2005/05/15 17:13:10  jay
Add Big Text for Pool

Revision 1.1.8.4  2005/05/08 17:27:08  jay
Add pool cue

Revision 1.1.8.3  2005/04/03 16:29:49  jay
Added entirely separate camera for birds eye mode.

Revision 1.1.8.2  2005/04/01 05:59:21  jay
Added flag for balls to set when not in view frustum
Added rough pool HUD

Revision 1.1.8.1  2005/03/17 20:56:07  jay
Added statics to handle birds eye view

Revision 1.1  2004/09/08 15:43:04  jay
Added to repository
  
*/

#if !defined(ENGINE_STATE_GOLF_BASE_H_INCLUDED)
#define ENGINE_STATE_GOLF_BASE_H_INCLUDED

#include "EngineRunningBase.h"
#include "Orientation.h"
#include "TexturedQuad.h"
#include "PoolCue.h"
#include "ExplosionEffect.h"

namespace Amju
{
class FreeMovingSolidGameObject;
class Trajectory;
class PoolHole;

class EngineStatePoolBase : public EngineRunningBase
{
public:

  static void Init();

  static Trajectory* GetTrajectory();

  EngineStatePoolBase();

  virtual void Draw();
  virtual void DrawOverlays();
  virtual void Update();
  virtual void SetActive(bool active);
  virtual void OnNewLevel();
  virtual void OnNewGame();

  virtual bool Load();

  virtual void DoCameraWork();
  virtual PCamera GetCamera();

  // Camera is static; above function calls this one; any object may need
  // to know the position of the camera.
  static PCamera SGetCamera();

  static void SetBallSize(float f) { s_ballSize = f; }

  static void SetMovingBallNotInFrustum();

  // Big Text
  static void DrawBigText();
  static void SetBigText(const char *);

protected:
  // Make sure no single player is drawn. Instead all the players are
  // standard game objects.
  virtual void DrawPlayer();

  // Don't do any collision testing for player.
  virtual void PlayerCollisionTest();

  static void DrawPlayerName();
  static void DrawScore();
  static void DrawBallsLeft();
  static void DrawBonusesLeft();
  // POOL
  static void DrawBallsPotted();

  // Draws ball we are aiming for, or for UK pool, the colour of ball (or the
  // black).
  void DrawHudBall();

public:
  // Pass in text as player name may be replaced by course name for one-player game.
  static void CreatePlayerName(const std::string& text);
  static void CreateScore();
  static void CreateBallsLeft();
  static void CreateBonusesLeft();

  static void AddExtraHearts(int playerId, int numHearts);

public:
  static void SetBirdsEye(bool b);
  static bool IsBirdsEye();

protected:
  static PCamera s_pCamera;

  // POOL: Bird's eye camera - simple camera, doesn't move at all.
  static PCamera s_pBirdsEyeCam;
 
  // Trajectory planner: calculates/shows how ball will travel.
  static RCPtr<Trajectory> s_pTrajectory;

  static PSolidComponent s_pPlayerNameText;
  // coords for player name
  static float s_left;
  static float s_top;
  static float s_z;

  static PSolidComponent s_pScoreText;
  static PSolidComponent s_pBallText;
  static PSolidComponent s_pBonusText;
  static TexturedQuad s_tqBall;
  static float s_ballSize; // size of ball TQ
  static bool s_flashBalls;

  // POOL: birds-eye camera toggle. In birds-eye mode the camera always looks
  // straight down at the cue ball.
  static bool s_isBirdsEye;

  // Set this flag when a moving ball is outside the view frustum.
  // This tells us to pull the camera back so we can see it.
  static bool s_movingBallNotInFrustum;

  // Pool cue - used to show player shot.
  static PoolCue s_cue;

  // Big 3D text which floats above the table.
  static PSolidComponent s_pBigText;
  // Time for which big text has been displayed.
  static float s_bigTextTime;
  // Explosion around big text
  static RCPtr<ExplosionEffect> s_pBigTextExpl;

  // No of extra hearts to award per player. 
  // These are added to each player's total in DrawBonusesLeft().
  static int s_extraHearts[2];

  // String showing number of players currently online
  std::string m_playersOnlineStr;
};
}

#endif

