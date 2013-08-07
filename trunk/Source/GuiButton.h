/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PoolGuiButton.h,v $
Revision 1.1  2004/09/08 15:43:07  jay
Added to repository
  
*/

#if !defined(AMJU_GUI_BUTTON_H_INCLUDED)
#define AMJU_GUI_BUTTON_H_INCLUDED

#include <vector>
#include "GuiElement.h"
#include "TexturedQuad.h"

namespace Amju
{
class PoolGuiButton : public PoolGuiElement
{
public:
  PoolGuiButton();
  
  virtual bool Load(File* );
  virtual void DrawImpl();

  void SetFromQuad(const TexturedQuad& tq);

protected:
  std::vector<TexturedQuad> m_quads;
};
}

#endif

