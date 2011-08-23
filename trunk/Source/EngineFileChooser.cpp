/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineFileChooser.cpp,v $
Revision 1.1  2004/09/08 15:42:35  jay
Added to repository
 
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EngineFileChooser.h"
#include "Engine.h"
#include "EngineRunning.h"
#include "LeafData.h"
#include "LeafRenderer.h"
#include "EngineStateShowLevel.h"

using namespace std;

namespace Amju
{
const std::string EngineFileChooser::Name = "filechoose";
// Register this State with the Engine.
//static const bool registered = Engine::Instance()->
//  RegisterEngineState(EngineFileChooser::Name, new EngineFileChooser);

void EngineFileChooser::SetActive(bool active)
{
  if (active)
  {
    // Start playing the file chooser song.
    string songFile = Engine::Instance()->GetConfigValue("file_music");
    if (!songFile.empty())
    {
      GetEngine()->PlaySong(songFile);
    }
  }
}

bool EngineFileChooser::Load()
{
  string fileMenu = Engine::Instance()->GetConfigValue("file_menu");
  if (!m_pMenu->Load(fileMenu))
  {
    return false;
  }

  if (!m_greet.Load(GetEngine()->GetConfigValue("file_greet")))
  {
    return false;
  }

  return EngineMenuState::Load();
}

void EngineFileChooser::Clear()
{
}

void EngineFileChooser::Draw()
{
  // Draw Background
  EngineMenuState::Draw();

  // The vertical position depends on whether we show keys.
  float k = atof(GetEngine()->GetConfigValue("show_key_offset").c_str());
  if (GetEngine()->GetShowKeys())
  {
    k = 0;
  }

  float y = m_greet.GetY();
  m_greet.SetY(y - k);
  m_greet.Draw(); // text greeting
  m_greet.SetY(y);

  // Draw the menu of files

  m_pMenu->Draw();

  // This is necessary if the Leaf Renderer sorts display lists before drawing.
  LeafData::GetRenderer()->DrawList();
}

void EngineFileChooser::DrawOverlays()
{
}

void EngineFileChooser::Blue(bool down)
{
  if (!down)
  {
    return;
  }

  int item = m_pMenu->Choose();
  // Item is zero-based index of chosen item.
  // Get the filename corresponding to the chosen item. Lookup "file_<n>",
  // where 0 <= n < 4
  char buf[100];
  sprintf(buf, "file_%d", item);
  string filename = GetEngine()->GetConfigValue(buf);
  if (filename.empty())
  {
    GetEngine()->ReportError("Can't get saved game state.");
  }

  // Saved Game file selected. Start Game.
  GetEngine()->StartGame(filename); 
  // initialises game

  // Go to the Running state.
//  GetEngine()->ChangeState(EngineRunning::Name, Engine::FADE);
  GetEngine()->ChangeState(EngineStateShowLevel::Name, Engine::FADE);
}

void EngineFileChooser::Red(bool down)
{
  Blue(down);
}
}
