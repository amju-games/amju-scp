/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PolyDrawer.h,v $
Revision 1.1  2004/09/08 15:42:54  jay
Added to repository
  
*/

#if !defined(POLYDRAWER_H_INCLUDED)
#define POLYDRAWER_H_INCLUDED

namespace Amju
{
class Polygon;

class PolyDrawer
{
public:
  virtual void Draw(Polygon* poly) = 0;
};
}
#endif

