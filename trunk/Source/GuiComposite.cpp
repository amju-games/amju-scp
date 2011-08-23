/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: GuiComposite.cpp,v $
Revision 1.1.10.2  2007/03/09 21:48:55  jay
Copy latest GUI library from trunk

Revision 1.3  2007/02/27 10:23:13  jay
Minor fix

Revision 1.2  2007/01/14 21:33:47  jay
Add more overrides

Revision 1.1  2004/09/08 15:43:07  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "GuiComposite.h"
#include "GuiFactory.h"
#include "File.h"

namespace Amju
{
GuiComposite::GuiComposite()
{
  m_canDrag = true; // top level composites are draggable
}

void GuiComposite::DrawImpl()
{
  int size = m_children.size();
  for (int i = 0; i < size; i++)
  {
    m_children[i]->Draw();
  }
}

bool GuiComposite::Load(File* pf)
{
  int numChildren = 0;
  if (!pf->GetInteger(&numChildren))
  {
    pf->ReportError("Expected number of GUI children.");
    return false;
  }
  for (int i = 0; i < numChildren; i++)
  {
    SharedPtr<GuiElement> pTb = GuiFactory::Instance()->Create(pf);
    if (!pTb->Load(pf))
    {
      pf->ReportError("Failed to load GUI child.");
      return false;
    }
    m_children.push_back(pTb);
  }
  return true;
}

void GuiComposite::OnKey(char c)
{
  int size = m_children.size();
  for (int i = 0; i < size; i++)
  {
    m_children[i]->OnKey(c);
  }
}

void GuiComposite::OnPlusUp(bool down)
{
  int size = m_children.size();
  for (int i = 0; i < size; i++)
  {
    m_children[i]->OnPlusUp(down);
  }
}

void GuiComposite::OnPlusDown(bool down)
{
  int size = m_children.size();
  for (int i = 0; i < size; i++)
  {
    m_children[i]->OnPlusDown(down);
  }
}

void GuiComposite::OnPlusLeft(bool down) 
{
  int size = m_children.size();
  for (int i = 0; i < size; i++)
  {
    m_children[i]->OnPlusLeft(down);
  }
}

void GuiComposite::OnPlusRight(bool down) 
{
  int size = m_children.size();
  for (int i = 0; i < size; i++)
  {
    m_children[i]->OnPlusRight(down);
  }
}

void GuiComposite::MousePos(int x, int y)
{
  int size = m_children.size();
  for (int i = 0; i < size; i++)
  {
    m_children[i]->MousePos(x, y);
  }
}

void GuiComposite::MouseButton(bool down, bool ctrl, bool shift)
{
  if (!IsVisible())
  {
    return;
  }

  int size = m_children.size();
  for (int i = 0; i < size; i++)
  {
    m_children[i]->MouseButton(down, ctrl, shift);
  }
}

bool GuiComposite::IsSelected() const
{
  int size = m_children.size();
  for (int i = 0; i < size; i++)
  {
    if (m_children[i]->IsSelected())
    {
      return true; 
    }
  }
  return false;
}

bool GuiComposite::CanDrag()
{
  // This composite can be dragged if it is selected, and no children
  // are selected, and the drag flag is set.
  if (m_isSelected && m_canDrag && !IsSelected())
  {
    return true;
  }
  return false;
}

void GuiComposite::AddChild(GuiElement* p)
{
  m_children.push_back(p);
  p->SetParent(this);
}

int GuiComposite::GetNumberOfChildren() const
{
  return m_children.size();
}

SharedPtr<GuiElement> GuiComposite::GetChild(int i)
{
  return m_children[i];
}

void GuiComposite::SetRelPos(float top, float left)
{
  GuiElement::SetRelPos(top, left);
  // Set the absolute pos of each child.
  int size = m_children.size();
  for (int i = 0; i < size; i++)
  {
    float childTop, childLeft;
    m_children[i]->GetRelPos(&childTop, &childLeft);
    m_children[i]->SetAbsPos(childTop + m_absTop, childLeft + m_absLeft);
  }
}

void GuiComposite::SetAbsPos(float top, float left)
{
  GuiElement::SetAbsPos(top, left);
  int size = m_children.size();
  for (int i = 0; i < size; i++)
  {
    float childTop, childLeft;
    m_children[i]->GetRelPos(&childTop, &childLeft);
    m_children[i]->SetAbsPos(childTop + m_absTop, childLeft + m_absLeft);
  }
}

}
