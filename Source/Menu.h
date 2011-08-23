/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Menu.h,v $
Revision 1.1  2004/09/08 15:42:52  jay
Added to repository
  
*/

#if !defined(MENU_H_INCLUDED)
#define MENU_H_INCLUDED

#include <string>
#include <vector>
#include "SolidComponent.h"
#include "GuiElement.h"

namespace Amju
{
class File;

// Menu: a list of items is displayed; one is selected.
class Menu : public Shareable
{
public:
  Menu();

  virtual ~Menu();
 
  // Call to choose the currently selected item.
  int Choose() { return m_selected; }
  bool IsChoiceValid() const;

  // Select the previous item.
  virtual void Previous();

  // Select the next item.
  virtual void Next();
  
  // Draw the menu. 
  virtual void Draw();

  // Draw overlays, e.g. GUI elements
  virtual void DrawOverlays();

  // Load menu from a file.
  virtual bool Load(File*);
  bool Load(const std::string& filename);

  int Size() const;

  virtual void Clear() {}

  SolidComponent* GetItem(int index);

  virtual void MousePos(int x, int y);
  virtual void MouseButton(bool down, bool ctrl, bool shift);

  // Set selected item. This is for setting the default choice.
  void SetSelected(int selected);

  // Pick mode - i,.e, select with mouse
  void SetPickMode(bool);
  bool IsPickMode() const;

  void SetVertical(bool vertical);
  bool IsVertical() const;

  void SetMenuTop(float f);
  
  bool IsEnabled(int i);
  void SetEnabled(int i, bool b);
  void EnableAll(bool b = true);

  virtual void AddItem(SolidComponent*);
  virtual void DeleteItem(int);

  float GetTop() const;
  void SetTop(float);

protected:
  virtual void DrawItem(int i);

  // Ensure the currently selected item is visible.
  virtual void EnsureVisible();

  virtual bool LoadButtons(File*);

  // Use mouse coord to decide if a new item has been selected.
  void MousePick();

protected:
  typedef std::vector<PSolidComponent> ItemList;
  ItemList m_items;

protected:
  // Index of selected item.
  int m_selected;

  // Index of topmost item visible
  int m_topItem;

  // x- or y-coord (depending on horizontal or vertical)
  // of top of menu. This should be changed so the active item is
  // always visible.
  float m_menuTop;

  // If true, menu is drawn for mouse picking.
  bool m_isPickMode;

  bool m_isVertical;

  // Enabled flag for each item.
  // NB Currently it's the responsibility of the subclass to make sure that
  // the size is always the same as the number of items in the menu.
  std::vector<bool> m_enabled;

  // True if mouse is being dragged.
  bool m_drag;

  SharedPtr<GuiElement> m_pPrevButton;
  SharedPtr<GuiElement> m_pNextButton;

};

typedef SharedPtr<Menu> PMenu;
}
#endif

