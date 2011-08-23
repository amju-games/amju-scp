/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: PoolPuck.cpp,v $
Revision 1.1.2.2  2006/08/14 17:50:24  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:39  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.1  2005/09/23 20:00:14  jay
Add 'puck' - non-pottable block which slides around.

*/

#include "Common.h"
#include "PoolPuck.h"

namespace Amju
{
GameObject* CreatePoolPuck()
{
  return new PoolPuck;
}

const char* PoolPuck::GetTypeName() const
{
  return "pool-puck";
}

void PoolPuck::HandleObjectCollision(GameObject* pObj)
{
  PoolBall::HandleObjectCollision(pObj);
}

void PoolPuck::Draw()
{
  AmjuGL::PushMatrix();
  GetOrientation()->Draw();
  m_pSolid->Draw();
  AmjuGL::PopMatrix();
}
}

