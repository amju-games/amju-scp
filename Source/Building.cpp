/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Building.cpp,v $
Revision 1.1  2004/09/08 15:42:34  jay
Added to repository
  
*/

#include "Building.h"
#include "File.h"

namespace Amju
{
Building::Piece::Piece()
{
}

Building::Piece::Piece(const std::string& type, const Orientation& o)
{
  m_type = type;
  m_or = o;
}

Building::Piece Building::GetPiece(int i) const
{
  return m_pieces[i];
}

int Building::GetNumPieces() const
{
  return m_pieces.size();
}

void Building::CalculateBoundingBox()
{
}

bool Building::Load(File* pf)
{
  // Get building name. 
  // The format for buildings is exactly the same as for Composites!
  // So any Composite can be a building if we want.
  std::string buildingName;
  if (!pf->GetDataLine(&buildingName))
  {
    pf->ReportError("Expected building name.");
    return false;
  }
  File bfile;
  if (!bfile.OpenRead(buildingName))
  {
    pf->ReportError("Failed to open building file.");
    return false;
  }

  if (!LoadComposite(&bfile))
  {
    pf->ReportError("Failed to load Building.");
    return false;
  }

  // Create  a bounding box around the composite. 
  // This will let us work out if a building will fit in a given volume.
  CalculateBoundingBox();

  // Load extra AI Cheat info.
  // Get exit point of building. I.e. the location you would be at if
  // you walked up the building placed at the origin.
  // TODO it's quite possible to work this out..


  return true;
}

bool Building::LoadComposite(File* pf)
{
  // Get number of pieces
  int numPieces = 0;
  if (!pf->GetInteger(&numPieces))
  {
    pf->ReportError("Expected number of pieces in building.");
    return false;
  }

  for (int i = 0; i < numPieces; i++)
  {
    // Get piece (takeable) type
    // Type is a string, just needs to be unique for each piece size/shape.
    std::string type;
    if (!pf->GetDataLine(&type))
    {
      pf->ReportError("Expected piece type.");
      return false;
    }

    // Get orientation
    Orientation o;
    if (!o.Load(pf))
    {
      pf->ReportError("Failed to load piece orientation.");
      return false;
    }
    
    m_pieces.push_back(Piece(type, o));
  }
  // TODO sort container here for faster searching ?
  return true;
}

bool Building::GetPiece(const Orientation& o, Building::Piece* pResult)
{
  // TODO better search - e.g. binary
  for (Pieces::iterator it = m_pieces.begin(); it != m_pieces.end(); ++it)
  {
    if (it->m_or == o)
    {
      *pResult = *it;
      return true;
    }
  }
  return false;
}

}
