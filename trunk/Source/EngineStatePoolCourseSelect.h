/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStatePoolCourseSelect.h,v $
Revision 1.1.2.1  2006/08/14 17:50:13  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:26  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.8.3  2005/09/20 13:15:35  jay
Show different cost types

Revision 1.1.8.2  2005/08/26 21:24:24  jay
Changed base class so we can use Pool Background

Revision 1.1.8.1  2005/06/04 22:29:54  jay
Minor cosmetic changes to game select screen

Revision 1.1  2004/09/08 15:43:04  jay
Added to repository
  
*/

#if !defined(ENGINE_STATE_GOLF_COURSE_SELECT)
#define ENGINE_STATE_GOLF_COURSE_SELECT

#include "EngineStateText.h"
#include "GuiElement.h"
#include "TexturedQuad.h"

namespace Amju
{
class EngineStatePoolCourseSelect : public EngineStateText
{
public:
  static const char* Name;

  virtual bool Load();
  virtual void Draw();
  virtual void DrawOverlays();
  virtual void Update();
  virtual void SetActive(bool);
  virtual void Red(bool down);
  virtual void MouseButton(bool, bool, bool);

  static void SetCourse(int c) { s_currentCourse = c; } 
  static int GetCourse() { return s_currentCourse; }

  static void SetCanClick(bool b) { s_canClick = b; }

protected:
  void GetThumbnails();

protected:
  PSolidComponent m_pGreetText;

  PGuiElement m_pSelectButton;
  PGuiElement m_pNextButton;
  PGuiElement m_pBackButton;
  PSolidComponent m_pText; // "game select" text
  PSolidComponent m_pNextText;
  PSolidComponent m_pBackText;

  // Thumbnails for current course
  // Double-clickable
  PGuiElement m_thumbnails[18];

  static int s_currentCourse;

  // Once a course or hole has been chosen, you can't click on anything else.
  static bool s_canClick;

  // TQs to show the cost of each room.
  // NB Each room has a cost of only one type.
  std::vector<TexturedQuad> m_costTqs;
};
}

#endif

