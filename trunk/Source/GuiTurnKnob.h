/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: GuiTurnKnob.h,v $
Revision 1.1  2004/09/08 15:43:08  jay
Added to repository
  
*/

#if !defined(GUI_TURN_KNOB_H_INCLUDED)
#define GUI_TURN_KNOB_H_INCLUDED 

#include "GuiButton.h"

namespace Amju
{
class GuiTurnKnob : public GuiButton
{
public:
  GuiTurnKnob();

  virtual bool Load(File* );
  virtual void DrawImpl();
  virtual void MousePos(int x, int y);

  void SetAngleDegs(float degs);
  float GetAngleDegs();

protected:
  float m_angleDegs;


};
}

#endif


