/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Score.h,v $
Revision 1.1  2004/09/08 15:42:41  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_SCORE_H_INCLUDED)
#define SCHMICKEN_SCORE_H_INCLUDED

#include "Number3d.h"
#include "SolidComponent.h"

namespace Amju
{
class Score
{
public:
  Score();
  ~Score();

  void Set(int);
  int Get() const;

  // Set score to zero.
  void Reset();

  void Init(); 
  void Draw();

protected:
  // TODO these should be 2D!
  Number3d m_num; // score number

  Number3d m_numHi;

  int m_score; 
  int m_hiScore;

  // "hi" text
  PSolidComponent m_pHiText;

  float m_left;
  float m_top;
  float m_z;
  float m_scale;
};
}

#endif

