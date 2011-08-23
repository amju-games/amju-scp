/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ExpandingBillboard.h,v $
Revision 1.1  2004/09/08 15:42:49  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_EXPANDING_BILLBOARD_H_INCLUDED)
#define SCHMICKEN_EXPANDING_BILLBOARD_H_INCLUDED

#include "BillBoard.h"

namespace Amju
{
class ExpandingBillboard : public BillBoard
{
public:
  ExpandingBillboard();
  virtual void Draw();
  virtual bool Load(File* pf);

  void SetExpansionVel(float vel) { m_expansionVel = vel; }

protected:
  float m_expansionVel;
};
}
#endif

