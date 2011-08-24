/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: PoolBg.cpp,v $
Revision 1.1.2.2  2006/08/14 17:50:21  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:37  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.1  2005/08/26 22:01:50  jay
Add scrolling balls background

*/

#include <iostream>
#include "Common.h"
#include "PoolBg.h"
#include "StringUtils.h"
#include "Engine.h"

namespace Amju
{
bool PoolBg::Load()
{
  int numBalls = 15;
  for (int i = 0; i < numBalls; i++)
  {
    std::string s = "ball";
    s += ToString(i + 1);
    s += ".comp";
#ifdef BG_DEBUG
std::cout << "POOL BG: Load " << s.c_str() << "\n";
#endif

    PSolidComponent pComp = SolidComponent::LoadSolid(s);
    if (!pComp.GetPtr())
    {
#ifdef BG_DEBUG
std::cout << "POOL BG: FAILED TO LOAD " << s.c_str() << "\n";
#endif
      return false;
    }
    // Set the orientation
    static const float XMAX = 5.0f;
    static const float YMAX = 4.0f;
    static float x = -XMAX;
    static float y = -YMAX;

    pComp->GetOrientation()->SetX(x);
    x += 3.0f;

    static float xoff = 1.0f;
    if (x > XMAX)
    {
      x = -XMAX + xoff;
      if (xoff == 0)
      {
        xoff = 1.0f;
      }
      else
      {
        xoff = 0;
      }
      y += 3.0f;
    }

    pComp->GetOrientation()->SetY(y);

    pComp->GetOrientation()->SetZ(-3.0f + (float)(i % 3));

    m_balls.push_back(pComp);
  }
  return true;
}

void PoolBg::Draw(float alpha)
{
  Update();

  Engine::Instance()->PushColour(1.0f, 1.0f, 1.0f, alpha);
  for (unsigned int i = 0; i < m_balls.size(); i++)
  {
    AmjuGL::PushMatrix();
    m_balls[i]->GetOrientation()->Draw();
    m_balls[i]->Draw();
    AmjuGL::PopMatrix();
  }
  Engine::Instance()->PopColour();

  // Clear the depth buffer so all drawing goes over the top of this BG.
  //glClear(AmjuGL::AMJU_DEPTH_READ);
}

void PoolBg::Update()
{
  // Move all balls up. Wrap around when ball is off top.
  // (check frustum intersect ?)

  static const float VEL = 4.0f;
  static float dr = 0;
  float dt = Engine::Instance()->GetDeltaTime();
  dr += 250.0f * dt;
  for (unsigned int i = 0; i < m_balls.size(); i++)
  {
    PSolidComponent p = m_balls[i];
    
    p->GetOrientation()->SetXRot(dr);
    p->GetOrientation()->SetYRot(p->GetOrientation()->GetYRot() + (float)i * 20.0f * dt);

    p->GetOrientation()->SetY(p->GetOrientation()->GetY() + dt * VEL);

    static const float MAXY = 6.0f;
    if (p->GetOrientation()->GetY() > MAXY)
    {
      p->GetOrientation()->SetY(-MAXY);
    }
  }
}
}


