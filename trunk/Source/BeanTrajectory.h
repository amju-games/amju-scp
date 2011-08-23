/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: BeanTrajectory.h,v $
Revision 1.1.2.2  2006/08/14 17:50:07  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:24  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1  2004/09/08 15:43:04  jay
Added to repository
  
*/

#if !defined(BEAN_TRAJECTORY_H_INCLUDED)
#define BEAN_TRAJECTORY_H_INCLUDED 

#include "Bean.h"

namespace Amju
{
class BeanTrajectory : public Bean
{
public:
  virtual const char* GetTypeName() const;

protected:
  virtual void DoBeanAction();

};
}

#endif

