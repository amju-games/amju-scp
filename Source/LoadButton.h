#ifndef LOAD_BUTTON_H_INCLUDED
#define LOAD_BUTTON_H_INCLUDED

#include <string>

namespace Amju
{
class PoolGuiButton;

// Decide at run time on button layout
// Choose file based on device or viewport size etc
bool LoadButtonLayout(const std::string& layoutfilename);

void SetButtonLayout(PoolGuiButton*, const std::string& buttonName);
}

#endif

