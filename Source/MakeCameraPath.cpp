/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: MakeCameraPath.cpp,v $
Revision 1.7.6.2  2006/08/14 17:38:04  jay
Rename "Pool"

Revision 1.7.6.1  2006/07/26 21:25:17  jay
Use Pool/, not Pool/

Revision 1.7  2004/09/27 15:32:57  Administrator
Attempt to back up a bit further so the ball can be seen.

Revision 1.6  2004/09/21 15:06:06  Administrator
Improvements to camera path. Now prunes Waypoints which are not required.

Revision 1.5  2004/09/20 13:30:40  Administrator
Improvements to Camera Path:
- Uses Waypoints if they exist in the room
- Camera looks initially at golf flag, then in the direction we
 have come from, i.e. back at the path travelled.
- Camera speed is more or less constant, and configurable

Revision 1.4  2004/09/18 14:34:29  Administrator
Improve flyby state with Waypoints

Revision 1.3  2004/09/17 13:51:02  jay
Now uses waypoints

Revision 1.2  2004/09/15 09:15:37  Administrator
Minor fixes

Revision 1.1  2004/09/08 15:42:40  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "MakeCameraPath.h"
#include "FunctionCtrlPoints.h"
#include "Level.h"
#include "PoolMisc.h"
#include "PoolHole.h"
#include "PoolTeeBox.h"
#include "Waypoint.h"
#include "Engine.h"
#include "AngleCompare.h"

