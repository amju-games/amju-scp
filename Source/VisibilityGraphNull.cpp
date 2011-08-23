/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: VisibilityGraphNull.cpp,v $
Revision 1.1  2004/09/08 15:42:59  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "VisibilityGraphNull.h"

namespace Amju
{
bool VisibilityGraphNull::PathExists(
    const VertexBase& v1, 
    const VertexBase& v2, 
    float r) const
{
  Assert(0); // you can't do path finding with this NULL visibility graph!
  return false;
}

void VisibilityGraphNull::AddPoint(const VertexBase&)
{
}

void VisibilityGraphNull::CreateGraph(float losWidth)
{
}

void VisibilityGraphNull::UpdatePieceTaken(Takeable*, float losWidth)
{
}

void VisibilityGraphNull::UpdatePieceDropped(Takeable*, float losWidth)
{
}

void VisibilityGraphNull::UpdatePieceRotated(Takeable*, float losWidth)
{
}

void VisibilityGraphNull::Clear()
{
}

void VisibilityGraphNull::ClearVisPoint(const VertexBase&)
{
}

void VisibilityGraphNull::AddLink(const VertexBase& v1, const VertexBase& v2)
{
}

void VisibilityGraphNull::RemoveLink(const VertexBase& v1, const VertexBase& v2)
{
}
}

