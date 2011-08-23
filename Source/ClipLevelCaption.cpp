/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ClipLevelCaption.cpp,v $
Revision 1.1  2004/09/08 15:42:46  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "ClipLevelCaption.h"
#include "CaptionBox.h"
#include "File.h"
#include <string>

using namespace std;

namespace Amju
{
ClipLevelCaption::ClipLevelCaption() :
  m_pCaptionBox(0),
  m_isActive(true)
{
}

void ClipLevelCaption::DrawOverlays()
{
  if (m_pCaptionBox)
  {
    m_pCaptionBox->Draw();
  }
}

bool ClipLevelCaption::IsActive()
{
  return m_pCaptionBox->IsActive();
}

bool ClipLevelCaption::Load(File* pf)
{
  if (!ClipLevel::Load(pf))
  {
    return false;
  }
  // Get caption box filename
  string capName;
  if (!pf->GetDataLine(&capName))
  {
    pf->ReportError("Expected caption box filename.");
    return false;
  }
  File f;
  if (!f.OpenRead(capName))
  {
    f.ReportError("Couldn't open caption file.");
    return false;
  }
  // Now load the caption.. TODO use a factory to get the caption box subtype.
  m_pCaptionBox = new CaptionBox;
  if (!m_pCaptionBox->Load(&f))
  {
    return false;
  }
  
  return true;
}

void ClipLevelCaption::Start()
{
  m_pCaptionBox->SetActive(true);
  ClipLevel::Start();
}

void ClipLevelCaption::JoyX(float f)
{
  m_pCaptionBox->JoyX(f);
}

void ClipLevelCaption::JoyY(float f)
{
  m_pCaptionBox->JoyY(f);
}

void ClipLevelCaption::Blue(bool down)
{
  m_pCaptionBox->Blue(down);
}

void ClipLevelCaption::Green(bool down)
{
  m_pCaptionBox->Green(down);
}

void ClipLevelCaption::Red(bool down)
{
  m_pCaptionBox->Red(down);
}
}

