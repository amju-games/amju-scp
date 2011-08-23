/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateTitle.h,v $
Revision 1.1.4.1  2004/11/09 16:01:31  jay
Merged full screen and exit features from XING 2.1 to this branch.

Revision 1.1.6.1  2004/11/08 10:16:56  Administrator
Added exit button to title screen.

Revision 1.1  2004/09/08 15:42:38  jay
Added to repository
  
*/

#if !defined(ENGINE_STATE_TITLE_H_INCLUDED)
#define ENGINE_STATE_TITLE_H_INCLUDED

#include "EngineStateText.h"
#include "ExplosionEffect.h"
#include "CharacterGameObject.h"
#include "TextLeaf.h"
#include "GuiElement.h"
#include "Mutex.h"

namespace Amju
{
class EngineStateTitle : public EngineStateText
{
public:
  // ID of this class, which we register with the Engine
  static const char* Name;

  EngineStateTitle();
  virtual bool Load();
  virtual void Red(bool);
  virtual void TimerExpired();
  virtual void Draw();
  virtual void DrawOverlays();
  virtual void SetActive(bool active);
  virtual void MouseButton(bool down, bool ctrl, bool shift);
  virtual void MousePos(int x, int y);

  // Static so it's easier to access from other thread.
  static void SetLatestVersion(float );
  static float GetLatestVersion();

protected:
  bool LoadExplosion();
  bool LoadCharacter(const std::string& charname);

protected:
  enum { NUM_EXPLOSIONS = 5 };

  ExplosionEffect m_explosion[NUM_EXPLOSIONS];

  float explosionTimer;
  int m_currentExplosion;

  PSolidComponent m_pGreetText;

  // Characters which appear under the title
  std::vector<SharedPtr<CharacterGameObject> > m_characters;

  TextLeaf m_hitSpaceText;

  // GUI
  // ===
  // GUI Button - click to start
  PGuiElement m_pStartButton;
  // GUI Button - click to register if not registered.
  PGuiElement m_pRegButton;
  // Buy Now button - go to store web page
  PGuiElement m_pBuyButton;
  // Exit button - like alt-f4/option-Q
  PGuiElement m_pExitButton;

  // Text to go next to the buttons
  PSolidComponent m_pStartText;
  PSolidComponent m_pRegText;
  PSolidComponent m_pBuyNowText;
  PSolidComponent m_pExitText;

  // Latest version info is got by another thread, so we must lock
  // the version variable.
  static float s_latestVersion;
  static Mutex s_mutex;
};
}

#endif

