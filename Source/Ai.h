/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Ai.h,v $
Revision 1.1  2004/09/08 15:43:01  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_AI_H_INCLUDED)
#define SCHMICKEN_AI_H_INCLUDED

#include "Npc.h"
#include "PieceMover.h"
#include "Strategizer.h"
#include "Finder.h"
#include "Behaviour.h"

namespace Amju
{
// Supposedly-intelligent opponent. Tries to beat player home.
class Ai : public Npc, public PieceMover
{
public:
  Ai();

  virtual const char* GetTypeName() const;

  virtual bool Load(File*);

  virtual void Update();

  virtual void Draw();

  virtual void SetTarget(GameObject*);

  virtual void OnRoomEntry();

  virtual void SetState(State newState);

  // Called by currently active Behaviour if it fails to do its work.
  // A new strategy must then be set.
  void Failed();
 
  // Called if piece is successfully placed. The current strategy 
  // may then be continued, or a new one started.
  void Succeeded();

  int GetPieceId() const;

  const VertexBase& GetPiecePos() const;

  float GetPieceYRot() const;

  void SetPieceId(int id);

  void SetPiecePos(const VertexBase& v);

  void SetPieceYRot(float yrot);

  PGameObject GetCurrentGoal();

  // Get the last goal we reached - may return zero if no goal reached 
  // since goals were reset.
  PGameObject GetPrevGoal();

  // Called when this AI reaches its current goal. A new goal becomes the
  // current goal.
  void CurrentGoalReached();

  // If a piece is picked up or dropped, all AIs are notified. This is so
  // the current task/strategy can be re-evaluated if necessary.
  // NB An AI is NOT notified when IT is responsible for taking/dropping 
  // the piece.
  virtual void NotifyPieceTaken(Takeable*, bool self);
  virtual void NotifyPieceDropped(Takeable*, bool self);
  virtual void NotifyPieceRotated(Takeable*, bool self);

  Finder* GetFinder(int height);

  // Get the number of pieces you ideally would like to place at a given 
  // height, relative to the goal height. If dh is positive, height is 
  // lower than the goal height.
  // This may depend on the character type.
  // Used by Evaluators.
  int GetIdealNumPiecesAtHeight(float dh);

protected:
  void ChooseStrategy();

  bool LoadGoalList(File*);

  bool LoadFinders(File*);

  void PlayerLoseTest();

  void ReachedHomeEffect();

  // Get a good place to dump the current piece.
  VertexBase GetDumpLocation();

  void RefreshStrategies();

protected:
  BehaviourMap m_behaviours;

  // The AI's job is to go to a piece, pick it up, take it somewhere else 
  // and then drop it there.
  // ID of piece we want to pick up.
  int m_pieceId;
  // Position where we want to place the piece.
  VertexBase m_piecePlace;
  // Required Y-rotation of piece.
  float m_pieceYRot;

  Strategizer* m_pStrategizer;

  // Each AI has a vector of Goals, where each goal is a Game Object to
  // intersect. 
  typedef std::vector<PGameObject> Goals;
  Goals m_goals;
  int m_goalIndex;

  // AI owns finders, one for each height of structure it's possible to build.
  // Not owned by a strategy, so they can be shared by all strategies.

  // Finds the next piece to place.
  // This is the current finder.

  // All  finders used by this strategy. 
  // The different finders load up different evaluator weightings and moves.
  // The finder to be used depends on the height of the goal.
  // Map heights (integerized) to Finders.
  typedef std::map<int, Finder*> Finders;
  Finders m_finders;


};
}

#endif