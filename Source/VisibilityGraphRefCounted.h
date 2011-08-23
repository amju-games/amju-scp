/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: VisibilityGraphRefCounted.h,v $
Revision 1.1  2004/09/08 15:43:00  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_VIS_GRAPH_REF_COUNTED_H_INCLUDED)
#define SCHMICKEN_VIS_GRAPH_REF_COUNTED_H_INCLUDED

#include "VisibilityGraphWithObjects.h"

namespace Amju
{
// Optimised version of base class, using ref counting to correctly
// modify links when objects are inserted or deleted.
class VisibilityGraphRefCounted : public VisibilityGraphWithObjects
{
public:
  // Rebuild graph from scratch
  virtual void CreateGraph(float losWidth);

  // Update graph due to a piece being taken
  virtual void UpdatePieceTaken(Takeable*, float losWidth);

  // Update graph due to a piece being placed
  virtual void UpdatePieceDropped(Takeable*, float losWidth);

#if defined(_DEBUG)
  bool EqualRefCounts(const VisibilityGraphRefCounted& rhs) const;
  void SweepUpZeroRefCounts();
#endif

protected:

  virtual void AddLink(const VertexBase& v1, const VertexBase& v2);

  // Really reomve the link.
  virtual void RemoveLink(const VertexBase& v1, const VertexBase& v2);

  void CreateAddPiece(Takeable*, float losWidth);

  void AddPiece(Takeable*, float losWidth);

  void RemovePiece(Takeable*, float losWidth);

  void InitRefcount(const VertexBase& v1, const VertexBase& v2);
  void IncRefcount(const VertexBase& v1, const VertexBase& v2);
  // Returns remaining ref count
  int DecRefcount(const VertexBase& v1, const VertexBase& v2);

  // Decrement ref count, remove link if count hits zero.
  void DecrementRemoveLink(const VertexBase& v1, const VertexBase& v2);


#if defined(_DEBUG)
  // called by operator<<
  virtual void Print(std::ostream& o);
#endif

protected:
  // Ref counts: for every link, a count of how many pieces have a vis.point
  // at either end of the link.
  typedef std::map<Link, int> LinkRefCount;
  LinkRefCount m_refcounts;

protected:
  // Map Pieces to the links which were added for them.
  // --------------------------------------------------
  typedef std::vector<Link> LinkVector;
  typedef std::map<Takeable*, LinkVector> PieceLinkMap;
  PieceLinkMap m_pieceLinkMap;

protected:
  void AddPieceLink(Takeable*, const Link&);
  void ResetPieceLinks(Takeable*);
  const LinkVector& GetPieceLinks(Takeable*) const;

};
}

#endif

