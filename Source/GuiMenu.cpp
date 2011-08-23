/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: GuiMenu.cpp,v $
Revision 1.1.10.1  2007/03/09 21:48:55  jay
Copy latest GUI library from trunk

Revision 1.6  2007/01/13 13:26:41  jay
Comment

Revision 1.5  2006/12/13 18:54:35  jay
Use new localisation scheme

Revision 1.4  2006/12/11 22:07:05  jay
Creator (for each menu item) sets text and font of the item

Revision 1.3  2006/12/11 09:43:24  jay
Factor out ButtonCreator

Revision 1.2  2006/12/03 21:24:51  jay
Changes to make GuiMenu really useable

Revision 1.1  2004/09/08 15:43:07  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "GuiMenu.h"
#include "File.h"
#include "Engine.h"
#include "GuiTextButton.h"
#include "TextWriter.h"

namespace Amju
{
GuiMenu::GuiMenu()
{
  m_pFont = 0;
}

void GuiMenu::SetFont(Font* pFont)
{
  m_pFont = pFont;
}

bool GuiMenu::Load(File* pf)
{
  // NB This menu file format should be, and is, compatible with the
  // othe menu file formats used in this code, i.e. Core/Menu

  // Get the number of menu items.
  int numItems = 0;
  if (!pf->GetInteger(&numItems))
  {
    pf->ReportError("Expected number of menu items.");
    return false;
  }

  // Get each item. 
  float widest = 0;
  int i;
  for (i = 0; i < numItems; i++)
  {
    // Make a Text button for each menu item.
    std::string buttonText;
    if (!pf->GetDataLine(&buttonText))  // TODO Localisation
    {
      pf->ReportError("Failed to load menu text.");
      return false;
    }
    GuiElement* pButton = CreateGuiTextButton(m_pFont, buttonText);

    float w = pButton->GetWidth();
    if (w > widest)
    {
      widest = w;
    }

/*
    float thisW = Engine::Instance()->GetTextWriter()->GetTextWidth(buttonText);
    if (thisW > w)
    {
      w = thisW;
    }
*/
    AddChild(pButton);
  }

  // Set the position of each menu item relative to this parent.
  float y = 0;
  for (i = 0; i < numItems; i++)
  {
    GetChild(i)->SetRelPos(y, 0); 
    y += GetChild(i)->GetHeight();
    // TODO configurable vertical spacing between elements
  }

  // Set the size of the menu to the widest button and the total height
  SetSize(widest, y);

  return true;
}

GuiElement* GuiMenu::CreateGuiTextButton(Font* pFont, const std::string& text)
{
  if (!m_pButtonCreator.GetPtr())
  {
#ifdef _DEBUG
std::cout << "** ERROR ** You must set the button creator of the GuiMenu.\n";
#endif
    Assert(0);
    return 0;
  }
  m_pButtonCreator->SetText(text);
  m_pButtonCreator->SetFont(pFont);
  
  return m_pButtonCreator->Create();
}

void GuiMenu::SetButtonCreator(ButtonCreator* p)
{
  m_pButtonCreator = p;
}
}

