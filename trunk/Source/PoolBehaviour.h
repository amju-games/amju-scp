/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: PoolBehaviour.h,v $
Revision 1.1.2.2  2006/08/14 17:50:21  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:37  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.11  2005/09/11 19:02:50  jay
Add CalcPlaceCueBall() - work out where AI should place cue ball after a foul.

Revision 1.1.2.10  2005/08/02 18:30:14  jay
Remove m_pRules. This breaks Undo and isn't even necessary.

Revision 1.1.2.9  2005/07/30 12:25:22  jay
Added Rail::LineIntersects, was to be used by trajectory but is now spare.

Revision 1.1.2.8  2005/06/29 20:23:53  jay
Store all shots, then sort and pick best one. Allows us to show all
shots considered, for debugging.

Revision 1.1.2.7  2005/06/22 21:43:15  jay
Added US 8 Ball

Revision 1.1.2.6  2005/06/19 21:35:26  jay
Construct with Rules object

Revision 1.1.2.5  2005/06/13 22:14:35  jay
Bounce off rails; draw trajectories for debugging

Revision 1.1.2.4  2005/06/04 22:37:19  jay
Recognise when unable to pot a ball and when totally snookered -
not finished. Also need to consider rail shots to hit a valid ball.

Revision 1.1.2.3  2005/05/08 09:55:48  jay
Ongoing improvements to AI

Revision 1.1.2.2  2005/04/11 21:23:52  jay
Ongoing fixes

Revision 1.1.2.1  2005/03/31 22:31:44  jay
Pool AI: initial revision is totally stupid.

*/

#ifndef AMJU_POOL_BEHAVIOUR_H_INCLUDED
#define AMJU_POOL_BEHAVIOUR_H_INCLUDED

#include <vector>
#include "Behaviour.h"
#include "PoolHole.h"
#include "PoolBall.h"
#include "Geom2d.h"
#include "Target.h"

namespace Amju
{
class Rules;

// Base class for AI for playing pool.
// Subclasses will need to override certain functions depending on the
// Rules being played. 
// NB Rule and Behaviour subclass are closely coupled - so the Rules object
// creates the corresponding Behaviour object.

class PoolBehaviour : public Behaviour
{
public:
  PoolBehaviour();

  virtual void Update();
  virtual void OnActivated();
  // Draw stuff for debugging
  virtual void Draw();

  // Work out the shot when OnActivated is called. 
  void CalcShot();

  // Get the Object Ball.
  PoolGameObject* GetTargetObject();

  // Get the target we want to aim at. This is usually the "ghost ball"
  // touching the object ball, but could be the position of a ghost
  // ball on a rail, for a shot which bounces off a rail.
  PoolGameObject* GetAimTarget();

  // Get ball to display in HUD as the target ball.
  // May return zero if any ball may be hit.
  virtual TexturedQuad* GetHudBall();

  // Get list of balls which are legal to hit with the cue ball.
  // This will depend on the Rules variant. E.g. for UK pool, any ball of the
  // correct colour is valid. For n-ball, the lowest numbered ball is the
  // only valid ball.
  typedef std::vector<PoolBall*> ValidBalls;
  virtual ValidBalls GetValidBalls() = 0;

  // Get position to place cue ball - use this for AI characters after a foul.
  VertexBase GetPlaceCueBallPos() const;

// NB Rails are public so Trajectory can use them. This lets us show
// the player the direction of a ball once it hits the rail.
public:
  // One Rail on the table, i.e. the cushion between pockets.
  // NB The long sides of the table have TWO rails.
  class Rail
  {
  public:
    Rail(const Geom2d::Line2d& lineseg, int id);

    // Find the ghost position on this rail which the cue ball should aim
    // for to hit the object ball.
    // Returns true if there is a ghost pos on this rail, false if there
    // is no way to hit the object ball by bouncing off this rail.
    bool Bounce(
      const Geom2d::Point2d& cue, 
      const Geom2d::Point2d& obj, 
      Geom2d::Point2d* pGhost,
      Geom2d::Point2d* pPerpPt1,
      Geom2d::Point2d* pPerpPt2);

    bool LineIntersects(
      const Geom2d::Line2d& line,
      Geom2d::Point2d* pIntersectPoint) const;

    Geom2d::Line2d GetLineseg() const;

#ifdef _DEBUG
  void Draw();
#endif 

  protected:
    // Try to find line perpendicular to rail going through pt.
    // If there is such a line, return true, and give the point on the line
    // where the perpendicular crosses it; also gives length of
    // perpendicular from pt to the point on the line.
    bool GetPerpendicular(
      const Geom2d::Point2d& pt, 
      Geom2d::Point2d* pPointOnLine, 
      float* pLineLen);

  protected:
    Geom2d::Line2d m_lineseg;
    int m_id; // for debug output more than anything else
  };

  typedef std::vector<Rail> Rails;

  const Rails& GetRails();

protected:
  // Get the current Rules. The Rules subtype should match the
  // behaviour subtype.
  static Rules* GetRules();

  // Create vector of Rail objects for the current table.
  void CreateRails();

  typedef std::vector<PoolHole*> Pockets;

  // Find pockets for the current room. 
  void FindPockets();

  // LOS between two objects ?
  static bool IsLos(PoolGameObject* p1, PoolGameObject* p2);

  // LOS between two points, using capsule of given radius ?
  // The two given GameObjects are ignored. This is so if you want to find
  // if there is LOS between two Game Objects, the objects themselves
  // don't appear to cause an obstruction.
  static bool IsLos(
    const VertexBase&, 
    const VertexBase&, 
    float radius,
    PoolGameObject* p1, 
    PoolGameObject* p2,
    bool showDebugOut = false);

