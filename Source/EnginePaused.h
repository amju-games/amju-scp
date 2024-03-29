/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EnginePaused.h,v $
Revision 1.1.4.1.2.2  2005/08/26 21:06:21  jay
Add Pool background

Revision 1.1.4.1.2.1  2005/06/29 20:17:28  jay
Changes for POOL look and feel

Revision 1.1.4.1  2004/11/09 16:01:30  jay
Merged full screen and exit features from XING 2.1 to this branch.

Revision 1.1.6.1  2004/11/08 10:16:34  Administrator
Added exit and confirm buttons

Revision 1.1  2004/09/08 15:42:36  jay
Added to repository
 
*/

#if !defined(ENGINE_PAUSED_H_INCLUDED)
#define ENGINE_PAUSED_H_INCLUDED

#include "EngineMenuState.h"
#include "SolidComponent.h"
#include "EngineStateText.h"
#include "GuiElement.h"
 
namespace Amju
{
// Engine state: game is being played but is paused.
class EnginePaused : public EngineStateText
{
public:
  static const std::string Name; // ID of this state.

  EnginePaused();

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
  float m_elapsedTime; // save the elapsed game time 

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

