/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ChickShedBehaviour.h,v $
Revision 1.1  2004/09/08 15:42:34  jay
Added to repository
  
*/

#if !defined(CHICK_SHED_BEHAVIOUR_H_INCLUDED)
#define CHICK_SHED_BEHAVIOUR_H_INCLUDED

#include "SeekBehaviourBase.h"
#include "GameObject.h"

namespace Amju
{
// This behaviour kicks in when a Chick can see a path to a shed (home).
class ChickShedBehaviour : public SeekBehaviourBase
{
public:
  // Get constants from config file.
  static void Init();

  static void SetVisibilityGraph(PVisibilityGraph visgraph);

  virtual void OnActivated();

protected:
  // Get visibility graph: all chicks use the same one.
  // It uses vis.points from the scene and all pieces.
  virtual PVisibilityGraph GetVisibilityGraph();

  virtual void ReachedTarget();

protected:
  // The vis. graph, used by all chicks.
  // TODO pointer for efficiency
  static PVisibilityGraph s_visgraph;
};
}
#endif
