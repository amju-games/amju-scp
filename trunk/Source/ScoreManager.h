/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ScoreManager.h,v $
Revision 1.1  2004/09/08 15:42:56  jay
Added to repository
  
*/

#if !defined(SCORE_MANAGER_H_INCLUDED)
#define SCORE_MANAGER_H_INCLUDED

namespace Amju
{
// Shows scores for items killed/collected etc.
// This avoids having to have numbers inside all objects which
// increase the score.
class SingleScoreManager
{
public:
  void CullDraw(Frustum*);
  void Update(); 
  void AddScore(int score, const VertexBase& v);

protected:
  std::deque<PSolidComponent> m_scores;
};

typedef Singleton<SingleScoreManager> ScoreManager;
}

#endif

