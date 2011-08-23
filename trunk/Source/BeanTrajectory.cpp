/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: BeanTrajectory.cpp,v $
Revision 1.1.2.2  2006/08/14 17:50:07  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:24  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1  2004/09/08 15:43:04  jay
Added to repository
  
*/

#include "BeanTrajectory.h"
#include "Trajectory.h"
#include "EngineStatePoolBase.h"

namespace Amju
{
const char* BeanTrajectory::GetTypeName() const
{
  return "bean_trajectory";
}

void BeanTrajectory::DoBeanAction()
{
  int mp = EngineStatePoolBase::GetTrajectory()->GetMaxPoints();
  mp += 10;
  EngineStatePoolBase::GetTrajectory()->SetMaxPoints(mp); 

}


}

