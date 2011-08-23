
#ifndef ES_GUI_TEST_H_INCLUDED
#define ES_GUI_TEST_H_INCLUDED

#include "EngineState.h"

namespace Amju
{
class EsGuiTest : public EngineState
{
public:
  static const char* Name;
  virtual void Draw();
  virtual void DrawOverlays();
};
}

#endif

