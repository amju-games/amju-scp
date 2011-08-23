/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: VisibilityGraphNull.h,v $
Revision 1.1  2004/09/08 15:43:00  jay
Added to repository
  
*/

#if !defined(VISIBILITY_GRAPH_NULL_H_INCLUDED)
#define VISIBILITY_GRAPH_NULL_H_INCLUDED

#include "VisibilityGraphWithObjects.h"

namespace Amju
{
class VisibilityGraphNull : public VisibilityGraphWithObjects
{
  virtual bool PathExists(
    const VertexBase& v1, 
    const VertexBase& v2, 
    float r) const;
  virtual void AddPoint(const VertexBase&);
  virtual void CreateGraph(float losWidth);
  virtual void UpdatePieceTaken(Takeable*, float losWidth);
  virtual void UpdatePieceDropped(Takeable*, float losWidth);
  virtual void UpdatePieceRotated(Takeable*, float losWidth);
  virtual void Clear();
  virtual void ClearVisPoint(const VertexBase&);

protected:
  virtual void AddLink(const VertexBase& v1, const VertexBase& v2);
  virtual void RemoveLink(const VertexBase& v1, const VertexBase& v2);

};
}

#endif

