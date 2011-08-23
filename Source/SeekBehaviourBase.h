/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: SeekBehaviourBase.h,v $
Revision 1.1  2004/09/08 15:42:56  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_SEEK_BEHAVIOUR_BASE_H_INCLUDED)
#define SCHMICKEN_SEEK_BEHAVIOUR_BASE_H_INCLUDED

#include <deque>
#include "AvoidBehaviourBase.h"
#include "Vertex.h"
#include "SolidComponent.h"
#include "VisibilityGraph.h"
#include "AStarPathFinder.h"

namespace Amju
{
// Base class for behaviours which seek a target. E.g.
//  - chick seeks shed (ChickShedBehaviour)
//  - croc seeks chick (ChaseBehaviour)
//  - croc seeks home (RunHomeBehaviour)
// This kind of behaviour follows a path of points to reach the target.
// In the most simple case, there's only one point, i.e. the target.
// Other possibilities:
// As the Character moves, avoid NPCs of same type, and avoid obstacles.
// (The path should already skirt fixed obstacles.)
class SeekBehaviourBase : public AvoidBehaviourBase
{
public:
  SeekBehaviourBase();

  virtual void Update();

  virtual void Draw();

  // Call to add a vertex along path. 
  // NB first call should be vertex nearest to Character; last call
  // should be final target.
  void AddPathPoint(const VertexBase& v);

  // Subclasses must override this to set up path, speed.
  // If the activation process will take too long for one frame, set
  // m_isStillActivating. Then continue the activation process each frame
  // by overriding ContinueActivating().
  virtual void OnActivated() = 0;

  // Use A* path finder to create a path from current Character position
  // to target.
  // Returns SearchOk if path exists from character to target.
  // (NB Target is a member of Behaviour base class.)
  // SearchFail is no path, or SearchPending if result not known yet.
  SearchResult CreatePath();

protected:
  // Get the scene used by the path finder.
  virtual PSolidComponent GetScene();

  // Get the visibility graph used by the path finder.
  virtual PVisibilityGraph GetVisibilityGraph();

  // Get path finder. Override this to return a subclassed pathfinder.
  virtual AStarPathFinder* GetPathFinder();

  // Called when final vertex in path is reached
  virtual void ReachedTarget() = 0;

  // Make Character head towards v at speed m_vel.
  // Return distance _squared_ to v in x-z plane.
  virtual float HeadTowards(const VertexBase& v);

  // This is called by Update() if the m_isStillActivating flag is true.
  virtual void ContinueActivating();

  void DrawPath();

protected:
  // Path to follow is a deque of points. 
  typedef std::deque<VertexBase> PathPoints;
  PathPoints m_path;

  // Speed at which to move along path.
  float m_vel;

  // If true, Update calls ContinueActivating() instead of doing its 
  // normal thing.
  // The idea is that if OnActivated() starts a task that will take some time,
  // we return before activation is finished.
  // We then continue the activation process in Update. Once this is complete,
  // this flag is reset.
  bool m_isStillActivating;
};
}
#endif
