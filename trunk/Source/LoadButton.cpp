#include <map>
#include <AmjuAssert.h>
#include <StringUtils.h>
#include <File.h>
#include <Vec2.h>
#include <ReportError.h>
#include "LoadButton.h"
#include "GuiButton.h"

namespace Amju
{
typedef std::pair<Vec2f, Vec2f> PosSize;
typedef std::map<std::string, PosSize> Layout;
static Layout s_layout;

bool LoadButtonLayout(const std::string& layoutfilename)
{
  File f;
  if (!f.OpenRead(layoutfilename))
  {
    f.ReportError("Failed to load button layout");
    return false;
  }

  s_layout.clear();

  std::string str;
  while (f.GetDataLine(&str))
  {
    // Should be name, pos x, pos y, size w, size h 
    // Comma separated
    Strings strs = Split(str, ',');
    Assert(strs.size() == 5);
    if (strs.size() != 5)
    {
      f.ReportError("Bad layout line, should be name, pos x, pos y, size w, size h");
      return false;
    }
    Vec2f pos(ToFloat(strs[1]), ToFloat(strs[2]));
    Vec2f size(ToFloat(strs[3]), ToFloat(strs[4]));

    // TODO Transform coord system here if required

    s_layout[strs[0]] = PosSize(pos, size);
  }

  return true;
}

void SetButtonLayout(PoolGuiButton* b, const std::string& buttonName)
{
  Layout::iterator it = s_layout.find(buttonName);
  if (it == s_layout.end())
  {
    ReportError("Unrecognised button name " + buttonName);
    Assert(0);
    return;
  }

  const PosSize& ps = it->second;
  const Vec2f& pos = ps.first;
  const Vec2f& size = ps.second;
  b->SetAbsPos(pos.y, pos.x); // top, left
  b->SetSize(size.x, size.y); // w, h
}

}


