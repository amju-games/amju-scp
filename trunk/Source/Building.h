/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Building.h,v $
Revision 1.1  2004/09/08 15:42:34  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_BUILDING_H_INCLUDED)
#define SCHMICKEN_BUILDING_H_INCLUDED

#include <vector>
#include <string>
#include "Orientation.h"
#include "BoundingBox.h"
#include "SharedPtr.h"

namespace Amju
{
//class Takeable;
class File;

// A Building is a configuration of some pieces.
class Building : public Shareable
{
public:
  // A Piece is a Type of takeable game object and some position info.
  struct Piece
  {
    Piece();
    Piece(const std::string&, const Orientation&);
    //Takeable* m_pTakeable;

    // Type of takeable: this could be an int, or simply the Solid name.
    std::string m_type;

    // Orientation is relative to the Building's origin, of course.
    Orientation m_or; // expect x-rot and z-rot to be zero
  };

public:
  Piece GetPiece(int i) const;

  int GetNumPieces() const;

  bool Load(File*);

  // If this building has a piece at the given orientation, return true and
  // set the piece type. If no piece at the orientation, return false.
  bool GetPiece(const Orientation& o, Piece* pResult);

protected:
  // Load the solid composite which defines the Solid names of pieces making
  // up the building.
  bool LoadComposite(File*);

  // Get the box enclosing the building.
  void CalculateBoundingBox();

protected:
  typedef std::vector<Piece> Pieces;
  Pieces m_pieces;

  // Probably don't need this - buildings don't have a fixed location!
  Orientation m_origin; // ????

  // Box around building if it was built at the origin.
  BoundingBox m_bbox;
};

typedef SharedPtr<Building> PBuilding;
}

#endif
