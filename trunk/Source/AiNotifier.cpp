/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: AiNotifier.cpp,v $
Revision 1.1  2004/09/08 15:43:01  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "AiNotifier.h"
#include "EngineRunningVisGraph.h"

namespace Amju
{
void SingleNotifier::Register(PieceMover* p)
{
  m_pms.push_back(p);
}

const SingleNotifier::Pms& SingleNotifier::GetAllPms() const
{
  return m_pms;
}

void SingleNotifier::NotifyPieceTaken(Takeable* piece, PieceMover* pSource)
{
  if (piece)
  {
    EngineRunningVisGraph::RecalcVisGraphPieceTaken(piece);
  }

  for (Pms::iterator it = m_pms.begin(); it != m_pms.end(); ++it)
  {
    PieceMover* p = *it;
    if (p != pSource)
    {
      p->NotifyPieceTaken(piece, false);
    }
  }
  pSource->NotifyPieceDropped(piece, true);
}

void SingleNotifier::NotifyPieceDropped(Takeable* piece, PieceMover* pSource)
{
  if (piece)
  {
    EngineRunningVisGraph::RecalcVisGraphPieceDropped(piece);
  }

  for (Pms::iterator it = m_pms.begin(); it != m_pms.end(); ++it)
  {
    PieceMover* p = *it;
    if (p != pSource)
    {
      p->NotifyPieceDropped(piece, false);
    }
  }

  if (pSource)
  {
    pSource->NotifyPieceDropped(piece, true);
  }
}

void SingleNotifier::NotifyPieceRotated(Takeable* piece, PieceMover* pSource)
{
  for (Pms::iterator it = m_pms.begin(); it != m_pms.end(); ++it)
  {
    PieceMover* p = *it;
    if (p != pSource)
    {
      p->NotifyPieceRotated(piece, false);
    }
  }
  pSource->NotifyPieceDropped(piece, true);
}

}
