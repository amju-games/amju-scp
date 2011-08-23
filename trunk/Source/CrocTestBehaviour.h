/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: CrocTestBehaviour.h,v $
Revision 1.1  2004/09/08 15:42:35  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_CROC_TEST_BEHAVIOUR_H_INCLUDED)
#define SCHMICKEN_CROC_TEST_BEHAVIOUR_H_INCLUDED

#include "Behaviour.h"

namespace Amju
{
// This behaviour is simple for testing. The Character runs round in circles.
class CrocTestBehaviour : public Behaviour
{
public:
  virtual void Update();
};
}

#endif

