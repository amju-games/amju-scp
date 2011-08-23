/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: PoolBg.h,v $
Revision 1.1.2.2  2006/08/14 17:50:21  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:37  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.1  2005/08/26 22:01:50  jay
Add scrolling balls background

*/

#ifndef POOL_BG_H_INCLUDED
#define POOL_BG_H_INCLUDED

#include <vector>
#include "SolidComponent.h"
#include "Singleton.h"

namespace Amju
{
class PoolBg 
{
public:
  bool Load();
  void Draw(float alpha = 0.5f);
  void Update();

protected:
  typedef std::vector<PSolidComponent> Balls;
  Balls m_balls;
};

typedef Singleton<PoolBg> ThePoolBg;
}

#endif

