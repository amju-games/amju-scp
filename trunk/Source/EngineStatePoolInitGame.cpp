#include "EngineStatePoolInitGame.h"
#include "Engine.h"
#include "PoolMisc.h"
#include "EngineStatePoolCourseSelect.h"
#include "LevelServer.h"

namespace Amju
{
const char* EngineStatePoolInitGame::Name = "init-game";

// Register this State with the Engine.
static const bool registered = Engine::Instance()->
  RegisterEngineState(EngineStatePoolInitGame::Name, new EngineStatePoolInitGame);

static int s_room = 0;

void EngineStatePoolInitGame::SetRoom(int room)
{
  s_room = room;
}

void EngineStatePoolInitGame::SetActive(bool active)
{
  if (active)
  {
    int c = EngineStatePoolCourseSelect::GetCourse();
    StartCourse(c);
     
    // Set hole number
    PLevel pLevel = LevelServer::Instance()->GetCurrentLevel();
    pLevel->SetRoomId(s_room);

#ifdef _DEBUG
std::cout << "** GAME SELECT! SETTING LEVEL ID: "
  << pLevel->GetId() << " ROOM ID: " << s_room << "\n";
#endif

    // This call has been removed fcrom StartCourse(). This fixes the bug
    // where any room > 0 has no cue ball or player characters.
    // The reason was that the EngineState was changed before the
    // new room ID was set.
    StartNewHole(); 
  }
}
}