namespace Amju
{
// Make angle *a nearest to the previous angle.
// This is to prevent the camera swinging around in circles. 
void NormalizeAngle(float *a, float prevAngle)
{
  float bestDiff = fabs(prevAngle - *a);

  float diff = fabs(prevAngle - (*a - 360.0f));
  if (diff < bestDiff)
  {
    *a -= 360.0f;
    return;
  }

  diff = fabs(prevAngle - (*a + 360.0f));
  if (diff < bestDiff)
  {
    *a += 360.0f;
    return;
  }

  return;
}

void PruneWaypoints(std::vector<Waypoint*>* pWaypoints, Level* pLevel)
{
  if (pWaypoints->empty())
  {
#ifdef SHOW_PRUNE_DEBUG
std::cout << "Prune: no waypoints.\n";
#endif
    return;
  }

  // The algo for pruning waypoints is:
  // For each waypoint,
  //  - Is the ball closer to the previous point than the waypoint ?
  //  - Is there LOS from the ball to the previous point ?
  //  - If so stop adding waypoints.

  // "Previous point" is initially the Hole, then Waypoint 0, then 
  // Waypoint 1, etc.

  VertexBase vPrev = GetPoolHole(pLevel)->GetOrientation()->GetVertex();
  vPrev.y += 2.0f;
  VertexBase vBall = GetBall()->GetOrientation()->GetVertex();
  vBall.y += 2.0f;

  int i = 0;
  std::vector<Waypoint*>::iterator it = pWaypoints->begin();
  for ( ; it != pWaypoints->end(); ++it)
  {
    Waypoint* p = *it;
    VertexBase v = p->GetOrientation()->GetVertex();
    // Get distance from this waypoint to the previous point
    float d1 = (v - vPrev).Length();
    // Get distance from ball to the previous point
    float d2 = (vBall - vPrev).Length();

    // If ball is closer than the current Waypoint to the previous
    // point, test for LOS...
    if (d2 < d1)
    {
#ifdef SHOW_PRUNE_DEBUG
std::cout << "Prune: ball is closer than waypoint # " << i << "\n";
#endif
      // NB final parameter is 'radius' of line.
      bool losExists = !pLevel->GetScene()->LineIntersects(vBall, vPrev, 0.5f);
      if (losExists)
      {
#ifdef SHOW_PRUNE_DEBUG
std::cout << "Prune: Pruning waypoints from waypoint # " << i << "\n";
#endif
        // Prune the current waypoint and any after it.
        break;
      }
      else
      {
#ifdef SHOW_PRUNE_DEBUG
std::cout << "Prune: ...but no LOS from ball to previous point.\n";
#endif
      }
    }

    // If we get here, this waypoint is good. 
    // Update vPrev so the next time around we check if the ball is
    // reached from this waypoint.
    vPrev = v;
    ++i;
  }

  if (it == pWaypoints->end())
  {
#ifdef SHOW_PRUNE_DEBUG
std::cout << "Prune: NO waypoints were pruned.\n";
#endif
  }
  else
  {
    // Chop all waypoints from it to the end.
    pWaypoints->erase(it, pWaypoints->end());
    if (pWaypoints->empty())
    {
#ifdef SHOW_PRUNE_DEBUG
std::cout << "Prune: ALL waypoints were pruned!\n";
#endif
    }
  }
}

void MakeCameraPath(FunctionCtrlPoints* pFunc, Level* pLevel)
{
  // For golf, go from hole to tee.
  // Start off by circling the hole.
  // Then travel to the Tee, then circle the tee.

  // For version 2.0, each room has waypoints.
  // The waypoints must be in order from the hole (lowest ID)  to the tee (highest ID).
  // Start at the hole.
  // The next point is either the next waypoint or the player, whichever is closer.
  // Repeat until no more waypoints, at which point the player position must be used.
  // NB With no waypoints, this behaviour should default to the v.1.0 behaviour.
  // Extra waypoints can be added to steer the camera path better.

  // Radius of circle around hole and tee
  const float RADIUS = 8.0f; // TODO CONFIG

  Assert(GetPoolHole(pLevel));
  Assert(GetPoolHole(pLevel)->GetOrientation());
  Orientation oStart = *(GetPoolHole(pLevel)->GetOrientation());
  oStart.SetY(oStart.GetY() + 2.0f);
  Orientation oEnd = *(GetBall()->GetOrientation());
  oEnd.SetY(oEnd.GetY() + 2.0f);

#ifdef _DEBUG
  Orientation oTee = *(GetTeeBox(pLevel)->GetOrientation());
  std::cout << "Tee: " << oTee.GetX() << "/" << oTee.GetY() << "/" << oTee.GetZ() << "\n";
  std::cout << "End: " << oEnd.GetX() << "/" << oEnd.GetY() << "/" << oEnd.GetZ() << "\n";
#endif


  static const float TIME_MULT = 
    Engine::Instance()->GetConfigFloat("golf_flyby_time_mult");
  int i = 0;
  float t = 0;

  // All waypoints which we should visit on the way from the hole to the ball.
  std::vector<Waypoint*> waypoints;

  Orientation prevOr = oStart;
  Orientation o;
  float prevAngle = 0;

  // Add each waypoint as a control point on the camera path.
  // Get the game objects which are in the same room as the player.
  int levelId = pLevel->GetId();
  int roomId = pLevel->GetRoomId();
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);
  // Iterate through map of Game Objects.
  bool firstwp = true;
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    GameObjectId id = it->first;
    PGameObject pGo = it->second;

