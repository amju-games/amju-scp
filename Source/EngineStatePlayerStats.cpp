/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStatePlayerStats.cpp,v $
Revision 1.3.8.1  2006/08/30 21:08:46  Administrator
Get Pool to build in MSVC

Revision 1.3  2004/09/23 11:53:58  Administrator
Initial stats do not increase from zero

Revision 1.2  2004/09/17 13:56:20  jay
Improvements to player stats

Revision 1.1  2004/09/15 09:12:35  Administrator
Adding player stats to game code
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EngineStatePlayerStats.h"
#include "Engine.h"
#include "PowerGuage.h"
#include "TextWriter.h"
#include "CharacterManager.h"
#include "GuiButton.h"
#include "TextFactory.h"
#include "PoolGameState.h"
#include "StringUtils.h"
#include "TextColourizerColourList.h"

namespace Amju
{
extern void ReportError(const std::string&);

void OnOkClicked()
{
  // Go to the next player's stats, or, if there are no more,
  // to to the next state.
  Engine::Instance()->GetEngineState()->Red(true);
}

void EngineStatePlayerStats::OnNewGame()
{
  // Blow away the previous stats for each player.
  ClearStats();
}

void EngineStatePlayerStats::ClearStats()
{
  m_prevStats.clear();
}

void EngineStatePlayerStats::StoreStats(int player, const std::vector<float>& stats)
{
  Assert(player < (int)m_prevStats.size());
  m_prevStats[player] = stats;
}

const std::vector<float>& EngineStatePlayerStats::GetStats(int player)
{
  Assert(player <= (int)m_prevStats.size());
  if (player == (int)m_prevStats.size())
  {
    // Requesting old stats which don't exist.
    // Assume this is ok, and just create an empty vector of stats.
    m_prevStats.push_back(std::vector<float>());
  }
  return m_prevStats[player];
}

void EngineStatePlayerStats::Red(bool down)
{
  if (!down)
  {
    return;
  }

  // If changing state do nothing
  if (GetEngine()->IsFading())
  {
    return;
  }

  // So transitions work for all players.
  m_time = 0;
  // Store the new stats for this player.
  PoolGameState::PlayerInfo* pInfo = 
    GetEngine()->GetGameState()->GetPlayerInfo(m_currentPlayer);
  Assert(pInfo);
  StoreStats(m_currentPlayer, pInfo->m_stats);

  // Show next player's stats
  ++m_currentPlayer;
  int numPlayers = GetEngine()->GetGameState()->GetNumberOfPlayers();
  Assert(m_currentPlayer <= numPlayers);
  if (m_currentPlayer == numPlayers)
  {
    // Final player has been displayed. Go to next state.
    Finished();
    // Restore current player so we can fade out if required.
    --m_currentPlayer;
  }
  else
  {
    InitPlayer();
    // TODO sound effect
  }
  
}

void EngineStatePlayerStats::Draw()
{
  AmjuGL::PushMatrix();
  // Paint background
  DrawBg();

  // Do camera work
  m_pCam->Update();
  m_pCam->Draw();

  // Lighting
  m_lighting.DrawLight();
  // Position the text
  m_or.Draw();

  // Draw the text
  if (m_pComp.GetPtr())
  {
    AmjuGL::PushMatrix();
    // position
    AmjuGL::Translate(0, m_verticalPos, 0);
    static const float statTextY = GetEngine()->GetConfigFloat("stats_text_y");
    static const float statTextZ = GetEngine()->GetConfigFloat("stats_text_z");
    AmjuGL::PushMatrix();
      AmjuGL::Translate(0, statTextY, statTextZ);
      m_pComp->Update();
      m_pComp->Draw();
    AmjuGL::PopMatrix();

    // draw a character
    static const float x = Engine::Instance()->GetConfigFloat("title_character_x");
    static const float y = Engine::Instance()->GetConfigFloat("title_character_y");
    static const float z = Engine::Instance()->GetConfigFloat("title_character_z");

    AmjuGL::PushMatrix();
    AmjuGL::Translate(x - 1.5f, y, z); 
    m_pChar->Recalculate();
    m_pChar->Draw();
    AmjuGL::PopMatrix();

    AmjuGL::PopMatrix();
  }
  AmjuGL::PopMatrix();
}

void EngineStatePlayerStats::DrawOverlays()
{
  EngineStateText::DrawOverlays();
  m_pOkButton->Draw();

  static float left = Engine::Instance()->GetConfigFloat("name_left");
  static float top = Engine::Instance()->GetConfigFloat("name_top");
  static float z = Engine::Instance()->GetConfigFloat("name_z");

  // Draw "ok" text over ok button  
  AmjuGL::PushMatrix();
  AmjuGL::Translate(left, top, z);
  AmjuGL::Translate(14.0f, -13.7f, -1.0f);
  AmjuGL::Scale(0.8f, 0.8f, 0.8f);
  m_pOkText->Draw();
  AmjuGL::PopMatrix();

  // Get playerinfo so we can compare current stat value with previous.
  PoolGameState::PlayerInfo* pInfo = 
    GetEngine()->GetGameState()->GetPlayerInfo(m_currentPlayer);
  Assert(pInfo);
  std::vector<float> oldStats = GetStats(m_currentPlayer);
  
  // Draw the stat guages
  int sz = m_nameGuages.size();
  for (int i = 0; i < sz; i++)
  {
    const std::string& name = m_nameGuages[i].first;
    RCPtr<Guage>& pGuage = m_nameGuages[i].second;

    float newval = pInfo->m_stats[i];

    Assert((int)oldStats.size() > i);
if ((int)oldStats.size() <= i)
{
std::cout << "Bad array size!!\n";
}

    float oldval = oldStats[i];
    float val = newval;
    // time to transition from old to new val
    static const float maxTime = GetEngine()->GetConfigFloat("stats_tr_time");
    if (m_time < maxTime)  // time to transition from old to new val
    {
      val = oldval + (newval - oldval) * m_time / maxTime;
    }

    pGuage->Set(val);

    pGuage->Draw();

    // Get text position from guage position.
    float x = m_textPositions[i].first;
    float y = m_textPositions[i].second;
    GetEngine()->GetTextWriter()->Print(x, y, name.c_str());
  }
}

// Get a colour given a guage number.
Colour GetColour(int i)
{
  static Colour cols[] = 
  {
    Colour(1.0f, 0, 0, 1.0f),
    Colour(1.0f, 1.0f, 0, 1.0f),
    Colour(0, 1.0f, 0, 1.0f),
    Colour(0, 1.0f, 1.0f, 1.0f),
    Colour(1.0f, 0.5f, 1.0f, 1.0f)
  };
  i = i % 5;
  return cols[i];
}

bool EngineStatePlayerStats::Load()
{
  if (!EngineStateText::Load())
  {
    return false;
  }

  std::string bg = GetEngine()->GetConfigValue("title_bg");
  if (!m_bg.Load(bg, ""))
  {
    ReportError("Failed to load stats background.");
    return false;
  }

  // Get the stats file
  std::string filename = GetEngine()->GetConfigValue("stat_file");
  File statfile;
  if (!statfile.OpenRead(filename))
  {
    statfile.ReportError("Failed to open stats file.");
    return false;
  }
  // Load guages and labels.
  int numGuages = 0;
  if (!statfile.GetInteger(&numGuages))
  {
    statfile.ReportError("Expected number of stat guages.");
    return false;
  }
  for (int i = 0; i < numGuages; i++)
  {
    std::string name;  // name of this stat
    if (!statfile.GetDataLine(&name))
    {
      statfile.ReportError("Expected stat name.");
      return false;
    }
    // Get (x, y) position for the stat name.
    float x = 0;
    float y = 0;
    if (!statfile.GetFloat(&x))
    {
      statfile.ReportError("Expected text x position.");
      return false;
    }
    if (!statfile.GetFloat(&y))
    {
      statfile.ReportError("Expected text y position.");
      return false;
    }
    m_textPositions.push_back(std::make_pair(x, y));

    // Get the "Anchor value" for the guage. This is where the coloured bar
    // starts, so if the value is the same as the anchor value, the bar
    // has zero length.
    float anchor = 0;
    if (!statfile.GetFloat(&anchor))
    {
      statfile.ReportError("Expected anchor value for guage.");
      return false;
    }

    // TODO need a factory for guages.
    // NB Should guages be derived from GUI base ??!
    PowerGuage* pPg = new PowerGuage;
    // Set the colour for this guage
    Colour c = GetColour(i);
    pPg->SetColour(c); 
    pPg->SetAnchor(anchor);
    
    RCPtr<Guage> pGuage = pPg;
    // TODO TEMP TEST
    // Get filename for this guage
    std::string guagefilename; ///= "statguage1.txt";
    if (!statfile.GetDataLine(&guagefilename))
    {
      statfile.ReportError("Expected guage filename.");
      return false;
    }
    File f;
    if (!f.OpenRead(guagefilename))
    {
      f.ReportError("Failed to load guage for player stats.");
      return false;
    }
    if (!pGuage->Load(&f))
    {
      f.ReportError("Failed to load guage for player stats.");
      return false;
    }
    m_nameGuages.push_back(std::make_pair(name, pGuage));
  }

  // Set up GUI button
  // "OK" button
  m_pOkButton = new GuiButton;
  // TODO Have a unique file
  std::string okButtonFile = GetEngine()->GetConfigValue("golf_start_button");
  if (!m_pOkButton->Load(okButtonFile))
  {
    return false;
  }
  m_pOkButton->SetSize(7.75f, 2.0f);
  m_pOkButton->SetRelPos(12.0f, 9.5f);
  m_pOkButton->SetCommand(&OnOkClicked);

  // Set up button text
  TextColourizerColourList tc1;
  tc1.AddColour(Colour(1.0f, 1.0f, 1.0f, 1.0f));
  m_pOkText = TextFactory::Instance()->Create("ok!", &tc1);

  return true;
}

void EngineStatePlayerStats::MouseButton(bool down, bool, bool)
{
  m_pOkButton->MouseButton(down, false, false);
}

void EngineStatePlayerStats::MousePos(int x, int y)
{
  m_pOkButton->MousePos(x, y);
}

void EngineStatePlayerStats::InitPlayer()
{
  PoolGameState::PlayerInfo* pInfo = 
    GetEngine()->GetGameState()->GetPlayerInfo(m_currentPlayer);
  Assert(pInfo);

  std::string charname = pInfo->m_name;
 
  m_pChar = new CharacterGameObject;
  Character* pChar = CharacterManager::Instance()->GetCharacter(charname);
  Assert(pChar);
  m_pChar->AddMesh(pChar);

  std::string playername = "player ";
  playername += ToString(m_currentPlayer + 1); // make it one-based
  playername += " -  "; 
  playername += charname;
  m_pComp = TextFactory::Instance()->Create(playername); 

  // For each stat in player info, set the guage.
  int sz = m_nameGuages.size();
  Assert((int)pInfo->m_stats.size() == sz);

  // Have any stats changed since last time ?
  // If so, play a wav
  // TODO play a happy or sad wav depending on movement - but this is hard,
  // because some stats could go up, and others down. Best if the wav is
  // neutral, just to indicate a change.
  bool hasChanged = false;
  // Compare old values with current values, where old values exist.
  
  std::vector<float> oldStats = GetStats(m_currentPlayer);
  for (int i = 0; i < sz; i++)
  {
    float newval = pInfo->m_stats[i];

    Assert((int)oldStats.size() >= i);
    if ((int)oldStats.size() == i)
    {
      oldStats.push_back(newval);
    }

    float oldval = oldStats[i];
    if (oldval != newval)
    {
      hasChanged = true;
      break;
    }
  }
  StoreStats(m_currentPlayer, oldStats);

  if (hasChanged)
  {
    std::string wav = GetEngine()->GetConfigValue("stat_change_wav");
    GetEngine()->PlayWav(wav);
  }
}

void EngineStatePlayerStats::SetActive(bool b)
{
  if (b)
  {
    m_currentPlayer = 0;

    Assert(m_currentPlayer <= GetEngine()->GetGameState()->GetNumberOfPlayers());

    InitPlayer();
  }
}

}

