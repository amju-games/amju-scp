/*
Amju Games source code (c) Copyright Jason Colman 2000-2006
*/

#ifndef SENDABLE_BUTTON_CREATOR_H_INCLUDED
#define SENDABLE_BUTTON_CREATOR_H_INCLUDED

#include "ButtonCreator.h"

namespace Amju
{
class SendableButtonCreator : public ButtonCreator
{
public:
  virtual GuiElement* Create();
};
}

#endif