    Waypoint* pW = dynamic_cast<Waypoint*>(pGo.GetPtr());
    if (pW)
    {
      waypoints.push_back(pW);
    }
  }
 
  // We have a list of all waypoints. If the ball is not on the Tee
  // then we may need to prune some waypoints, or the path will
  // stupidly go to the Tee then back to the ball.
  PruneWaypoints(&waypoints, pLevel);

  int numwps = waypoints.size();
  for (int j = 0; j < numwps; j++)
  {
    Waypoint* pW = waypoints[j];

    o = *(pW->GetOrientation());
    VertexBase vo = o.GetVertex();

    // Get distance between control points, so we can make time
    // proportional to this distance. Unfortunately it isn't quite 
    // right because the path isn't straight.
    // TODO Is there a way to get path length ?

    VertexBase vdiff = prevOr.GetVertex();
    vdiff -= vo;
    float dist = vdiff.Length();

    if (firstwp)
    {
      firstwp = false;
      // Now we have a second point, add the first one (for the golf 
      // hole). We had to wait until now so we knew which direction to
      // face.
      // The direction to face is from the second point to the first.
      VertexBase vs = oStart.GetVertex();
      float yRad = AngleCompare::TurnToFace(vs.x, vs.z, vo.x, vo.z);
      float yDeg = RadToDeg(yRad);
      NormalizeAngle(&yDeg, yDeg);
      prevAngle = yDeg;
      oStart.SetYRot(yDeg);

//std::cout << "PATH: Start angle:" << yDeg << "\n";

      // Add the hole as the first point. 
      pFunc->AddPoint(t, oStart);
      // Add this point several times to start the path correctly.
      for (i = 0; i < 4; i++)
      {
        t += 0.1f;
        pFunc->AddPoint(t, oStart);
      }
    }

    // TODO set y-rotation
    // Set y-rot to face the last point.
    VertexBase vp = prevOr.GetVertex();
    float yRad = AngleCompare::TurnToFace(vp.x, vp.z, vo.x, vo.z);
    float yDeg = RadToDeg(yRad);
    NormalizeAngle(&yDeg, prevAngle);
    prevAngle = yDeg;
    o.SetYRot(yDeg);

//std::cout << "PATH: Next angle:" << yDeg << "\n";

    t += dist * TIME_MULT; 
    pFunc->AddPoint(t, o);

    // Update the "previous" position, so we always get the distance
    // between the last two points.
    prevOr = o;
  }

  if (firstwp)
  {
    // No waypoints were found.
    // We must add the control point for the golf hole, using the 
    // Tee to get the y-rot value.
    VertexBase vp = prevOr.GetVertex();
    VertexBase vo = oEnd.GetVertex();
    float yRad = AngleCompare::TurnToFace(vp.x, vp.z, vo.x, vo.z);
    float yDeg = RadToDeg(yRad);
    NormalizeAngle(&yDeg, yDeg);
    prevAngle = yDeg;
    oStart.SetYRot(yDeg);

//std::cout << "PATH: Start angle (no waypoints):" << yDeg << "\n";
   
    // Add the hole as the first point. 
    pFunc->AddPoint(t, oStart);
    // Add this point several times to start the path correctly.
    for (i = 0; i < 4; i++)
    {
      t += 0.1f;
      pFunc->AddPoint(t, oStart);
    }
 
    prevOr = oStart;
  }

  // Direction for final point: facing the previous control point.
  VertexBase vp = prevOr.GetVertex();
  VertexBase vo = oEnd.GetVertex();
  float yRad = AngleCompare::TurnToFace(vp.x, vp.z, vo.x, vo.z);
  float yDeg = RadToDeg(yRad);
  NormalizeAngle(&yDeg, prevAngle);
  oEnd.SetYRot(yDeg);

//std::cout << "PATH: Final angle:" << yDeg << "\n";

  // Set t proportional to distance
  VertexBase vdiff = prevOr.GetVertex();
  vdiff -= oEnd.GetVertex();
  float dist = vdiff.Length();
  t += dist * TIME_MULT;

  pFunc->AddPoint(t, oEnd);

  // Keep moving back from final point - that is, away from the penultimate
  // point.
  vdiff.Normalize();
  float scale = 2.0f; // TODO CONFIG final backtracking speed
  vdiff *= scale;

//  for (i = 0; i < 4; i++)
//  {
    t += 2.0f;
    // Add vDiff to the final position.
    VertexBase v = oEnd.GetVertex();
    v -= vdiff;
    oEnd.SetVertex(v);
    pFunc->AddPoint(t, oEnd);
//  }

  // Add more points to end the path correctly.
  for (i = 0; i < 4; i++)
  {
    t += 0.1f;
    pFunc->AddPoint(t, oEnd);
  }
}

}




