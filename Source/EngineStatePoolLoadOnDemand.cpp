#include "EngineStatePoolLoadOnDemand.h"
#include "Engine.h"
#include "PoolBg.h"
#include "LevelServer.h"
#include "PoolCourseManager.h"

namespace Amju
{
const std::string EngineStatePoolLoadOnDemand::Name = "load-on-demand";
// Register this State with the Engine.
static const bool registered = Engine::Instance()->
  RegisterEngineState(EngineStatePoolLoadOnDemand::Name, new EngineStatePoolLoadOnDemand);

static int s_levelNum = 0;

// Go back to this previous state when loaded
static std::string s_prevStateName;

void EngineStatePoolLoadOnDemand::SetLevelToLoad(int levelNum)
{
  s_levelNum = levelNum;
}

EngineStatePoolLoadOnDemand::EngineStatePoolLoadOnDemand()
{
}

void EngineStatePoolLoadOnDemand::SetPrevState(const std::string& stateName)
{
  s_prevStateName = stateName;
}

void EngineStatePoolLoadOnDemand::SetActive(bool active)
{
#ifdef _DEBUG
  std::string levelFile = PoolCourseManager::Instance()->GetCourseFileName(s_levelNum);
  std::cout << "In load on demand...... loading level " << levelFile << "....\n";
#endif
}

void EngineStatePoolLoadOnDemand::Update()
{
  // Load all rooms up to and including s_room (or *all* rooms for that level?)

  static const int NUM_LOOPS_BEFORE_REDRAW = 1; // for smoothest frame rate?
  for (int i = 0; i < NUM_LOOPS_BEFORE_REDRAW; i++)
  {
    std::string levelFile = PoolCourseManager::Instance()->GetCourseFileName(s_levelNum);
    LoadResult r = LevelServer::Instance()->Load(levelFile, false, s_levelNum);

    if (r.IsOk())
    {
      // Finished loading this room
#ifdef _DEBUG
      std::cout << "Finished loading level " << s_levelNum << "\n";
#endif

      GetEngine()->ChangeState(s_prevStateName, Engine::IMMEDIATE);
      return;
    }
    else if (r.Failed())
    {
      // Report Error TODO
      Assert(0);
    }
  }
#ifdef LOAD_LEVEL_DEBUG
  std::cout << "Loading level " << s_levelNum << "\n";
#endif
}

bool EngineStatePoolLoadOnDemand::Load()
{

  return true;
}

void EngineStatePoolLoadOnDemand::Draw()
{
  AmjuGL::PushMatrix();
  DrawBg();
  m_pCam->Update();
  m_pCam->Draw();
  m_lighting.DrawLight();
  m_or.Draw();
  ThePoolBg::Instance()->Draw();
  AmjuGL::PopMatrix();
}

void EngineStatePoolLoadOnDemand::DrawOverlays()
{
}

}

