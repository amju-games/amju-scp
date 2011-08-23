/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: GuiToolbox.h,v $
Revision 1.1  2004/09/08 15:43:08  jay
Added to repository
  
*/

#if !defined(AMJU_GUITOOLBOX_H_INCLUDED)
#define AMJU_GUITOOLBOX_H_INCLUDED

#include <string>
#include <vector>
#include "GuiComposite.h"
#include "GuiButton.h"

namespace Amju
{
class File;

// GuiToolbox
// A rectangular palette of buttons.
// Draw as an overlay in each EngineState subclass which needs a GuiToolbox.
class GuiToolbox : public GuiComposite
{
public:
  GuiToolbox();
  virtual ~GuiToolbox();

  static void Init();

  virtual void DrawImpl();
  virtual bool Load(File* );

  // Set the size of the GuiToolbox in buttons.
  void SetSize(int x, int y);

protected:
  // Side length of each button. They are square.
  static float s_buttonSize;

  // Size, as number of buttons in x and y.
  int m_numX, m_numY;
};
}

#endif

