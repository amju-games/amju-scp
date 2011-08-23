/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: AiRotateBehaviour.h,v $
Revision 1.1  2004/09/08 15:43:01  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_AI_ROTATE_BEHAVIOUR_H_INCLUDED)
#define SCHMICKEN_AI_ROTATE_BEHAVIOUR_H_INCLUDED

#include "Behaviour.h"

namespace Amju
{
class AiRotateBehaviour : public Behaviour
{
public:
  AiRotateBehaviour();

  virtual void OnActivated();
  virtual void Update();

protected:
  bool m_droppedPiece;
};
}

#endif
