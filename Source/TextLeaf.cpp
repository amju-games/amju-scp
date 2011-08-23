/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: TextLeaf.cpp,v $
Revision 1.1  2004/09/08 15:42:58  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "TextLeaf.h"
#include "CompositeFactory.h"
#include "Engine.h"
#include "TextWriter.h"
#include "Engine.h"
#include "File.h"

using namespace std;

namespace Amju
{
TextLeaf::TextLeaf()
{
  m_x = 0;
  m_y = 0;
}

TextLeaf::TextLeaf(const TextLeaf& rhs) : SolidComponent(rhs)
{
  m_x = rhs.m_x;
  m_y = rhs.m_y;
  m_text = rhs.m_text;
}

PSolidComponent TextLeaf::Clone()
{
  return new TextLeaf(*this);
}

void TextLeaf::Draw()
{
  TextWriter* pW = Engine::Instance()->GetTextWriter();
  pW->Print(m_x, m_y, m_text.c_str());
}

bool TextLeaf::Load(const std::string& filename)
{
  File f;

  // Prepend language directory.
  string lang = Engine::Instance()->GetConfigValue("language");
  string dash = "-"; //slash = Engine::Instance()->GetConfigValue("slash");
  string langFileName = lang + dash + filename;

  if (!f.OpenRead(langFileName))
  {
    f.ReportError("Couldn't open file.");
    return false;
  }

  return Load(&f);
}

bool TextLeaf::Load(File* pf)
{
  // Get x and y coords to print at
  if (!pf->GetFloat(&m_x))
  {
    pf->ReportError("Expected text X coord.");
    return false;
  }
  if (!pf->GetFloat(&m_y))
  {
    pf->ReportError("Expected text Y coord.");
    return false;
  }
  // Get text
  if (!pf->GetDataLine(&m_text))
  {
    pf->ReportError("Expected text.");
    return false;
  }
  /*
  // Get colour
  if (!m_colour.Load(&f))
  {
    return false;
  }
  */

  return true;
}
}

