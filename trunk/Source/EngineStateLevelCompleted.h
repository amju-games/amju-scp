/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateLevelCompleted.h,v $
Revision 1.1  2004/09/08 15:42:36  jay
Added to repository
  
*/

#if !defined(ENGINE_STATE_LEVEL_COMPLETED_H_INCLUDED)
#define ENGINE_STATE_LEVEL_COMPLETED_H_INCLUDED

#include "EngineStateDemo.h"

namespace Amju
{
// This state is entered when a Level is finished by the player.
// The idea is to show all the pieces moved, very quickly.
class EngineStateLevelCompleted : public EngineStateDemo
 // ? 
{
public:
  static const std::string Name; // ID of this state.

  EngineStateLevelCompleted();
  virtual void Draw();
  virtual void SetActive(bool active);
  virtual void DrawPlayer();
  virtual void GameOver();
  virtual void Update();

protected:
  virtual void DoCameraWork();

  void DisableAis();

protected:
  int m_pieceIndex;
  bool m_finished;
};
}

#endif

