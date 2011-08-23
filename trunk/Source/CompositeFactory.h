/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: CompositeFactory.h,v $
Revision 1.1  2004/09/08 15:42:47  jay
Added to repository
  
*/

#if !defined(COMPOSITE_FACTORY_H_INCLUDED)
#define COMPOSITE_FACTORY_H_INCLUDED

#include "SolidComponent.h"
#include <string>

namespace Amju
{
// Should use Factory<SolidComponent>, but registering Createable
// classes doesn't work.
class CompositeFactory 
{
  // The only thing that can create SolidComponents is 
  // SolidComponent::Create().
  friend class SolidComponent;

private:
  static CompositeFactory* Instance();
  PSolidComponent Create(const std::string& filename);

private:
  CompositeFactory(); // Singleton
};
}
#endif

