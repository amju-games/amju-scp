/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PieceFootprint.h,v $
Revision 1.1  2004/09/08 15:42:40  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_PIECE_FOOTPRINT_H_INCLUDED)
#define SCHMICKEN_PIECE_FOOTPRINT_H_INCLUDED

#include <vector>
#include "Orientation.h"
#include "Polygon.h"
#include "Attrib.h"

namespace Amju
{
class VisibleGameObject;
class Texture;
class File;
class HeightServer;

class PieceFootprint
{
public:
  PieceFootprint();

  static void SetTexture(Texture* pTexture);

  void Draw();

  void SetPiece(VisibleGameObject*);

  void SetOrientation(const Orientation&);

  void SetHeightServer(const HeightServer*);

  // Build list of polys.
  void BuildList();

protected:
  void BuildVertex(Polygon* pPoly, float s, float t, float x, float y, float z);
  void BuildSquare(float x, float z, float h);

protected:
  typedef std::vector<Polygon> Polys;
  Polys m_polys;

  static Texture* s_pTexture;
  VisibleGameObject* m_pTakeable;
  Orientation m_or;
  const HeightServer* m_pHs;

  PAttrib m_pAttrib;
};
}

#endif


