/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Evaluator.h,v $
Revision 1.1  2004/09/08 15:43:01  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_EVALUATOR_H_INCLUDED)
#define SCHMICKEN_EVALUATOR_H_INCLUDED

#include <string>
#include "Vertex.h"
#include "SharedPtr.h"

namespace Amju
{
class Ai;
class File;
class VisibilityGraphWithObjects;
class Finder;
struct RankedMove;

// Evaluates a move, i.e. a piece moved to a new destination,
// w.r.t an AI's goal.
class Evaluator : public Shareable
{
public:
  Evaluator();
  virtual ~Evaluator();

  // Return a score for the move rm. 
  // High positive score means better. 
  // TODO Should the score be within a range, e.g. 0..1 ??
  // Currently you can give moves very high or low scores, to ensure they are
  // selected, or not.

  // NB Scoring function may return EvalPending if it needs more time to 
  // do its evaluating. In this case keep calling the function
  // until it has finished.
  // If EvalFail is returned the move can't be evaluated, don't call again.
  enum EvalResult { EvalOk, EvalFail, EvalPending };

  virtual EvalResult Score(
    const RankedMove& rm, 
    Ai* pAi, 
    VisibilityGraphWithObjects* pVgo,
    float* pResult) = 0;

  virtual bool Load(File*);

  // Call to reset cached/precalculated data.
  // This should be called by the Finder which owns this Evaluator just before
  // evaluating a _set_ of moves. It isn't called before evaluating _every_ move.
  virtual void Reset(Finder*);

  float GetWeight() const;
  void SetWeight(float w);

#if defined(_DEBUG)
public:
  const std::string& GetName() const;
  void SetName(const std::string& name);
protected:
  std::string m_name;
public:
#endif

protected:
  // Useful functions for Evaluators
  // -------------------------------
  static VertexBase GetGoalPos(Ai* pAi);

protected:
  float m_weight;
};
}

#endif
