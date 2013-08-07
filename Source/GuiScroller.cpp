/*
Amju Games source code (c) Copyright Jason Colman 2000-2006
*/

#include "GuiScroller.h"
#include "Engine.h"

namespace Amju
{
PoolGuiScroller::PoolGuiScroller()
{
  m_xVel = 0;
  m_yVel = 0;
  m_xAccel = 0;
  m_yAccel = 0;
}

void PoolGuiScroller::DrawImpl()
{
  Update();
  PoolGuiComposite::DrawImpl();
}

void PoolGuiScroller::Update()
{
  float dt = Engine::Instance()->GetDeltaTime();
  m_xVel += dt * m_xAccel;
  if (m_xVel < 0 && m_xAccel < 0)
  {
    m_xVel = 0;
    m_xAccel = 0;
  }
  else if (m_xVel > 0 && m_xAccel > 0)
  {
    m_xVel = 0;
    m_xAccel = 0;
  }

  m_yVel += dt * m_yAccel;
  if (m_yVel < 0 && m_yAccel < 0)
  {
    m_yVel = 0;
    m_yAccel = 0;
  }
  else if (m_yVel > 0 && m_yAccel > 0)
  {
    m_yVel = 0;
    m_yAccel = 0;
  }

  int size = m_children.size();
  for (int i = 0; i < size; i++)
  {
    float x, y;

    m_children[i]->GetRelPos(&x, &y);
    x += dt * m_xVel;
    y += dt * m_yVel;
    m_children[i]->SetRelPos(x, y);
  }
}

void PoolGuiScroller::Scroll(PoolGuiScroller::ScrollDir dir, float distance, float time)
{
  static const float VEL = Engine::Instance()->GetConfigFloat("scroll-vel");
  static const float ACCEL = Engine::Instance()->GetConfigFloat("scroll-accel");

  switch (dir)
  {
  case LEFT:
    m_xVel = -VEL;
    m_xAccel = ACCEL;
    break;
  case RIGHT:
    m_xVel = VEL;
    m_xAccel = -ACCEL;
    break;
  case UP:
    m_yVel = -VEL;
    m_yVel = ACCEL;
    break;
  case DOWN:
    m_yVel = VEL;
    m_yVel = -ACCEL;
    break;
  }
}
}


