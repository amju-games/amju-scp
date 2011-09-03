/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineRunningBase.h,v $
Revision 1.1  2004/09/08 15:42:48  jay
Added to repository
  
*/

#if !defined(ENGINE_RUNNING_BASE_H_INCLUDED)
#define ENGINE_RUNNING_BASE_H_INCLUDED

#include "EngineState.h"
#include "Player.h"

namespace Amju
{
class Level;
class PoolGameObject;
class CharacterGameObject;

class EngineRunningBase : public EngineState
{
public:
  EngineRunningBase();

  virtual void OnNewRoom();

  void DrawLensflare();
  void DrawSkybox();

  virtual void Draw();

  virtual void Update();

  virtual CharacterGameObject* GetPlayer();

  Level* GetLevel();

  // Do "LookAt" function. 
  virtual void DoCameraWork();

  void SetPlayerRoom(int roomId);

protected:

  virtual void SetUpPlatforms();

  virtual void DrawPlayer();

  // Draw things which interact with player.
  // Draws translucent or non-translucent objects, as specified by flag.
  void DrawGameObjects(bool translucent);

  void DrawGameObjectShadows();

  // Update one Game Object using collision data from all other Game 
  // Objects in the same room.
  void UpdateGameObjectHeightServer(VisibleGameObject* pVgo);

  // Test for player collision with Game Objects.
  virtual void PlayerCollisionTest();

  // Test for G.O./G.O. collision
  void ObjectCollisionTest(PoolGameObject* pObj);

  // Draw shadow under player/other characters
  void DrawCharacterShadow(CharacterGameObject* pCharacter);

  // Returns true if camera is underwater.
  bool IsUnderwater();

  void UpdateGameObjects();

  // Draw static scenery and game objects
  void DrawForeground();

  // Draw the stuff which should be furthest in the depth-buffer
  void DrawBackground();

protected:

  SharedPtr<Level> m_pLevel; // Point to current level - owned by LevelServer.

  // Number of polys in static scene drawn.
  int m_scenePolys;

};
}

#endif

