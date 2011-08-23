/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: AiNotifier.h,v $
Revision 1.1  2004/09/08 15:43:01  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_AI_NOTIFIER_H_INCLUDED)
#define SCHMICKEN_AI_NOTIFIER_H_INCLUDED

#include <vector>
#include "Singleton.h"
#include "PieceMover.h"

namespace Amju
{
class Takeable;

// Notifies AIs whenever a piece is taken or dropped.
class SingleNotifier
{
public:
  typedef std::vector<PieceMover*> Pms;

public:
  // Register a piece mover to get notifications.
  void Register(PieceMover*);

  // When a Piece Mover changes a piece's state, it calls one of these 
  // functions. Registered Piece movers are then notified.
  void NotifyPieceTaken(Takeable*, PieceMover*);
  void NotifyPieceDropped(Takeable*, PieceMover*);
  void NotifyPieceRotated(Takeable*, PieceMover*);

  const Pms& GetAllPms() const;

protected:
  Pms m_pms;
};

typedef Singleton<SingleNotifier> Notifier;
}

#endif
