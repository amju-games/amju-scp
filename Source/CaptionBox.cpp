/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: CaptionBox.cpp,v $
Revision 1.1  2004/09/08 15:42:45  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "CaptionBox.h"
#include "Engine.h"
#include "File.h"
#include "TextWriter.h"

namespace Amju
{
CaptionBox::CaptionBox() :
  m_isActive(false),
  m_x(0),
  m_y(0),
  m_w(0),
  m_h(0)
{
}

CaptionBox::~CaptionBox()
{
}

void CaptionBox::Draw()
{
  // Draw box - args are  t, l, b, r
  //Engine::Instance()->GetBgBox()->Draw(m_y, m_x, m_y + m_h, m_x + m_w); 
  // Draw text
  Engine::Instance()->GetTextWriter()->Print(m_x, m_y, m_text.c_str() );
}

bool CaptionBox::LoadSize(File* pf)
{
  if (!pf->GetInteger(&m_x))
  {
    pf->ReportError("Expected caption box x pos.");
    return false;
  }
  if (!pf->GetInteger(&m_y))
  {
    pf->ReportError("Expected caption box y pos.");
    return false;
  }
  if (!pf->GetInteger(&m_w))
  {
    pf->ReportError("Expected caption box width.");
    return false;
  }
  if (!pf->GetInteger(&m_h))
  {
    pf->ReportError("Expected caption box height.");
    return false;
  }
  return true;
}

bool CaptionBox::Load(File* pf)
{
  if (!LoadSize(pf))
  {
    return false;
  }
  if (!pf->GetDataLine(&m_text))
  {
    pf->ReportError("Expected caption box text.");
    return false;
  }
  return true;
}

void CaptionBox::Red(bool)
{
}

void CaptionBox::Green(bool)
{
}

void CaptionBox::Blue(bool down)
{
  // Dismisses caption
  m_isActive = false;
}

void CaptionBox::JoyX(float)
{
}

void CaptionBox::JoyY(float)
{
}
}
