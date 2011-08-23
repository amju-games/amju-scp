
#ifndef GUI_TEXT_EDITABLE_H_INCLUDED
#define GUI_TEXT_EDITABLE_H_INCLUDED

#include "GuiText.h"

namespace Amju
{
// This GUI element shows text and allows user to edit it.
class GuiTextEditable : public GuiText
{
public:
  virtual void DrawImpl();
  
  virtual void OnKey(char c);
  
  virtual void OnPlusUp(bool down);
  virtual void OnPlusDown(bool down);
  virtual void OnPlusLeft(bool down);
  virtual void OnPlusRight(bool down);

};
}

#endif

