/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: HelpText.cpp,v $
Revision 1.1.2.3  2006/08/30 21:12:17  Administrator
Get Pool to build in MSVC

Revision 1.1.2.2  2006/08/14 17:50:20  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:36  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.2  2005/08/26 21:49:23  jay
Centre text vertically

Revision 1.1.2.1  2005/07/05 09:42:34  jay
Added help state

*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "HelpText.h"
#include "Engine.h"
#include "TextWriter.h"
#include "File.h"
#include "StringUtils.h"

#define HELPTEXT_CENTRED

namespace Amju
{
bool HelpText::Load(const std::string& filename)
{
  File f;
  if (!f.OpenRead(filename))
  {
    f.ReportError("Failed to load help text.");
    return false;
  }  
  int numLines = 0;
  if (!f.GetInteger(&numLines))
  {
    f.ReportError("Expected number of lines for helptext.");
    return false;
  }
  std::string s;
  for (int i = 0; i < numLines; i++)
  {
    if (!f.GetDataLine(&s))
    {
      std::string e = "Expected line ";
      e += ToString(i);
      e += " of helptext.";
      f.ReportError(e);
      return false;
    }
    m_lines.push_back(s);
  }
  return true;
}

void HelpText::Draw(float y)
{
  static const float line_spacing = Engine::Instance()->GetConfigFloat(
    "help_line_spacing");

  int s = m_lines.size();

  static const float CENTRE_LINE = 6.5f;
  // Vertically centre the text - ignores parameter
  y = CENTRE_LINE - (float)s * 0.5f * line_spacing; 

  for (int i = 0; i < s; i++)
  {
#ifdef HELPTEXT_CENTRED
    Engine::Instance()->GetTextWriter()->PrintCentre(y, m_lines[i].c_str());
#else
    Engine::Instance()->GetTextWriter()->Print(x, y, m_lines[i].c_str());
#endif    

    y += line_spacing;
  }
}
}


