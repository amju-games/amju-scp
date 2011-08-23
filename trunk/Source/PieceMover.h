/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PieceMover.h,v $
Revision 1.1  2004/09/08 15:42:40  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_PIECE_MOVER_H_INCLUDED)
#define SCHMICKEN_PIECE_MOVER_H_INCLUDED

#include "PieceFootprint.h"
#include "GameObject.h"
#include "Orientation.h"
#include "HeightServer.h"
#include "ExplosionEffect.h"
#include "AiCooperationGroup.h"

namespace Amju
{
class CharacterGameObject;
class Takeable;

// Mixin class for characters which can move pieces around.
// I.e. player and NPC opponents.
// Maybe non-local human players in future.
class PieceMover
{
public:
  PieceMover();
  virtual ~PieceMover();

  // Callbacks: by default these do nothing.
  // self is true if this PieceMover is responsible for moving the piece.
  virtual void NotifyPieceTaken(Takeable*, bool self);
  virtual void NotifyPieceDropped(Takeable*, bool self);
  virtual void NotifyPieceRotated(Takeable*, bool self);

  void SetPMCharacter(CharacterGameObject* pCharacter);

  CharacterGameObject* GetPMCharacter();

  // Call on new level etc
  void ResetPieceMover();

  // Call to stop Action button making piece rotate.
  void ResetPieceCanRotate();

  // Each of these functions is called when the Action button is pressed.
  // Returns true if the action was successful.
  bool ActionTakePiece();
  bool ActionDropPiece();
  bool ActionRotatePiece();
  // Call to take a specific object. This is to be called by AI which
  // should hopefully not try to take something inappropriate.
  // Returns true if Take was successful.
  bool ActionTakePiece(Takeable* pTakeable);

  // Draw footprint of piece carried, if any. This lets the user see where the
  // piece will go if it is put down.
  void DrawPieceFootprint();
  void UpdatePieceFootprint();

  // Returns true if the character (set with SetPMCharacter) is standing on 
  // the given piece.
  bool CharacterIsOnPiece(Takeable* pTakeable);

  bool IsCarryingPiece() const;

  // Draw piece being carried if any.
  // Best called after everything else has been drawn if translucent.
  void DrawTakenPiece();

  // Identify the currently appropriate action for this piece mover.
  // This is used to work out what should happen when the player presses
  // the action button.
  enum Action { NONE, DROP, TAKE, ROTATE };
  Action GetCurrentAction();

  // Return the currently carried piece.
  // If called we assume that the PieceMover is definitely carrying something.
  Takeable* GetCurrentPiece();

  // Return the most recently placed piece, or zero.
  Takeable* GetLastPiecePlaced();

#if defined(_DEBUG)
  // For forcing a particular behaviour, the last piece to be placed can be set.
  // Currently only needed for testing.
  void SetLastPiecePlaced(Takeable*);
#endif

  // Each Piece Mover belongs to a cooperation group.
  // Members of the same group work together.
  CooperationGroup* GetCooperationGroup();

  void SetCooperationGroup(CooperationGroup*);

protected:
  // Intended for wavs etc
  void CantDrop();
  void CantTake();
  void CantRotate();

  bool IsOkToDropPiece();

  // These functions call their static equivalents in EngineRunningVisGraph,
  // adding an object to apply the function to.
  bool PieceIntersects(Takeable* pObj);
  bool PieceSupportsOthers(Takeable* pTakeable);
  bool AllPieceCornersSupported(Takeable* pTakeable);

  // Return piece we would take if Action button pressed now.
  // (for player)
  Takeable* DecideTakeablePiece();

  bool LoadExplosion();

protected:
  CharacterGameObject* m_pPMCharacter;

  // If true, we just dropped a takeable, so pressing the action button again
  // will rotate it.
  bool m_canRotatePiece;
  // Points to piece carried.
  // TODO This should be a takeable to avoid dynamic_casts
  PGameObject m_pRotateablePiece;

  // Piece last placed. TODO Use SharedPtr ?
  Takeable* m_pLastPiece;

  // If true, we are carrying a piece.
  bool m_isCarrying;

  // Last 'snapped-to-grid' orientation of carried piece. 
  // This is useful as we only need to do intersection test when the snapped
  // position changes.
  Orientation m_snappedPiecePos;
  // HeightServer storing last snapped position. This is used to draw the 
  // piece footprint.
  HeightServer m_snappedPieceHs;

  PieceFootprint m_pieceFootprint;

  // Set if we find that the piece can't be dropped in the current position.
  bool m_cantDrop;

  // Height of carried piece if any
  float m_pieceHeight;

  // 'Explosion' effects when piece dropped
  enum { NUM_EXPLOSIONS = 4 };
  ExplosionEffect m_explosion[NUM_EXPLOSIONS]; // one at each corner of the piece

  PCooperationGroup m_pCoopGroup;
};
}

#endif

