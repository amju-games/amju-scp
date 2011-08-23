/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Trajectory.h,v $
Revision 1.1.2.2  2006/08/14 17:50:26  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:41  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.8.2  2005/08/03 21:04:57  Administrator
MSVC compile bug

Revision 1.1.8.1  2005/07/30 12:28:04  jay
Overhauled trajectory to show directions of multiple balls.

Revision 1.1  2004/09/08 15:43:07  jay
Added to repository
  
*/

#if !defined(TRAJECTORY_H_INCLUDED)
#define TRAJECTORY_H_INCLUDED

#include <map>
#include <utility>
#include <vector>
#include "SharedPtr.h"
#include "Vertex.h"
#include "BillBoard.h"
#include "Colour.h"

namespace Amju
{
class Trajectory : public Shareable
{
public:
  Trajectory();

  static bool Init();

  void Draw();
  void SetDeltaTime(float);
  void Recalc(const VertexBase& v0, float yRot, float vertVel, float horVel);
  void Clear();
  void SetMaxPoints(int m) { m_maxPoints = m; }
  int GetMaxPoints() const { return m_maxPoints; }
  void Update();
  void SetBallPos(const VertexBase& );

protected:
  void GetLineSegsForBall(
    int ballId, 
    VertexBase v1,
    float yRot);

  bool FindClosestBallCollision(
    int ballId, // ID of the current ball
    const VertexBase& v1, 
    const VertexBase& v2,
    int* pBallId, // ID of the new ball we just collided with
    VertexBase* pVGhost);

  bool FindWallCollision(
    int ballId,
    VertexBase v1,
    float yRot,
    VertexBase* pEndPoint, 
    float* pReflectionDir);

protected:
  float m_deltaTime;

//  typedef std::vector<VertexBase> Verts;
//  Verts m_vertices;

  static SharedPtr<SolidComponent> s_pBillBoard;

  int m_maxPoints;

  VertexBase m_v0;
  float m_yRot;
  float m_vertVel;
  float m_horVel;

  // POOL: Trajectory for one ball consists of a series of line segments.
  struct BallTr
  {
    void Draw();

    typedef std::vector<std::pair<VertexBase, VertexBase> > Segs;
    Segs m_linesegs;
    Colour m_colour;
    int m_ballId;
  };
  typedef std::map<int, BallTr> BallTrs;
  BallTrs m_trs;

#ifdef MSVC
  // Holy crap, for MSVC we need to give the nested class access!
  friend struct Trajectory::BallTr;
#endif
};
}

#endif

