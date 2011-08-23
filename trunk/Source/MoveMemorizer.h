/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: MoveMemorizer.h,v $
Revision 1.1  2004/09/08 15:42:40  jay
Added to repository
  
*/

#if !defined(MOVE_MEMORIZER_H_INCLUDED)
#define MOVE_MEMORIZER_H_INCLUDED

#include <vector>
#include "Orientation.h"
#include "Singleton.h"

namespace Amju
{
// Remember all moves made by all characters in a level. 
// These can then be played back.
struct MemMove
{
  MemMove(int id, const Orientation& orBefore, const Orientation& orAfter) :
    m_id(id), m_orBefore(orBefore), m_orAfter(orAfter)
  {
  }

  int m_id;
  Orientation m_orBefore;
  Orientation m_orAfter;
};

class SingleMoveMemorizer
{
public:
  SingleMoveMemorizer();
  void Clear();
  void AddMove(const MemMove&);
  int Size();
  MemMove GetMove(int i);

protected:
  typedef std::vector<MemMove> Moves;
  Moves m_moves;
};

typedef Singleton<SingleMoveMemorizer> MoveMemorizer;
}

#endif
