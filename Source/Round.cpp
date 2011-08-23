/*
Amju Games source code (c) Copyright Jason Colman 2000-2006
$Log: Round.cpp,v $
Revision 1.1.2.5  2007/03/27 09:18:48  jay
Debug output

Revision 1.1.2.4  2007/03/12 22:08:45  jay
More ShotLog logging

Revision 1.1.2.3  2006/08/28 22:33:18  jay
Round shot info in an attempt to remove inconsistencies between clients

*/

#include <iostream>
#include "Round.h"
#include "PoolMisc.h"
#include "ShotLog.h"

namespace Amju
{
void Round(float& f, unsigned int decimalPlaces)
{
  static const int POWERS_OF_TEN [] =
  {
    1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000
  };

  // 12.34, 1 DP => 123.4 => 123 => 12.3
  // 12.36, 1 DP => 123.6 => 124 => 12.4 

  int p = POWERS_OF_TEN[decimalPlaces]; 
  float g = f * p;
  if (g > 0)
  {
    g = (float)((int)(g + 0.5f));
  }
  else
  {
    g = (float)((int)(g - 0.5f));
  }
  float f1 = g / (float)p;

#ifdef _DEBUG
std::cout << "ROUND: " << f << " (" << decimalPlaces << ") => " << f1 << "\n";
#endif

  f = f1;
}

void Round(GameState::PlayerInfo* pInfo)
{
    Round(pInfo->m_golfStroke.m_yRot, 1);
    Round(pInfo->m_golfStroke.m_vertVel, 1);
    Round(pInfo->m_golfStroke.m_horVel, 1);

    // DON'T round accel: it isn't stored on the server, we always use the
    // same value in all clients.
    // If we round here the other clients will NOT have the same rounded
    // value!!!

    Round(pInfo->m_golfStroke.m_english, 2);
    Round(pInfo->m_golfStroke.m_drawRoll, 2);
    VertexBase v = pInfo->m_golfStroke.GetCueBallPos();
    Round(v.x, 2);
    Round(v.y, 2);
    Round(v.z, 2);
    pInfo->m_golfStroke.SetCueBallPos(v);
    // Make sure cue ball starts at the same position on all clients
    Orientation o = *(GetBall()->GetOrientation());
    o.SetVertex(v);
    GetBall()->SetOrientation(o);

#ifdef POOL_ONLINE
    ShotLog("place cue ball 20");
#endif
}
}


