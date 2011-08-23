/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: HelpText.h,v $
Revision 1.1.2.2  2006/08/14 17:50:20  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:36  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.1  2005/07/05 09:42:34  jay
Added help state

*/

#ifndef HELP_TEXT_H_INCLUDED
#define HELP_TEXT_H_INCLUDED

#include <string>
#include <vector>

namespace Amju
{
class HelpText
{
public:
  bool Load(const std::string& filename);

  // Draw the text at the given position. This can be off the screen, 
  // for scrolling.
  void Draw(float y);

protected:
  std::vector<std::string> m_lines;
};
}

#endif

