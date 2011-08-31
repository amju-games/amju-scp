/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: GuiToolbox.cpp,v $
Revision 1.1  2004/09/08 15:43:08  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "GuiToolbox.h"
#include "File.h"
#include "Engine.h"

namespace Amju
{
float GuiToolbox::s_buttonSize;

GuiToolbox::GuiToolbox()
{
  m_numX = 0;
  m_numY = 0;
}

GuiToolbox::~GuiToolbox()
{
}

void GuiToolbox::Init()
{
  s_buttonSize = Engine::Instance()->GetConfigFloat("toolbox_button_size");
}

void GuiToolbox::DrawImpl()
{
  // Draw each button.
  // The selected button is offset to look 'pressed'.
  // Or, each button could have a pressed and non-pressed set of textures.
  // (If in pick mode)
  // compare colour under cursor before and after drawing to decide if item is
  // selected.
  int index = 0;
  for (int i  = 0; i < m_numX; i++)
  {
    for (int j = 0; j < m_numY; j++)
    {
      GuiButton* pChild = (GuiButton*) m_children[index].GetPtr();
      pChild->SetRelPos(i * s_buttonSize, j * s_buttonSize);
      pChild->Draw(); 

      ++index;
      if (index >= GetNumberOfChildren())
      {
        // Or blank buttons for the remainder of the GuiToolbox ?
        return;
      }
    }
  }

}

bool GuiToolbox::Load(File* pf)
{
/*
  if (!GuiComposite::Load(pf))
  {
    return false;
  }
*/
  int numButtons = 0;
  if (!pf->GetInteger(&numButtons))
  {
    pf->ReportError("Expected number of GuiToolbox buttons.");
    return false;
  }
  for (int i = 0; i < numButtons; i++)
  {
    SharedPtr<GuiButton> pTb = new GuiButton;
    if (!pTb->Load(pf))
    {
      pf->ReportError("Failed to load GuiToolbox button.");
      return false;
    }
    pTb->SetSize(s_buttonSize, s_buttonSize);
    m_children.push_back(pTb.GetPtr());
  }

  // Set default size
  m_numX = (int)(sqrt(GetNumberOfChildren()) + 0.5f);
  m_numY = GetNumberOfChildren() / m_numX;
  return true;
}

void GuiToolbox::SetSize(int x, int y)
{
  m_numX = x;
  m_numY = y;
}

}
