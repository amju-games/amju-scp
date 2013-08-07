/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PoolGuiComposite.cpp,v $
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
PoolGuiComposite::PoolGuiComposite()
{
  m_canDrag = true; // top level composites are draggable
}

void PoolGuiComposite::DrawImpl()
{
  int size = m_children.size();
  for (int i = 0; i < size; i++)
  {
    m_children[i]->Draw();
  }
}

bool PoolGuiComposite::Load(File* pf)
{
  int numChildren = 0;
  if (!pf->GetInteger(&numChildren))
  {
    pf->ReportError("Expected number of GUI children.");
    return false;
  }
  for (int i = 0; i < numChildren; i++)
  {
    RCPtr<PoolGuiElement> pTb = PoolGuiFactory::Instance()->Create(pf);
    if (!pTb->Load(pf))
    {
      pf->ReportError("Failed to load GUI child.");
      return false;
    }
    m_children.push_back(pTb);
  }
  return true;
}

void PoolGuiComposite::OnKey(char c)
{
  int size = m_children.size();
  for (int i = 0; i < size; i++)
  {
    m_children[i]->OnKey(c);
  }
}

void PoolGuiComposite::OnPlusUp(bool down)
{
  int size = m_children.size();
  for (int i = 0; i < size; i++)
  {
    m_children[i]->OnPlusUp(down);
  }
}

void PoolGuiComposite::OnPlusDown(bool down)
{
  int size = m_children.size();
  for (int i = 0; i < size; i++)
  {
    m_children[i]->OnPlusDown(down);
  }
}

void PoolGuiComposite::OnPlusLeft(bool down) 
{
  int size = m_children.size();
  for (int i = 0; i < size; i++)
  {
    m_children[i]->OnPlusLeft(down);
  }
}

void PoolGuiComposite::OnPlusRight(bool down) 
{
  int size = m_children.size();
  for (int i = 0; i < size; i++)
  {
    m_children[i]->OnPlusRight(down);
  }
}

void PoolGuiComposite::MousePos(int x, int y)
{
  int size = m_children.size();
  for (int i = 0; i < size; i++)
  {
    m_children[i]->MousePos(x, y);
  }
}

void PoolGuiComposite::MouseButton(bool down, bool ctrl, bool shift)
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

bool PoolGuiComposite::IsSelected() const
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

bool PoolGuiComposite::CanDrag()
{
  // This composite can be dragged if it is selected, and no children
  // are selected, and the drag flag is set.
  if (m_isSelected && m_canDrag && !IsSelected())
  {
    return true;
  }
  return false;
}

void PoolGuiComposite::AddChild(PoolGuiElement* p)
{
  m_children.push_back(p);
  p->SetParent(this);
}

int PoolGuiComposite::GetNumberOfChildren() const
{
  return m_children.size();
}

RCPtr<PoolGuiElement> PoolGuiComposite::GetChild(int i)
{
  return m_children[i];
}

void PoolGuiComposite::SetRelPos(float top, float left)
{
  PoolGuiElement::SetRelPos(top, left);
  // Set the absolute pos of each child.
  int size = m_children.size();
  for (int i = 0; i < size; i++)
  {
    float childTop, childLeft;
    m_children[i]->GetRelPos(&childTop, &childLeft);
    m_children[i]->SetAbsPos(childTop + m_absTop, childLeft + m_absLeft);
  }
}

void PoolGuiComposite::SetAbsPos(float top, float left)
{
  PoolGuiElement::SetAbsPos(top, left);
  int size = m_children.size();
  for (int i = 0; i < size; i++)
  {
    float childTop, childLeft;
    m_children[i]->GetRelPos(&childTop, &childLeft);
    m_children[i]->SetAbsPos(childTop + m_absTop, childLeft + m_absLeft);
  }
}

}
