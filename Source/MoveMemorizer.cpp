/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: MoveMemorizer.cpp,v $
Revision 1.1  2004/09/08 15:42:40  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "MoveMemorizer.h"

namespace Amju
{
SingleMoveMemorizer::SingleMoveMemorizer()
{
}

void SingleMoveMemorizer::Clear()
{
  m_moves.clear();
}

void SingleMoveMemorizer::AddMove(const MemMove& m)
{
  m_moves.push_back(m);
}

int SingleMoveMemorizer::Size()
{
  return m_moves.size();
}

MemMove SingleMoveMemorizer::GetMove(int i)
{
  return m_moves[i];
}
}
