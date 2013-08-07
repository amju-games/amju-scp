/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: EngineStateOptions.h,v $
Revision 1.1.2.2  2006/08/14 17:50:11  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:31  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.1  2005/10/21 17:42:06  jay
Add new options menu, accessed from main menu

*/

#ifndef ENGINE_STATE_OPTIONS_H_INCLUDED
#define ENGINE_STATE_OPTIONS_H_INCLUDED 

#include "SolidComponent.h"
#include "EngineStateText.h"
#include "GuiElement.h"
 
namespace Amju
{
// Options menu accessed from the start menu - i.e. game is not yet 
// being played.
class EngineStateOptions : public EngineStateText
{
public:
  static const std::string Name; 

  EngineStateOptions();

  virtual void SetActive(bool active);
  virtual bool Load();
  virtual void Clear();
  virtual void Draw();
  virtual void DrawOverlays();

  virtual void Red(bool);
  virtual void MouseButton(bool, bool, bool);

  // Get key press for configuring keys.
  //virtual void OnKey(char key, bool down);

  static void SetShowConfirmButtons(bool b);

protected:
  PPoolGuiElement m_pButtonResume;
  PPoolGuiElement m_pButtonNextHole;
  PPoolGuiElement m_pButtonPrevHole;
  PPoolGuiElement m_pButtonNextCourse;
  PPoolGuiElement m_pButtonPrevCourse;
  PPoolGuiElement m_pButtonRestartHole;
  PPoolGuiElement m_pButtonRestartGame;
  PPoolGuiElement m_pButtonFullScreen;
  PPoolGuiElement m_pButtonSoundVol;
  PPoolGuiElement m_pButtonMusicVol;
  PPoolGuiElement m_pButtonQuit; // quit the current hole
  PPoolGuiElement m_pButtonQuitGame; // quit the game, i.e. exit()
  PPoolGuiElement m_pButtonQuitYes;
  PPoolGuiElement m_pButtonQuitNo;

  // Show sound/music vol
  TexturedQuad m_tqSound;
  TexturedQuad m_tqNoSound;

  static bool s_showConfirmButtons;
};
}
#endif

