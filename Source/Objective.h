/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Objective.h,v $
Revision 1.1  2004/09/08 15:42:53  jay
Added to repository
  
*/

#if !defined(OBJECTIVE_H_INCLUDED)
#define OBJECTIVE_H_INCLUDED

#include "GameObject.h"
#include "States.h"
#include <utility> // pair
#include <vector>

namespace Amju
{
class File;

typedef std::pair<GameObjectId, State> ObjectivePair;
  
class Objective : public std::vector<ObjectivePair>
{
public:
  ~Objective() {}

  // Load an objective from a file.
  bool Load(File* pf);

  // Returns true when the Objective has been met.
  bool IsSatisfied() const;

protected:
  enum Operator
  {
    AND,
    OR
  };

  Operator m_op;

public:
  bool Save(File*);

#if defined(SCENE_EDITOR)
  Operator GetOp() const { return m_op; }
  void SetOp(Operator op) { m_op = op; }

#endif

};
}

#endif

