/*
Amju Games source code (c) Copyright Jason Colman 2000-2006
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "SendableButtonCreator.h"
#include "GuiSendable.h"

namespace Amju
{
GuiElement* SendableButtonCreator::Create()
{
  GuiElement* p = ButtonCreator::Create();
  GuiSendable* dec = new GuiSendable;
  dec->AddChild(p);
  return dec;
}
}


