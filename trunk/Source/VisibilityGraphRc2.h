/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: VisibilityGraphRc2.h,v $
Revision 1.1  2004/09/08 15:43:00  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_VIS_GRAPH_RC_2_H_INCLUDED)
#define SCHMICKEN_VIS_GRAPH_RC_2_H_INCLUDED

#include "VisibilityGraphWithObjects.h"

namespace Amju
{
// This graph holds a ref count of visibility points, not links.
// The idea is to speed up the graph when adding and removing pieces by
// only making the minimal necessary changes.
class VisibilityGraphRc2 : public VisibilityGraphWithObjects
{
public:

  virtual void AddPoint(const VertexBase&);

  // Rebuild graph from scratch
  virtual void CreateGraph(float losWidth);

  // Update graph due to a piece being taken
  virtual void UpdatePieceTaken(Takeable*, float losWidth);

  // Update graph due to a piece being placed
  virtual void UpdatePieceDropped(Takeable*, float losWidth);

  virtual void Clear();

protected:

  virtual void AddLink(const VertexBase& v1, const VertexBase& v2);

  // Really reomve the link.
  // Called when a piece blocks a previously-good link.
  virtual void RemoveLink(const VertexBase& v1, const VertexBase& v2);

  void CreateAddPiece(Takeable*, float losWidth);

  void AddPiece(Takeable*, float losWidth);

  void RemovePiece(Takeable*, float losWidth);

  // Decrement ref count on vis. point, and delete all liks to it if 
  // the count hits zero.
  void RemovePoint(const VertexBase& v);


#if defined(_DEBUG)
  // called by operator<<
  virtual void Print(std::ostream& o);
#endif

protected:
  // Map visibility points to the number of pieces which created them.
  typedef std::map<VertexBase, int> VertexRcs;
  VertexRcs m_vertexRcs;
};
}

#endif

