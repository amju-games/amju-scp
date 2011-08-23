/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PolyDrawOpenGL.h,v $
Revision 1.1  2004/09/08 15:42:54  jay
Added to repository
  
*/

#if !defined(POLY_DRAW_OPENGL_H_INCLUDED)
#define POLY_DRAW_OPENGL_H_INCLUDED

#include "PolyDrawer.h"

namespace Amju
{
class Polygon;

class PolyDrawOpenGL : public PolyDrawer
{
public:
  virtual void Draw(Polygon* poly);
};
}
#endif

