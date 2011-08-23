/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: TextMenu.h,v $
Revision 1.1  2004/09/08 15:42:58  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_TEXT_MENU_H_INCLUDED)
#define SCHMICKEN_TEXT_MENU_H_INCLUDED

#include "Menu.h"

namespace Amju
{
class TextMenu : public Menu
{
public:
  virtual bool Load(File*);
};
}

#endif

