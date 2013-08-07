/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PoolGuiFactory.h,v $
Revision 1.1  2004/09/08 15:43:07  jay
Added to repository
  
*/

#if !defined(AMJU_GUI_FACTORY_H_INCLUDED)
#define AMJU_GUI_FACTORY_H_INCLUDED

#include "Singleton.h"
#include "Factory.h"
#include "GuiElement.h"

namespace Amju
{
class File;

class SinglePoolGuiFactory : public Factory<PoolGuiElement>
{
public:
  PoolGuiElement* Create(File*);
};
typedef Singleton<SinglePoolGuiFactory> PoolGuiFactory;
}

#endif

