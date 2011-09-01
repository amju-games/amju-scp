/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineRunning.cpp,v $
Revision 1.1.8.1  2006/08/14 17:38:03  jay
Rename "Pool"

Revision 1.1  2004/09/08 15:42:36  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <algorithm>
#include "EngineRunning.h"
#include "GameState.h"
#include "EnginePaused.h"
//#include "EngineDeadState.h"
#include "CharacterGameObject.h"
#include "CharacterManager.h"
#include "HeightServer.h"
#include "Engine.h"
#include "TextWriter.h"
#include "Level.h"
#include "GameObject.h"
#include "Camera.h"
#include "LevelServer.h"
#include "Geometry.h"
#include "LeafData.h"
#include "LeafRenderer.h"
#include "Npc.h"
#include "Frustum.h"
#include "TextureServer.h"
#include "ThirdPersonCamera2.h"
#include "ThirdPersonCameraFixed.h"
#include "FirstPersonCamera.h"
#include "SchAssert.h"
#include "KeymapPrinter.h"
#include "LevelServer.h"
#include "Platform.h"

using namespace std;

namespace Amju
{
const std::string EngineRunning::Name = "running";
// Register this State with the Engine.
static const bool registered = Engine::Instance()->
  RegisterEngineState(EngineRunning::Name, new EngineRunning);

EngineRunning::EngineRunning()
{
  m_showControls = true;

  m_heightFallenFrom = 0;
  m_maxForwardVel = 0;
  m_playerAccel = 0;
  m_playerRotateVel = 0;
  m_walkForwardVel = 0;

  m_currentMaxForwardVel = 0;

  m_pTimeGuage = new PowerGuage;

}

EngineRunning::~EngineRunning()
{ 
  Clear();
}

void EngineRunning::SetControlStyle(ControlStyle* pCs)
{
  m_pControlStyle = pCs;
  m_pControlStyle->Reset();
}

void EngineRunning::SetControlStyle(ControlStyleEnum cs)
{
  // Turn off everything the player is doing
  GetPlayer()->SetForwardVel(0);
  GetPlayer()->SetForwardAccel(0);
  GetPlayer()->SetYRotateVel(0);

  switch (cs)
  {
  case ROTATE_KEYS:
    //GetEngine()->GetEngineState()->Set("control_style", "rkeys");
    //SetControlStyle(new ControlStyleRotateKeys);
    //TheKeymapPrinter::Instance()->SetMode(KeymapPrinter::KEYS);
    break;
  case DIRECTIONAL_KEYS:
    //GetEngine()->GetEngineState()->Set("control_style", "dirkeys");
    //SetControlStyle(new ControlStyleDirectionalKeys);
    //TheKeymapPrinter::Instance()->SetMode(KeymapPrinter::KEYS);
    break;
  case MOUSE_PICK:
    //GetEngine()->GetEngineState()->Set("control_style", "mouse");
    //SetControlStyle(new ControlStyleMousePickBase);
    break;
  case JOYSTICK:
    //GetEngine()->GetEngineState()->Set("control_style", "joystick");
    //SetControlStyle(new ControlStyleJoystick);
    //TheKeymapPrinter::Instance()->SetMode(KeymapPrinter::JOYSTICK);
    break;
  }
}

void EngineRunning::SetControlStyleFromConfig()
{
  std::string s = GetEngine()->GetGameState()->Get("control_style");
  if (s == "joystick")
  {
    // But check if there's a joystick connected. If not, go to
    //  keyboard control style.
    if (GetEngine()->IsJoystickEnabled())
    {
      SetControlStyle(JOYSTICK);
    }
    else
    {
      SetControlStyle(ROTATE_KEYS);
    }
  }
  else if (s == "mouse")
  {
    SetControlStyle(MOUSE_PICK);
  }
  else if (s == "rkeys")
  {
    SetControlStyle(ROTATE_KEYS);
  }
  else if (s == "dirkeys")
  {
    SetControlStyle(DIRECTIONAL_KEYS);
  }
}

void EngineRunning::SetCameraStyle(Camera* pCam)
{
  m_pCamera = pCam;
  m_pCamera->SetPlayer(GetPlayer());
  m_pCamera->GetConfigValues();
  m_pCamera->Reset();
}

void EngineRunning::SetCameraStyle(CameraEnum c)
{
  // Turn off everything the player is doing
  GetPlayer()->SetForwardVel(0);
  GetPlayer()->SetForwardAccel(0);
  GetPlayer()->SetYRotateVel(0);

  switch (c)
  {
  case FOLLOW:
    GetEngine()->GetGameState()->Set("camera_style", "follow");
    SetCameraStyle(new ThirdPersonCamera2);
    m_firstPerson = false;
    break;

  case BIRDS_EYE:
    GetEngine()->GetGameState()->Set("camera_style", "birds_eye");
    SetCameraStyle(new ThirdPersonCamera2);
    m_firstPerson = false;
    break;

  case FIRST_PERSON:
    GetEngine()->GetGameState()->Set("camera_style", "amju_cam");
    SetCameraStyle(new FirstPersonCamera);
    m_firstPerson = true;
    break;

  case FIXED:
    GetEngine()->GetGameState()->Set("camera_style", "fixed");
    SetCameraStyle(new ThirdPersonCameraFixed);
    m_firstPerson = false;
    break;

  }
}

void EngineRunning::SetCameraStyleFromConfig()
{
  std::string s = GetEngine()->GetGameState()->Get("camera_style");
  if (s == "follow")
  {
    SetCameraStyle(FOLLOW);
  }
  else if (s == "amju_cam")
  {
    SetCameraStyle(FIRST_PERSON);
  }
  else if (s == "fixed")
  {
    SetCameraStyle(FIXED);
  }
}

bool EngineRunning::IsFirstPerson() const
{
  return m_firstPerson;
}

void EngineRunning::GameOver()
{
#if defined(_DEBUG)

  cout << "*** GAME OVER ***" << endl;

#endif

  // Player is dead. Set action to death, stop moving.
  //EngineRunning* pEr = (EngineRunning*)GetEngineState(EngineRunning::Name).GetPtr();

//// TODO This is here because there is no "death" animations in the characters!
//////pEr->GetPlayer()->SetActionDead();
  GetPlayer()->SetActionIdle();

  GetPlayer()->SetForwardVel(0);
  GetPlayer()->SetYRotateVel(0);

  // Set engine state to Death.
//  GetEngine()->ChangeState(EngineDeadState::Name, Engine::IMMEDIATE); 
}

void EngineRunning::OnNewGame()
{
  SetControlStyleFromConfig();
  SetCameraStyleFromConfig();

  m_pLevel = LevelServer::Instance()->GetCurrentLevel();

  //  NB Don't call OnNewLevel();

  m_score.Reset();

  // Set player state, so shadow is shown.
  GetPlayer()->SetState(UNKNOWN);
}

void EngineRunning::OnNewLevel()
{
  // DON'T do anything here, for Pool.
  return;
/*
  // Initialises to start of current level.
  std::string levelFilename = Engine::Instance()->GetConfigValue("levels");
  if (LevelServer::Instance()->Load(levelFilename, false, 0).Failed())
  {
    Assert(0);
  }
  
  m_pLevel = LevelServer::Instance()->GetCurrentLevel();

  // Tell the player character what level we're in.
  GetPlayer()->SetLevel(m_pLevel.GetPtr());


  // Set the starting room.
  m_pLevel->SetRoomId(m_pLevel->GetStartRoomId());


  // TODO 'Level creator'/reset takeables


  // Rebuild the visibility graph. We must do this here, but elsewhere we
  // can get away with updating only parts of the graph.
//////  RecalcVisGraph(m_pLevel);

  SetPlayerRoom(m_pLevel->GetStartRoomId());

  // Set the starting player position.
  GetPlayer()->SetOrientation(m_pLevel->GetStartOrientation());
  
  GetPlayer()->SetActionIdle(); // Initially standing idle.

  // Set the camera orientation to the same point as the player. This will be adjusted
  // to the min. distance from the player, but means we start off with the 
  // player in sight.
  GetCamera()->SetOrientation(*(GetPlayer()->GetOrientation()));

  // Start with 3rd person camera. Point it at the Player.
  GetCamera()->SetPlayer(GetPlayer());
  GetCamera()->Reset();
  // Get the config values for the camera ??

  // TODO In fact reset ALL control styles, as they can be enabled at any time.
  m_pControlStyle->Reset();

  // Reset all piece movers
  //GetPlayer()->ResetPieceMover();
  ResetAis();

  CooperationGroupManager::Instance()->ClearAllGroups();

  // NB This code is the same as Ai::RoomEntry()
  // This looks like a good time to join a Cooperation Group.
  // Join the group with the same name as the character - so
  // all characters which look the same work together.
  // TODO clear from any previously joined group
  const std::string& groupName = "name"; // TODO - all in same group!
  //CooperationGroupManager::Instance()->JoinOrCreateGroup(GetPlayer(), groupName);

  MoveMemorizer::Instance()->Clear();

  // Set time for level
  float levelTime = GetEngine()->GetConfigFloat("level_time");
  // NB The time limit is proportional to the level number, up to some limit.
  int levelId = m_pLevel->GetId();
  float timeMult = levelId + 1;
  float maxTimeMult = GetEngine()->GetConfigFloat("max_time_mult");
  if (timeMult > maxTimeMult)
  {
    timeMult = maxTimeMult;
  }
  levelTime *= timeMult;
  m_pTimeGuage->SetPowerTime(levelTime);
*/
}

void EngineRunning::IncreaseScore(int i)
{
  int s = m_score.Get();
  s += i;
  m_score.Set(s);
}

// SetActive() is called when this state becomes active - in this case when 
// menu is exited, or game started, etc.
void EngineRunning::SetActive(bool active)
{
  // Make sure level exists.
  m_pLevel = LevelServer::Instance()->GetCurrentLevel();

  if (active)
  {
    GetEngine()->PlaySong(m_pLevel->GetMusicFilename());
    GetPlayer()->SetState(UNKNOWN);
  }
  else
  {
    //GetEngine()->StopMidi(m_pLevel->GetMidiFilename());
  }
}

void EngineRunning::CallbackSongFinished(const std::string& file)
{
  // TODO check if active ?
  // TODO Interactive/procedural music. For now, we simply start playing the
  // same MIDI file again.
  GetEngine()->PlaySong(m_pLevel->GetMusicFilename());
}

bool EngineRunning::Load()
{
  /*
  // Get health texture (heart ?)
  if (!m_heart.Load(GetEngine()->GetConfigValue("health"), 
                     GetEngine()->GetConfigValue("health_alpha")))
  {
    GetEngine()->ReportError("Failed to load player health icon.");
    return false;
  } 
  */

  // Run speed
  m_maxForwardVel = atof(GetEngine()->GetConfigValue("player_fwd_vel_run").c_str());
  // Walk speed
  m_walkForwardVel = atof(GetEngine()->GetConfigValue("player_fwd_vel_walk").c_str());
  // Acceleration from walking to running speed
  m_playerAccel = atof(GetEngine()->GetConfigValue("player_walk_accel").c_str());

  m_currentMaxForwardVel = m_maxForwardVel;
  m_playerRotateVel = atof(GetEngine()->GetConfigValue("player_rot_vel").c_str());

  // Blast wave centred on player
  /*
  string bwTex = GetEngine()->GetConfigValue("player_bw_texture");
  string bwAlpha = GetEngine()->GetConfigValue("player_bw_alpha");

  Texture* pBwTex = TextureServer::Instance()->Get(bwTex, bwAlpha);
  if (!pBwTex)
  {
    GetEngine()->ReportError("Failed to load blast wave texture.");
    return false;
  }
  */

  // TODO CONFIG!
//  PieceFootprint::SetTexture(TextureServer::Instance()->Get("diag.bmp", ""));

  File ftimer;
  if (!ftimer.OpenRead(Engine::Instance()->GetConfigValue("powertimer")))
  {
    GetEngine()->ReportError("Bad timer file.");
    return false;
  }
  if (!m_pTimeGuage->Load(&ftimer))
  {
    GetEngine()->ReportError("Failed to load timer.");
    return false;
  }

  m_score.Init();

  return true;
}

void EngineRunning::Clear()
{
}

void EngineRunning::DrawPlayer()
{
  if (!IsFirstPerson())
  {
    EngineRunningBase::DrawPlayer();
  }

  // Draw currently held object if any
  //GetPlayer()->DrawTakenPiece();

  // This looks like a good place to draw the footprint of any piece carried.
  //GetPlayer()->DrawPieceFootprint();
}

void EngineRunning::Update()
{
  EngineRunningBase::Update();

  m_pControlStyle->Update();
  GetCamera()->Update();

  PlayerFellOffWorldTest();
}

void EngineRunning::Draw()
{
  m_pControlStyle->Draw();

  // Get window size
  m_windowX = GetEngine()->GetWindowX();
  m_windowY = GetEngine()->GetWindowY();

  // Normal drawing
  EngineRunningBase::Draw();
}

void EngineRunning::MousePos(int x, int y)
{
  m_pControlStyle->MousePos(x, y);
  m_pCamera->MousePos(x, y);
}

void EngineRunning::MouseButton(bool down, bool ctrl, bool shift)
{
  m_pControlStyle->MouseButton(down, ctrl, shift);
  m_pCamera->MouseButton(down, ctrl, shift);
}

void EngineRunning::DrawHeart(int position)
{
  // If player has lots of hearts, they are drawn up the screen.
  int col = position % 5;
  int row = position / 5;

  m_heart.Draw(13 - row * 2, 1 + col * 2, 15 - row * 2, 3 + col * 2); // t, l, b, r
}

void EngineRunning::DrawOverlays()
{
  // j.c. 1/1/2003 - hearts disabled
/*
  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ);
  AmjuGL::PushMatrix();
  // Draw player health, starting in bottom left corner.
  float health = GetEngine()->GetEngineState()->GetPlayerHealth();
  float maxHealth = GetEngine()->GetEngineState()->GetMaxHealth();

  // Draw integer health, then remainder.
  //int wholeHealth = (int)(health);
  for (int i = 0; i < maxHealth; i++)
  {
    // Draw fractional part.
    float f = health - (float)i;
    if (f < 0)
    {
      f = 0;
    }
    if (f > 1)
    {
      f = 1;
    }
    // As f gets smaller, we go more blue.
    GetEngine()->PushColour(f, f, 1, 1);
    DrawHeart(i); 
    // Restore colour.
    GetEngine()->PopColour();
  }
  AmjuGL::PopMatrix();
  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_READ);
*/

  // Score etc in top left corner
  //int score = GetEngine()->GetEngineState()->GetFloat("score");
  //m_score.Set(score);
  //m_score.Draw();
  //m_pTimeGuage->Draw();

//  GetEngine()->Fps();
}

void EngineRunning::MultPlayerSpeed(float speedMult)
{
  bool changeSpeed = (GetPlayer()->GetForwardVel() == m_currentMaxForwardVel);

  m_currentMaxForwardVel *= speedMult;

  if (changeSpeed)
  {
    GetPlayer()->SetForwardVel(m_currentMaxForwardVel);
  }
}

void EngineRunning::RestorePlayerSpeed()
{
  bool changeSpeed = (GetPlayer()->GetForwardVel() == m_currentMaxForwardVel);

  m_currentMaxForwardVel = m_maxForwardVel;

  if (changeSpeed)
  {
    GetPlayer()->SetForwardVel(m_currentMaxForwardVel);
  }
}

void EngineRunning::YellowUp(bool down)
{
  m_pControlStyle->YellowUp(down);
  m_pCamera->YellowUp(down);
}

void EngineRunning::Blue(bool down)
{
  m_pControlStyle->Blue(down);
  m_pCamera->Blue(down);
}

void EngineRunning::Green(bool down)
{
  m_pControlStyle->Green(down);
  m_pCamera->Green(down);
}

void EngineRunning::Red(bool down)
{
  if (!down)
  {
    return;
  }
  JoyX(0);
  JoyY(0);
  Engine::Instance()->ChangeState(EnginePaused::Name, Engine::IMMEDIATE);

//  m_pControlStyle->Red(down);
}

void EngineRunning::Z(bool down)
{ 
  m_pControlStyle->Z(down);
  m_pCamera->Z(down);
}

void EngineRunning::JoyX(float f)
{
  m_pControlStyle->JoyX(f);
  m_pCamera->JoyX(f);
}

void EngineRunning::JoyY(float f)
{
  m_pControlStyle->JoyY(f);
  m_pCamera->JoyY(f);
}

void EngineRunning::PlusUp(bool b)
{
  m_pControlStyle->PlusUp(b);
  m_pCamera->PlusUp(b);
}

void EngineRunning::PlusDown(bool b)
{
  m_pControlStyle->PlusDown(b);
  m_pCamera->PlusDown(b);
}

void EngineRunning::PlusLeft(bool b)
{
  m_pControlStyle->PlusLeft(b);
  m_pCamera->PlusLeft(b);
}

void EngineRunning::PlusRight(bool b)
{
  m_pControlStyle->PlusRight(b);
  m_pCamera->PlusRight(b);
}

void EngineRunning::ShowStats()
{
  int totalPolys = m_pLevel->GetNumberOfPolygons(); 
  int drawnPolys = m_scenePolys;

  char buf[100];
  sprintf(buf, "Polygons: %d, drawn: %d", totalPolys, drawnPolys);
  
  GetEngine()->GetTextWriter()->ScrollPrint(buf);
  //sprintf(buf, "Action: %s", GetPlayer()->GetCharacter()->GetActionName().c_str());
  //GetEngine()->GetTextWriter()->ScrollPrint(buf);

  // Active platform
  Platform* p = GetPlayer()->GetActivePlatform();
  if (p)
  {
    sprintf(buf, "ACTIVE PLATFORM: %d", p->GetId() );
    GetEngine()->GetTextWriter()->ScrollPrint(buf);
  }
  else
  {
    GetEngine()->GetTextWriter()->ScrollPrint("No active platform");
  }

  // Is Falling status
  if (GetPlayer()->IsFalling())
  {
    GetEngine()->GetTextWriter()->ScrollPrint("Player IS FALLING");
  }
  else
  {
    GetEngine()->GetTextWriter()->ScrollPrint("Player is not falling");
  }
}

void EngineRunning::PlayerFellOffWorldTest()
{
  static const float FELL_OFF_WORLD_HEIGHT = -50.0f; 

  if (GetPlayer()->GetHeight() < FELL_OFF_WORLD_HEIGHT)
  {
    //m_pBlastWave->Reset(m_player.GetPosition());
  }

  if (GetPlayer()->GetHeight() < (FELL_OFF_WORLD_HEIGHT - 30.0f))
  {
    // TODO Play fell off world sound.
    // TODO fade in/out ?
    // Start at the beginning of the level again - but Game Objects are not reset.
    // TODO if this is too harsh, just selectively re-initialise. E.g. don't go
    // back to the start room.
    // TODO Each room has a room/orientation for this case ?
    //Initialise();
    //OnNewLevel();
    // Set the starting player position.
    GetPlayer()->SetOrientation(m_pLevel->GetStartOrientation());

    /*
    TODO this is done in Draw() so should be ok.
    GetPlayer()->RefreshHeightServer();
    */
    // We must zero the player's velocity etc.
    GetPlayer()->SetForwardVel(0);
    GetPlayer()->SetYRotateVel(0);
    GetPlayer()->SetActivePlatform(0); // TODO does this prevent resetting to on a platform ?
    GetPlayer()->SetFalling(false);
  }
}

void EngineRunning::ExtractPlayerFromWall()
{
  //CharacterControl::ExtractCharacterFromWall(GetPlayer());
}

void EngineRunning::IncreaseLevelTime(float secs)
{
  m_pTimeGuage->IncreaseTime(secs);
}

}