  // Get pockets which the given ball has a clear line of sight to.
  Pockets GetLosPockets(PoolBall*);

  // Find a shot which should pot the given object ball in the given
  // pocket. Returns true if the shot exists, and returns the shot
  // rank, ghost ball pos, cue ball vel.
  bool CalcPotShot(
    const VertexBase& vCue, 
    const VertexBase& vObj, 
    const VertexBase& vPocket,
    float radius,
    float* pRank,
    BoundingSphere* pGhostBs,
    float* pHVel,
    float* pCutAngle);

  // Nothing is pottable - just try to hit a valid ball.
  // Return true if this is possible, false if snookered.
  bool CalcAnyValidShot(
    const VertexBase& vCue, 
    float radius,
    float* pRank, 
    BoundingSphere* pGhostBs);

  // Try to find a way to hit a valid ball by bouncing off a rail
  // (and possibly two rails).
  // Call this if nothing is pottable and there is no direct 
  // LOS to any valid ball.
  bool CalcValidBounceShot(
    const VertexBase& vCue, 
    float radius,
    float* pRank, 
    BoundingSphere* pGhostBs);

  // If we are snookered, we will try a jump shot.
  // This function sets up the shot params to hit the closest valid ball
  // with a jump shot. The ghost ball specified is simply set to the
  // object ball we are aiming at.
  // Sets cue ball horizontal vel, like the other shot calc functions,
  // but also sets vertical vel as this is a jump shot.
  void CalcJumpShot(BoundingSphere* pGhostBs, float* pHVel, float* pVVel);

  // Find a shot which bounces off one rail and pots the given
  // object ball in the given pocket. Returns true if such a
  // shot is found, with rank.
  bool CalcBouncePotShot(
    const VertexBase& vCue, 
    const VertexBase& vObj, 
    const VertexBase& vPocket,
    float radius,
    float* pRank, 
    BoundingSphere* pGhostBs,
    float* pHVel);

  // Find a sensible position to place the Cue ball - assuming there has
  // just been a foul, and we can place it anywhere.
  // On success, sets m_placeCueBallPos and returns true.
  // If we return false there is no good place - just use the Tee position.
  bool CalcPlaceCueBallPos();

protected:
  float m_time;
  float m_hVel;
  float m_vVel;
  float m_yRot;

  // The object ball we are aiming to hit with the cue ball.
  PPoolGameObject m_pTargetObject;

  // Ghost ball, touching object ball. 
  BoundingSphere m_ghostBall;
 
  // Point on a rail which we aim the ball at for a bounce shot.
  // Undefined value if flag below is false.
  Geom2d::Point2d m_railPos;
  // This flag set to true if shot bounces off rail.
  bool m_isBounceShot;

  // Pocket we are aiming at. May be zero if the shot does not
  // attempt to pot anything.
  PoolHole* m_pPocket;

  // Pockets for the current table.
  Pockets m_pockets;

  // Rails for the current table.
  Rails m_rails;

  // This is the object the player aims at. We can't just aim at the
  // object ball because the shot may bounce off a rail first.
  Target m_target;

  // Store the endpoints of the perpendiculars from the cue ball and 
  // object ball to the rails, for bounce shots.
  // This is used for testing.
  Geom2d::Point2d m_perpPt1;
  Geom2d::Point2d m_perpPt2;

  // Ball to show in HUD as the target. May be zero if there are multiple
  // targets.
  PoolBall* m_pHudBall;

  // Store info for ALL valid shots, for AI debugging.
  struct ValidShot
  {
    float m_rank;
    PoolGameObject* m_pTargetObject;
    PoolHole* m_pPocket; // may be zero
    Geom2d::Point2d m_railPos; // rail pos for bounce/rail shots
    BoundingSphere m_ghostBall; 

    enum ShotType { BREAK, POT, RAIL_POT, HIT, RAIL_HIT, JUMP };
    ShotType m_shotType;

    // All pockets visible from object ball   
    Pockets m_pockets;
    // Cut angle - shallow cut is bad.
    float m_cutAngle;
    // Total distance balls must travel (cue + possible object ball)
    float m_dist;

    float m_hVel;
    float m_vVel;

    ValidShot(ShotType, PoolGameObject*, PoolHole*, Geom2d::Point2d*, BoundingSphere*, float rank);
    void Draw();
    std::string ToString();
    // For sorting, to get the best shot
    bool operator<(const ValidShot&) const;
  };

  typedef std::vector<ValidShot> ValidShots;
  ValidShots m_validShots;

  // If we can place the cue ball anywhere (i.e., after a foul),
  // this is the new cue ball position.
  VertexBase m_placeCueBallPos;
  
public:
  // Public interface for drawing all valid shots - just for 
  // AI debugging.
  int GetNumValidShots() const;
  void DrawValidShot(int);
  std::string ValidShotString(int);
};

class PoolBehaviourUk : public PoolBehaviour
{
public:
  PoolBehaviourUk();
  virtual ValidBalls GetValidBalls();
};

class PoolBehaviourNBall : public PoolBehaviour
{
public:
  PoolBehaviourNBall(int numBalls);
  virtual ValidBalls GetValidBalls();

protected:
  int m_numBalls;
};

class PoolBehaviourUs8Ball : public PoolBehaviour
{
public:
  PoolBehaviourUs8Ball();
 
  // Show spots or stripes ball
  virtual TexturedQuad* GetHudBall();

  virtual ValidBalls GetValidBalls();

protected:
  TexturedQuad m_spots, m_stripes;
};
}

#endif

