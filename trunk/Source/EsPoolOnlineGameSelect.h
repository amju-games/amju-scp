/*
Amju Games source code (c) Copyright Jason Colman 2007
*/

#ifndef ES_POOL_ONLINE_GAME_SELECT_H_INCLUDED
#define ES_POOL_ONLINE_GAME_SELECT_H_INCLUDED

#include "EngineStatePoolCourseSelect.h"

namespace Amju
{
class EsPoolOnlineGameSelect : public EngineStatePoolCourseSelect
{
public:
  static const char* Name;

  virtual bool Load();
};
}

#endif

