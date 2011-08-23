/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateTextGameOver.cpp,v $
Revision 1.1  2004/09/08 15:42:37  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EngineStateTextGameOver.h"
#include "EngineStateTitle.h"
//#include "EngineStateIntro.h"
#include "Engine.h"
#include "FunctionFactory.h"
#include "Text3dLeaf.h"

using namespace std;
namespace Amju
{
const char* EngineStateTextGameOver::Name = "textgameover"; 
// Register this State with the Engine.
static const bool registeredtextgameover = Engine::Instance()->
  RegisterEngineState(EngineStateTextGameOver::Name, new EngineStateTextGameOver);

EngineStateTextGameOver::EngineStateTextGameOver() 
{
  m_verticalPos = 0; 
}

void EngineStateTextGameOver::TimerExpired()
{
  // Go to TITLE State.
  GetEngine()->ChangeState(EngineStateTitle::Name, Engine::IMMEDIATE);
}

bool EngineStateTextGameOver::Load()
{
  // Get 3D letters
//  string comp = GetEngine()->GetConfigValue("game_over_comp");
  m_pComp = new Text3dLeaf(".game over.");
  if (!m_pComp.GetPtr())
  {
    GetEngine()->ReportError("Failed to load Game Over letters.");
    return false;
  }

  string bg = GetEngine()->GetConfigValue("game_over_bg");
  if (!m_bg.Load(bg, ""))
  {
    return false;
  }

  // Load function which moves text.
  string funcType = GetEngine()->GetConfigValue("game_over_func_type");
  m_pFunc = FunctionFactory::Instance()->Create(funcType);
  if (!m_pFunc.GetPtr())
  {
    GetEngine()->ReportError("Bad function type for Game Over text.");
    return false;
  }
  string funcFileName = GetEngine()->GetConfigValue("game_over_func_file");
  File funcfile;
  if (!funcfile.OpenRead(funcFileName))
  {
    GetEngine()->ReportError("Couldn't open Game Over function file.");
    return false;
  }
  if (!m_pFunc->Load(&funcfile))
  {
    GetEngine()->ReportError("Failed to load Game Over text function.");
    return false;
  }

  m_maxTime = atof(GetEngine()->GetConfigValue("game_over_text_time").c_str());
  if (m_maxTime == 0)
  {
    GetEngine()->ReportError("No Game Over text time.");
    return false;
  }
  return true;
}

void EngineStateTextGameOver::Red(bool down)
{
  if (down)
  {
    GetEngine()->ChangeState(EngineStateTitle::Name, Engine::IMMEDIATE);
  }
}

void EngineStateTextGameOver::Draw()
{
  EngineStateText::Draw();
}

}
