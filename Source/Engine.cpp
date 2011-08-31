/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Engine.cpp,v $
Revision 1.2.6.27  2007/08/02 08:47:19  jay
No FPS in release build

Revision 1.2.6.26  2007/07/03 23:09:32  Administrator
Check for existence of sound player.
Temp: always show FPS

Revision 1.2.6.25  2007/06/01 08:17:19  jay
Speed up load by loading 10 Level Workers before redrawing

Revision 1.2.6.24  2007/03/23 22:54:22  Administrator
No frame rate

Revision 1.2.6.23  2007/03/09 23:51:59  Administrator
Hmm, this assert always fires

Revision 1.2.6.22  2007/03/09 23:01:58  jay
POOL_ONLINE: special version of GetDeltaTime

Revision 1.2.6.21  2006/08/17 18:14:19  jay
Show FPS so we can check fixed frame rate

Revision 1.2.6.20  2006/08/14 17:37:29  jay
Rename "Pool"

Revision 1.2.6.19  2006/07/26 21:25:23  jay
Use Pool/, not Pool/

Revision 1.2.6.18  2006/05/04 17:43:49  jay
Extra call to set viewport, attempt to fix full screen bug.

Revision 1.2.6.17  2006/03/23 08:28:45  jay
Stop current song when Fade ends

Revision 1.2.6.16  2006/03/21 19:13:17  jay
Try to fix confusing config file error messages

Revision 1.2.6.15  2006/03/18 23:23:13  Administrator
StopSong() takes one param only

Revision 1.2.6.14  2006/03/12 00:35:18  Administrator
Fix for new SoundPlayer::StopSong interface

Revision 1.2.6.13  2005/12/12 13:29:32  jay
Remove load font char widths - now in FontManager

Revision 1.2.6.12  2005/10/29 10:15:10  jay
Add support for OS-specific special events.
E.g. Paste command.

Revision 1.2.6.11  2005/09/29 19:02:13  jay
Allow Earthquakes when Engine State changes. This may turn out to be
a bad idea, but for POOL it is so bombs which end the game can cause
earthquakes.

Revision 1.2.6.10  2005/09/06 17:20:06  jay
Turn off debug output

Revision 1.2.6.9  2005/08/26 21:11:13  jay
New font loading code, using FontManager

Revision 1.2.6.8  2005/08/02 18:34:08  jay
Debug output only

Revision 1.2.6.7  2005/07/30 23:36:29  jay
Load all levels up front, not just level 0.

Revision 1.2.6.6  2005/07/17 23:00:54  jay
Handle Right and Middle mouse buttons

Revision 1.2.6.5  2005/07/05 09:56:34  Administrator
No FPS for release build

Revision 1.2.6.4  2005/06/22 21:46:10  jay
Added some debug output

Revision 1.2.6.3  2005/06/04 22:24:48  jay
Fix to check if new song is same as current song

Revision 1.2.6.2  2005/05/15 17:11:06  jay
Show FPS for now

Revision 1.2.6.1  2005/03/17 20:48:58  jay
Pool: temp hack, only one level

Revision 1.2  2004/09/15 09:18:08  Administrator
Negligible change

Revision 1.1  2004/09/08 15:42:48  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#if defined(WIN32)
// This is to make sure <winsock2.h> gets included before <windsock.h>
#include "Socket.h"
#endif

#include <iostream>
#include "Engine.h"
#include "Level.h"
#include "TextWriter.h"
#include "TextureServer.h"
#include "SoundPlayer.h"
#include "CharacterManager.h"
#include "Controller.h"
#include "LevelServer.h"
#include "GameState.h"
#include "SoundServer.h"
#include "PowerUpServer.h"
#include "MessageSerializer.h"
#include "ClientSocket.h"
#include "HeightServer.h"
#include "Shadow.h"
#include "BasicShadow.h"
#include "BlockShadow.h"
#include "EarthquakeServer.h"
#include "TextWriter.h"
#include "TextFactory.h"
#include "Player.h"
#include "File.h"
#include "Mouse.h"
#include "KeymapPrinter.h"
#include "PoolCourseManager.h"
#include "StringUtils.h"
#include "Font.h"
#include "Number3d.h"
#include "SchAssert.h"

#if defined(SCENE_EDITOR)
#include "EngineStateNull.h"
#endif

using namespace std;

namespace Amju
{
const float ASPECT_RATIO = 1.5f;
const float LETTERBOX_ASPECT_RATIO = 1.8f;
const float FIELD_OF_VIEW = 45.0f;
const float NEAR_PLANE = 0.5f;
const float VERY_NEAR_PLANE = 0.1f;
const float FAR_PLANE = 1500.0f;
// IDs for special message recipients
const int   SOUND_SERVER = -999;
const int   CUT_SCENE_SERVER = -888;
const int   POWER_UP_SERVER = -777;
const int   POWER_UP_HEALTH = -776;
const int   POWER_UP_MAX_HEALTH = -775;
const int   POWER_UP_FLIGHT = -774;
const int   POWER_UP_SPEED = -773;
const int   EARTHQUAKE_SERVER = -772;
const int   POWER_UP_AMMO = -771;

// We can declare this non-member function as extern in other files, avoiding
// having to include Engine.h.
void ReportError(const string& error)
{
  Engine::Instance()->ReportError(error);
}

Engine* Engine::Instance()
{
  static Engine e;
  return &e;
}

Engine::Engine() 
{
  m_pCurrentState = 0; // Start in file checking mode.
  m_pNewState = 0;

  m_numFrames = 0;
  m_elapsedSecs = 0;
  m_oldElapsedSecs = 0;
  m_deltaTime = 0;

  m_pSoundPlayer = 0; // Created in OS-specific code.

  m_isFading = false;
  m_fade = 0;

  m_showStats = false;
  m_showSpheres = false;

  // Start with white screen
  SetClearColour(1.0f, 1.0f, 1.0f);

  SetLetterbox(false);

  m_pGameState = new GameState;
  
  m_pTextWriter = new TextWriter;

  m_globalR = 1.0f;
  m_globalG = 1.0f;
  m_globalB = 1.0f;

  m_startLevel = 0;

  m_joystickEnabled = false;
  m_prevLeft = false;
  m_prevRight = false;
  m_joystickZeroZone = 0;

  m_maxDeltaTime = 0.04f;

  m_frozen = false;
  m_frozenTime = 0;

  m_showKeys = false;

  m_pPlayer = new Player;

  m_levelIsCompleted = false;

#if defined(SCENE_EDITOR)
  // Start with an Engine State that does nothing.
  RegisterEngineState("null", new EngineStateNull);
  SetState(GetEngineState("null"));
#endif
}

Engine::~Engine()
{
//  CleanUp(); // causes crash!?!
}

int Engine::GetWindowX() const
{
  return m_viewportX;
}

int Engine::GetWindowY() const
{
  return m_viewportY;
}

Player* Engine::GetPlayer()
{
  return m_pPlayer.GetPtr();
}

bool Engine::RegisterEngineState(const std::string& stateName, EngineState* pState)
{
  m_engineStateMap[stateName] = pState;
  return true;
}

LoadResult Engine::LoadEngineStates()
{
  // Progress bar
  // Sadly we can't load in a separate thread, as we make gl calls which
  // require the render context of the main thread :-(
  // So load one EngineState at a time. Return MORE until the
  // final state has been loaded.

  // Call Load() on each engine state.
  static EngineStateMap::iterator it = m_engineStateMap.begin(); 

  PEngineState pState = it->second;
#ifdef _DEBUG
  std::cout << "Loading state: " << it->first.c_str() << ": " << pState.GetPtr() << "\n";
#endif    
  Assert(pState.GetPtr());
  if (pState->Load())
  {
    ++it;
  }
  else
  {
    return LoadResult::FAIL;
  }

  if (it == m_engineStateMap.end())
  {
#ifdef _DEBUG
std::cout << "Loaded all engine states ok.\n";
#endif
    return LoadResult::OK; // finished loading
  }

  return LoadResult::MORE; 
}

/*void Engine::PushState(const std::string& newStateName)

{
  m_stateStack.push(m_pCurrentState);
  ChangeState(newStateName, IMMEDIATE_WITH_RESET);
}

void Engine::PopState()
{
  Assert(!m_stateStack.empty());
  PEngineState prevState = m_stateStack.top();
  m_stateStack.pop();
  ChangeState(prevState, IMMEDIATE_WITH_RESET);
}*/

void Engine::ChangeState(const std::string& newStateName, Engine::ChangeMethod cm)
{
/*
  EngineStateMap::iterator it = m_engineStateMap.find(newStateName);
  if (it == m_engineStateMap.end())
  {
    Assert(0);
    return; // Bad state name.
  }

  PEngineState pState = it->second;
*/
  PEngineState pState = GetEngineState(newStateName);
  Assert(pState.GetPtr());
  ChangeState(pState, cm);
}

void Engine::ChangeState(PEngineState pState, Engine::ChangeMethod cm)
{
  // Ignore if the new state is the same as the current state.
  if (pState.GetPtr() == m_pCurrentState.GetPtr())
  {
    return;
  }

  if (cm == IMMEDIATE)
  {
    if (!m_isFading)
    {
      SetState(pState);
      GetTextWriter()->ClearScrollingItems(); // clear last FPS count
    }
  }
  else if (cm == FADE)
  {
    SetFadeState(pState);
  }
}

PCamera Engine::GetCamera()
{
  Assert(m_pCurrentState.GetPtr());
  return m_pCurrentState->GetCamera();
}

PEngineState Engine::GetEngineState(const std::string& stateName)
{
  EngineStateMap::iterator it = m_engineStateMap.find(stateName);
  if (it == m_engineStateMap.end())
  {
    Assert(0);
    return 0; // Bad state name.
  }
  return it->second;
}

PEngineState Engine::GetEngineState()
{
  return m_pCurrentState;
}

void Engine::SetFadeState(PEngineState pNewState)
{
  Assert(pNewState.GetPtr());
  if (m_isFading)
  {
    // Can't change state - already fading to a new one.
    return;
  }
  // Set up fade effect. When the effect has finished, change states.
  m_isFading = true;
  m_fade = 1.0f; 

  m_pNewState = pNewState;

  GetTextWriter()->ClearScrollingItems(); // clear last FPS count
}

void Engine::SetState(PEngineState pNewState)
{
  Assert(pNewState.GetPtr());
  // Assert(m_pCurrentState.GetPtr()); - initially 0
  // TODO maybe set to Null state in ctor ?

#if defined(STATE_NAME_DEBUG)
  // Look up state name from ptr.
  std::string stateName;
  for (EngineStateMap::iterator it = m_engineStateMap.begin(); 
       it != m_engineStateMap.end(); ++it)
  {
    if (it->second.GetPtr() == pNewState.GetPtr())
    {
      stateName = it->first;
      break;
    }
  }
  std::cout << "New state: " << stateName.c_str() << "\n";
#endif

  if (m_pCurrentState.GetPtr())
  {
    m_pCurrentState->SetActive(false);  
  }
  m_pCurrentState = pNewState;
  m_pCurrentState->SetActive(true);
  m_pNewState = 0;

  // Disable any active earthquake
  // TODO TEMP TEST
  // POOL:
  // This is preventing Bombs which end the game from making an Earthquake.
//  m_earthquake.Reset();
}

void Engine::FinishedFading()
{
  Assert(m_pNewState.GetPtr());
  Assert(!m_isFading);

  StopSong();

  SetState(m_pNewState);
}

void Engine::SetNextStateName(const std::string stateName)
{
  m_nextStateName = stateName;
}

std::string Engine::GetNextStateName() const
{
  return m_nextStateName;
}

void Engine::StartGame(const string& savedGameName)
{
  GetGameState()->SetName("gs.cfg");
  if (!GetGameState()->Load())
  {
    ReportError("Failed to load game state config.");
  }

  m_levelIsCompleted = false;

  // Set all game object states to UNKNOWN..
  for (GameObjectMap::iterator it = m_objectMap.begin(); it != m_objectMap.end(); ++it)
  {
#ifdef _DEBUG
    //std::cout << "Resetting game object " << it->first << "\n";
#endif
    //GameObjectId id = it->first;
    PGameObject pGo = it->second;
    Assert(pGo.GetPtr());
    pGo->SetState(UNKNOWN);
  }

  // Call NewGame/NewLevel for each engine state.
  for (EngineStateMap::iterator jt = m_engineStateMap.begin(); 
       jt != m_engineStateMap.end(); 
       ++jt)
  {
#ifdef _DEBUG
    std::cout << "Resetting state " << jt->first.c_str() << "\n";
#endif
    jt->second->OnNewGame();
    jt->second->OnNewLevel();
  }

  ClearMessageQueue();

  // Set time of day
  //float startTime = atof(GetConfigValue("start_time_of_day").c_str());
//  m_dayNightSky.SetTimeOfDay(startTime);

  // Clear text if there's an error message from loading the game config file.
  GetTextWriter()->ClearScrollingItems();
}

void Engine::CleanUp()
{
  delete m_pGameState;

  Clear();
  TextureServer::Instance()->Clear();
  //CharacterManager::Instance()->Clear();
  LevelServer::Instance()->Clear();
}

void Engine::SetClearColour(float r, float g, float b)
{
  m_clearR = r;
  m_clearG = g;
  m_clearB = b;
}

TextWriter* Engine::GetTextWriter()
{
  return m_pTextWriter;
}

void Engine::SetTextWriter(TextWriter* pTextWriter)
{
  delete m_pTextWriter;
  m_pTextWriter = pTextWriter;
}

float Engine::GetConfigFloat(const Cfg::Key& key) const
{
  string s = GetConfigValue(key);
  float f = atof(s.c_str());
  return f;
}

string Engine::GetConfigValue(const Cfg::Key& key) const
{
  string s = m_config.GetValue(key);
//#if defined(_DEBUG)
  if (s.empty())
  {
    string error = "Missing value for " + key + " in game config.";
    ReportError(error);
  }
//#endif
  return s;
}

void Engine::SetConfigValue(const char* key, const char* value)
{
  m_config.Set(key, value);
}

GameState* Engine::GetGameState()
{
  return m_pGameState;
}

GameObjectMap& Engine::GetGameObjects()
{
  return m_objectMap;
}

GameObjectMap& Engine::GetGameObjects(int levelId, int roomId)
{
  pair<int, int> p(levelId, roomId); // make a level/room pair
  GameObjectMap& m = m_objectMaps[p]; // get the map for the level/room pair
  return m;
}

PGameObject Engine::GetGameObject(GameObjectId gid)
{
  // Don't add null pointer to map if ID is bad.
  if (m_objectMap.find(gid) == m_objectMap.end())
  {
//    Assert(0);
    /*
    ReportError("Bad ID passed to GetGameObject.");
    */
    return 0;
  }

  return m_objectMap[gid];
}

void Engine::GameObjectChangeRoom(GameObjectId id, int newRoomId)
{
  // Get object for ID.
  PGameObject pGo = GetGameObject(id);
  int levelId = pGo->GetLevel()->GetId();
  int oldRoomId = pGo->GetRoomId();
  // Make sure object has new room ID set.
  pGo->SetRoomId(newRoomId);
  // Remove object from list for old room
  pair<int, int> p(levelId, oldRoomId); // make a level/room pair
  GameObjectMap& m = m_objectMaps[p]; // get the map for the level/room pair
  m.erase(id);

  // Add object to list for new room
  pair<int, int> newp(levelId, newRoomId);
  GameObjectMap& newm = m_objectMaps[newp]; // get the map for the level/room pair
  newm[id] = pGo;
}

void Engine::ClearGameObject(int id)
{
  // TODO make sure ID is valid.

  PGameObject pGo = GetGameObject(id);
  m_objectMap.erase(id);
  GameObjectMap& m = GetGameObjects(pGo->GetLevel()->GetId(), pGo->GetRoomId());
  m.erase(id);
  // TODO maybe notify current state ?
//  m_pCurrentState->ClearObject(id);
}

void Engine::HoldGameObject(int levelId, int roomId, PGameObject pGo)
{
  int objId = pGo->GetId();

  // Game objects are held in two maps, for speed.
  // m_objectMap holds ALL game objects, and is for looking up on object ID.
  // Add the object *pGo to the map.
  // - if it's already in the map, delete the existing object.
  if (m_objectMap.find(objId) != m_objectMap.end())
  {
#if defined(_DEBUG)
std::cout << "Replacing game object ID " << objId << " (" << pGo->GetTypeName()
  << ")\n";
#endif
  }

  m_objectMap[objId] = pGo;

  // The second kind of map holds only the Game objects in one room in one level.
  // So there is one map per room.
  GameObjectMap& m = GetGameObjects(levelId, roomId);
  m[pGo->GetId()] = pGo; // add the game object to the map.

  // Tell the current state about the new object.
  m_pCurrentState->AddObject(pGo);
}

void Engine::ClearGameObjects()
{
  m_objectMap.clear();
  m_objectMaps.clear();
}

void Engine::ClearText()
{
  GetTextWriter()->ClearScrollingItems();
}

void Engine::ReportError(const std::string& errorMessage) const
{
  const_cast<Engine*>(this)->GetTextWriter()->ScrollPrint(errorMessage);
  // In case the TextWriter text is unreadable..
  // NB Deliberately NOT in _DEBUG only build
  cout << errorMessage.c_str() << endl;
}

bool Engine::ReLoad()
{
  // We want to almost do a CleanUp(), but presumably not reset the Game State,
  // otherwise you may as well just restart the application.
  /*
  TextureServer::Instance()->Clear();
  CharacterManager::Instance()->Clear();
  */
  return Load();
}

bool Engine::LoadConfigFile(const std::string& configFileName)
{
  m_configFileName = configFileName;
  if (m_configFileName.empty())
  {
    ReportError("No config filename set.");
    return false;
  }

  // Read the engine config file.
  if (m_config.Load(m_configFileName))
  {
    ReportError("Loaded config file ok.\n");
  }
  else
  {
    ReportError("Failed to load config file, but that's ok.");
    // We don't care, as default values are set by ConfigHack.
    //return false;
  }

  // Add the path separator, i.e. forward or backward slash.
#if defined(WIN32)
  m_config.Set("slash", "\\");
#else
  m_config.Set("slash", "/");
#endif

  // Get the data directory.
  //string dataDir = m_config.GetValue("data_dir");
  // This path will be prepended to everything loaded.
  //File::SetRoot(dataDir, m_config.GetValue("slash"));

  m_maxDeltaTime = atof(GetConfigValue("max_delta_t").c_str());

  m_joystickEnabled = (GetConfigValue("joystick") == "y");
  m_joystickZeroZone = atof(GetConfigValue("joystick_zero_zone").c_str());

  return true;
}

bool Engine::LoadFont()
{
  // Check this open GL driver for max. texture size.
  // We would like to load a 512*512 texture if supported,
  // otherwise we fall back to a 256*256 texture.
  int size = 0;
  //glGetIntegerv(GL_MAX_TEXTURE_SIZE, (GLint*)&size);

#ifdef _DEBUG
  std::string sizeMsg = "Max texture size: ";
  sizeMsg += ToString(size);
  ReportError(sizeMsg);
#endif

  // Get font bitmap name; load bitmap and create font.
  string font = m_config.GetValue("font");
  string fontalpha = m_config.GetValue("font_alpha");
  string widthsFile = m_config.GetValue("font_widths");

  string fontsmall = GetConfigValue("font_small");
  string fontsmallalpha = GetConfigValue("font_alpha_small");
  // Load the small font so it is not removed from the glue file.
  if (!TextureServer::Instance()->Get(fontsmall, fontsmallalpha))
  {
    ReportError("Failed to load small font.");
    return false;
  }

  if (size < 512)
  {
    font = fontsmall;
    fontalpha = fontsmallalpha;
    widthsFile = GetConfigValue("font_widths_small");
  }

  if (!TheFontManager::Instance()->Init()) //GetTextWriter()->Load(font, fontalpha))
  {
    ReportError("Failed to load fonts.");
    return false;
  }

  return true;
}

bool Engine::Load()
{
  // New for v.1.2: progress bar - we have to load in the main
  // thread, so this function gets called many times.
  // Each call we increment the progress bar, and load something
  // different.

  // This counter gets incremented each time we successfully load something.
  // So on the next call we go on to load the next bit of data.
  // If something really fails to load, we set this to -1, which means stop
  // trying to load.
  static int count = 0;
  if (count != -1)
  {
    IncProgress(); // increment the progress bar the user can see.
  }

  switch (count)
  {
  case -1:
    // This means there has been a load error.
    // Don't try to re-load the bad data.
    return false;

  case 0:
    {
      // Get gravity string; convert to float and set value.
      // TODO this should be in level
      string g_str = m_config.GetValue("gravity");
      float g = atof(g_str.c_str());
      FreeMovingGameObject::SetGlobalGravity(g);
      TextFactory::Instance()->Init();
      /*
      if (!TheKeymapPrinter::Instance()->Init())
      {
        ReportError("Failed to load key printer.");
        return false;
      }
      */
      // Help user: show what keys do what.
      bool showkeys = (GetConfigValue("show_keys") == "y");
      SetShowKeys(showkeys);
    }
    ++count;
    return false;

  case 1:
    {
      string charList = m_config.GetValue("characters");
      if (!LoadCharacters(charList))
      {
        count = -1;
        ReportError("Failed to load characters.");
        return false;
      }
    }
    ++count;
    return false;    

  case 2:
/*
    {
      // Load Player character
      // Get the filename where the player description lives.
      string playerFile = GetConfigValue("player_file");
      m_pPlayer = new Player;
      if (!m_pPlayer->Load(playerFile))
      {
        count = -1;
        ReportError("Failed to load player from file " + playerFile);
        return false;
      }
    }
*/
    ++count;
    return false;

  case 3:
    {
      // This is called multiple times until all the states are loaded.
      // When all the states are successfully loaded it returns true.
      // Because this is called multiple times we use LoadResult to decide if
      // there is more to load or there has been a failure.
      // We call this until OK is returned, in which case we go to the next section,
      // or FAIL is returned, in which case the load fails.
      LoadResult r = LoadEngineStates();
      if (r.HasMore())
      {
        // DON'T set count to -1.
        return false;
      }
      if (r.Failed())
      {
        count = -1;
        return false;
      }
    }
    // Successfully loaded all states - go to next section the next time
    // this function is called.
    ++count;
    return false;

  case 4:
    {
      // Initialise shadow textures etc
      if (!Shadow::Init())
      {
        count = -1;
        ReportError("Failed to load shadow.");
        return false;
      }

      if (!BasicShadow::Init())
      {
        count = -1;
        ReportError("Failed to load basic shadow.");
        return false;
      }

      if (!BlockShadow::Init())
      {
        count = -1;
        ReportError("Failed to load block shadow.");
        return false;
      }
    }
    ++count;
    return false;

  case 5:
    {
      Number3d::Init();

      // Sky and lensflare
      if (!m_dayNightSky.Init())
      {
        count = -1;
        return false;
      }

      /*
      if (!m_lensflare.Init())
      {
        return false;
      }
      */
      // Get config values for character control.
      Controller::Init();
    }
    ++count;
    return false;

  case 6:
    {
      // Load anything for special message recipients
      for (SpecialRecipientMap::iterator it = m_specialRecipients.begin();
           it != m_specialRecipients.end();
           ++it)
      {
        if (!it->second->Load())
        {
          count = -1;
          ReportError("Failed to load special recipient.");
          return false;
        }
      }
    }
    ++count;
    return false;

  case 7:
    {
#ifdef _DEBUG
std::cout << "Loading golf courses...\n";
#endif
      if (!PoolCourseManager::Instance()->Load())
      {
        count = -1;
        ReportError("Failed to load golf courses.");
        return false;
      } 
    }
    ++count;
    return false;

  case 8:
    {
      // Load first level, so there is no pause later on.
      // For GOLF v.1.1 there are two levels. Load them both up front so
      // there is no pause later.

      static int courseNum = 0;
    
      std::string levelFile = PoolCourseManager::Instance()->
        GetCourseFileName(courseNum);
      bool isUser = false; //PoolCourseManager::Instance()->IsUser(courseNum);

      // We make multiple calls to this Load function until it returns true,
      // when we then go on to the next course.
      // NB This can return false until the whole course is loaded, which
      // means this will also increment the progress bar.

      static const int NUM_LOOPS_BEFORE_REDRAW = 10;
      for (int i = 0; i < NUM_LOOPS_BEFORE_REDRAW; i++)
      {
        LoadResult r = LevelServer::Instance()->Load(levelFile, isUser, courseNum);

        if (r.IsOk())
        {
          courseNum++;
        
          // Once all the courses are loaded we increment count, and 
          //  so leave this  section.     
          int maxCourse = PoolCourseManager::Instance()->GetNumCourses();
          if (courseNum > (maxCourse - 1)) //  final course number
          {
            ++count;
          }
          return false; 
        }
        else if (r.Failed())
        {
          count = -1;
          return false;
        }
      }
    }
    return false;
  }

  Mouse::SetCursor(Mouse::STANDARD);

  return true;
}

void Engine::OnKey(char key, bool down)
{
  // Pass the key to the current state, which can be a "key configure" state.
  // Or the default behaviour will pass the key to the Engine's KeyMap.
  m_pCurrentState->OnKey(key, down);
}

void Engine::OnSpecialEvent(SpecialEvent* p)
{
  m_pCurrentState->OnSpecialEvent(p);
}

void Engine::Red(bool down)
{
  if (m_isFading) return;
  m_pCurrentState->Red(down);
}

void Engine::Blue(bool down)
{
  if (m_isFading) return;
  m_pCurrentState->Blue(down);
}

void Engine::Green(bool down)
{
  if (m_isFading) return;
  m_pCurrentState->Green(down);
}

void Engine::YellowUp(bool down)
{
  if (m_isFading) return;
  m_pCurrentState->YellowUp(down);
}

void Engine::YellowDown(bool down)
{
  if (m_isFading) return;
  m_pCurrentState->YellowDown(down);
}

void Engine::YellowLeft(bool down)
{
  if (m_isFading) return;
  m_pCurrentState->YellowLeft(down);
}

void Engine::YellowRight(bool down)
{
  if (m_isFading) return;
  m_pCurrentState->YellowRight(down);
}

void Engine::Z(bool down)
{
  if (m_isFading) return;
  m_pCurrentState->Z(down);
}

void Engine::PlusUp(bool down)
{
  if (m_isFading) return;
  static bool prev = !down;
  if (prev == down)
  {
    return;
  }
  prev = down;
  m_pCurrentState->PlusUp(down);
}

void Engine::PlusDown(bool down)
{
  if (m_isFading) return;
  static bool prev = !down;
  if (prev == down)
  {
    return;
  }
  prev = down;
  m_pCurrentState->PlusDown(down);
}

void Engine::PlusLeft(bool down)
{
  if (m_isFading) return;

  if (down == m_prevLeft)
  {
    return;
  }
  m_prevLeft = down;

  m_pCurrentState->PlusLeft(down);
}

void Engine::PlusRight(bool down)
{
  if (m_isFading) return;
  // Disable auto-repeat
  if (down == m_prevRight)
  {
    return;
  }
  m_prevRight = down;

  m_pCurrentState->PlusRight(down);
}

void Engine::JoyX(float f)
{
  if (m_isFading) 
  { 
    return;
  }

  if (fabs(f) < m_joystickZeroZone)
  {
    f = 0;
  }

  m_pCurrentState->JoyX(f);
}

void Engine::JoyY(float f)
{
  if (m_isFading) 
  { 
    return;
  }

  if (fabs(f) < m_joystickZeroZone)
  {
    f = 0;
  }

  m_pCurrentState->JoyY(f);
}

void Engine::MousePos(int x, int y)
{
  Mouse::s_mousex = x;
  Mouse::s_mousey = y;

  m_pCurrentState->MousePos(x, y);
}

void Engine::MouseButton(bool down, bool ctrl, bool shift)
{
  Mouse::s_mouseDown = down;

  // No need for auto-repeat check (TODO right ?)
  m_pCurrentState->MouseButton(down, ctrl, shift);
}

void Engine::MouseRButton(bool down, bool ctrl, bool shift)
{
  Mouse::s_mouseRDown = down;
  m_pCurrentState->MouseRButton(down, ctrl, shift);
}

void Engine::MouseMidButton(bool down, bool ctrl, bool shift)
{
  Mouse::s_mouseMidDown = down;
  m_pCurrentState->MouseMidButton(down, ctrl, shift);
}

bool Engine::IsMouseEnabled() const
{
  return true; // TODO
}

float Engine::GetDeltaTime() const
{
  // Pool online: fixed dt
#ifdef POOL_ONLINE

  static const float DELTA_TIME = (float)(1.0 / 30.0);

  //static const float MAX_DT_INACCURACY = 0.00001f;
//  Assert(fabs(m_deltaTime - DELTA_TIME) < MAX_DT_INACCURACY);

  return DELTA_TIME;
#else
  return m_deltaTime;
#endif
}

void Engine::SetElapsedTime(double secs)
{
  m_oldElapsedSecs = m_elapsedSecs;
  m_elapsedSecs = secs;

  m_deltaTime = (float)m_elapsedSecs - m_oldElapsedSecs;

  if (m_deltaTime > m_maxDeltaTime)
  {
    m_deltaTime = m_maxDeltaTime;
    m_oldElapsedSecs = secs;
  }
  else if (m_deltaTime < 0)
  {
    m_deltaTime = 0;
    m_oldElapsedSecs = secs;
  }
}

bool Engine::PlayWav(const string& file, float volume)
{
  if (m_pSoundPlayer.GetPtr())
  {
    // Prepend the data directory
    string fullpath = File::GetRoot();//GetConfigValue("data_dir");

    //fullpath += GetConfigValue("slash");
    fullpath += file;
    return m_pSoundPlayer->PlayWav(fullpath, volume);
  }
  return false;
}

bool Engine::PlaySong(const string& file)
{
  if (m_pSoundPlayer.GetPtr())
  {
    // Prepend the data directory
    string fullpath = File::GetRoot();
    fullpath += file;

    static string lastplayed;
    bool success = true;
    if (file != lastplayed)
    {
#ifdef _DEBUG
std::cout << "SONG CHANGE: Last song: " << lastplayed 
  << " this: " << file << "\n";
#endif
      StopSong();

      success = m_pSoundPlayer->PlaySong(fullpath);
      lastplayed = file;
    }

    return success;
  }
  return false;
}

void Engine::StopSong()
{
  if (m_pSoundPlayer.GetPtr())
  {
    m_pSoundPlayer->StopSong();
  }
}

void Engine::CallbackSongFinished(const std::string& file)
{
  // Called when MIDI file finishes.
  // Notify current Engine State.
  Assert(m_pCurrentState.GetPtr());
  m_pCurrentState->CallbackSongFinished(file);
}

bool Engine::LoadMinimum()
{
/*
  string charList = m_config.GetValue("characters");
  if (!LoadCharacters(charList))
  {
    return false;
  }
*/
  // TODO load other required stuff
  return true;
}

bool Engine::LoadCharacters(const string& charList)
{
  return CharacterManager::Instance()->LoadCharacters(charList);
}

void Engine::Clear()
{
  for (EngineStateMap::iterator it = m_engineStateMap.begin(); 
       it != m_engineStateMap.end(); 
       ++it)
  {
    PEngineState pState = it->second;
    Assert(pState.GetPtr());
    pState->Clear();
  }

  // Delete all game objects. We don't need to, now we are using smart ptrs!
/*
  for (GameObjectMap::iterator jt = m_objectMap.begin(); jt != m_objectMap.end(); ++jt)
  {
    GameObject* pGo = jt->second;
    delete pGo;
  }
*/
  m_objectMap.clear();
  m_objectMaps.clear(); 
}

void Engine::SetLetterbox(bool b)
{
  UndoLetterbox(); // reset in case arg is zero
  if (b)
  {
    m_letterbox = 2.5f;
  }
  else
  {
    m_letterbox = 0;
  }
}

void Engine::DoLetterbox()
{
  if (m_letterbox == 0)
  {
    return;
  }

  int clipy0 = (int)(m_letterbox * 0.05f * (float)m_viewportY);
  int clipy1 = (int)((1.0f - m_letterbox * 0.05f) * (float)m_viewportY);
  AmjuGL::Viewport(0, clipy0, m_viewportX, clipy1 - clipy0);

  // Adjust aspect ratio so the scene doesn't look squashed.
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::SetIdentity();
  AmjuGL::SetPerspectiveProjection(FIELD_OF_VIEW, LETTERBOX_ASPECT_RATIO, NEAR_PLANE, FAR_PLANE);
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
}

void Engine::UndoLetterbox()
{
  if (m_letterbox == 0)
  {
    return;
  }

  AmjuGL::Viewport(0, 0, m_viewportX, m_viewportY);

  // Restore aspect ratio.
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::SetIdentity();
  AmjuGL::SetPerspectiveProjection(FIELD_OF_VIEW, ASPECT_RATIO, NEAR_PLANE, FAR_PLANE);
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
}

void Engine::SetShowStats(bool b)
{
  if (!b)
  {
    GetTextWriter()->ClearScrollingItems();
  }

  m_showStats = b; 
}

void Engine::Update()
{
  UpdateMessageQueue();

  if (m_pSoundPlayer.GetPtr())
  {
    m_pSoundPlayer->Update();
  }

  m_pCurrentState->Update();

  ForEachSpecialRecipient(&SpecialRecipient::Update);

  if (m_levelIsCompleted)
  {
    // Get rid of ALL pending messages
    m_messageQueue.Clear();
    PlayWav(GetConfigValue("wav_completed"));
    //ChangeState(EngineStateLevelCompleted::Name, IMMEDIATE);
    m_levelIsCompleted = false;
  }
}

void Engine::Draw()
{
  Assert(m_pCurrentState.GetPtr());

  m_numFrames++;

  InitFrame(); // gl commands to begin drawing frame.

  // Do any Fade in/out effect.
  // NB Fade() pushes a colour onto the colour stack.
  Fade();
  // 'Letterbox' effect, for cut scenes.
  DoLetterbox();

  // Draw the scene.
  AmjuGL::PushMatrix();
  m_earthquake.Draw(); // jitters camera if active.
  m_pCurrentState->Draw();
  AmjuGL::PopMatrix();

  ForEachSpecialRecipient(&SpecialRecipient::Draw);

  // Undo Letterbox, for overlays.
  UndoLetterbox();


  AmjuGL::PushMatrix();
  m_pCurrentState->DrawOverlays();
  AmjuGL::PopMatrix();


  ForEachSpecialRecipient(&SpecialRecipient::DrawOverlays);

#if defined(_DEBUG) && defined(SHOW_STATS)
  // Set text colour.
  // glColor3f(1.0f, 1.0f, 1.0f);
  if (m_showStats)
  {
    Fps();
    m_pCurrentState->ShowStats();
  }
#else
  // Show FPS counter even in release build??
  Fps();
#endif

  // Tell TextWriter to write any scrolling text over everything.
  GetTextWriter()->DrawScrollingItems();

  if (m_showKeys && m_pCurrentState->ShowControls())
  {
    TheKeymapPrinter::Instance()->Draw(20, 0.5);
  }

  // TODO currently this asserts if you leave a room (via a portal) when the
  // room is underwater and the destination isn't.
  // (Because the underwater colour is pushed on but not popped off.)
  // Splitting draw/update into separate functions will fix this.
  if (m_colourStack.size() != 1)
  {
    // TODO TEMP TEST
    std::cout << "Colour stack error\n";
  }

  m_colourStack.pop();

  // Absolutely DO NOT CALL glFinish here.
  // We want to do other things while the GPU gets on with the GL instructions
  //  we sent it. We do not want to stall until it has finished!
}

void Engine::LevelCompleted()
{
  //if (m_isFading)
  //{
  //  return; // quick way of stopping this from being called multiple times
  //}

  m_levelIsCompleted = true;

  // Call NewLevel for each engine state.
  for (EngineStateMap::iterator jt = m_engineStateMap.begin(); 
       jt != m_engineStateMap.end(); 
       ++jt)
  {
    jt->second->OnNewLevel();
  }
}

void Engine::OnNewRoom()
{
  for (EngineStateMap::iterator jt = m_engineStateMap.begin(); 
       jt != m_engineStateMap.end(); 
       ++jt)
  {
    jt->second->OnNewRoom();
  }
}

void Engine::Fade()
{
  static float maxVol = 1.0f;
  static bool first = true;
  if (first)
  {
    if (GetSoundPlayer().GetPtr())
    {
      maxVol = GetSoundPlayer()->GetSongMaxVolume();
    }
    first = false;
  }
  // TODO doesn't work for volume adjust.

  static bool musicIsSilent = false;

  if (m_isFading)
  {
    if (m_fade == 1.0f)
    {
      if (GetSoundPlayer().GetPtr() && GetSoundPlayer()->GetSongMaxVolume() == 0)
      {
         musicIsSilent = true;
      }
      else
      {
         musicIsSilent = false;
      }
    }

    // Fade value is a function of time..
    m_fade -= GetDeltaTime() * 0.5f; // fades to black in 2s. TODO CONFIG!

    PushColour(m_fade * m_globalR, m_fade * m_globalG, m_fade * m_globalB, 1);

    if (!musicIsSilent && GetSoundPlayer().GetPtr())
    {
      GetSoundPlayer()->SetSongMaxVolume(m_fade * maxVol);
    }

    if (m_fade <= SMALLEST)
    {
      if (!musicIsSilent && GetSoundPlayer().GetPtr())
      {
         GetSoundPlayer()->SetSongMaxVolume(maxVol);
      }
      m_isFading = false;
      FinishedFading();
    }
  }
  else
  {
    // Not fading.
    PushColour(m_globalR, m_globalG, m_globalB, 1);
  }
}

void Engine::PushColour(float r, float g, float b, float a)
{
  // Get the current top - white if stack is empty.
  Colour topC(1, 1, 1, 1);
  if (!m_colourStack.empty())
  {
    topC = m_colourStack.top();
  }
  // Modulate new colour with top colour
  Colour newTop(topC.m_r * r, topC.m_g * g, topC.m_b * b, topC.m_a * a);
  // Set the new colour and push it.
  AmjuGL::SetColour(Colour(newTop.m_r, newTop.m_g, newTop.m_b, newTop.m_a));
  m_colourStack.push(newTop);
}

void Engine::PushAbsColour(float r, float g, float b, float a)
{
  Colour c(r, g, b, a);
  AmjuGL::SetColour(c); //glColor4f(r, g, b, a);
  m_colourStack.push(c);
}

void Engine::PopColour()
{
  // Pop the top colour off. Set the colour to the new top, or white if empty.
  Assert(!m_colourStack.empty());
  m_colourStack.pop();
  if (m_colourStack.empty())
  {
    //glColor4f(1, 1, 1, 1);
    AmjuGL::SetColour(Colour(1, 1, 1, 1));
  }
  else
  {
    Colour newTop = m_colourStack.top();
    //glColor4f(newTop.m_r, newTop.m_g, newTop.m_b, newTop.m_a);
    AmjuGL::SetColour(Colour(newTop.m_r, newTop.m_g, newTop.m_b, newTop.m_a));
  }
}

void Engine::InitFrame()
{
  AmjuGL::SetClearColour(Colour(m_clearR, m_clearG, m_clearB, 1.0f));
  AmjuGL::InitFrame();

  // Should be unnecessary, but may fix letterbox issue found on Intel iMac, 4/5/2006
  AmjuGL::Viewport(0, 0, m_viewportX, m_viewportY);

  // Do GL initialisation before we draw the frame.
  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_READ);
  AmjuGL::Enable(AmjuGL::AMJU_BLEND);

  // Set the modelview matrix
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
  AmjuGL::SetIdentity();
}

void Engine::InitGl()
{
  // Set the projection matrix
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::SetIdentity();

  // We fix the aspect ratio here. If the viewport dimensions don't match this
  // ratio, the display is stretched or squashed. But things are always in the
  // right place on the screen.

  AmjuGL::SetPerspectiveProjection(FIELD_OF_VIEW, ASPECT_RATIO, NEAR_PLANE, FAR_PLANE);
  
  // Set the modelview matrix
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
  AmjuGL::SetIdentity();
  
  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_READ);

  // Disable lighting for flat, hopefully cartoony look.
  if (GetConfigValue("lighting") == "y")
  {
    AmjuGL::Enable(AmjuGL::AMJU_LIGHTING); 
  }
  else
  {
    AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);
  }
}

void Engine::Fps()
{
  static int intSec = 0;
  static int oldIntSec = 0;
  static int frames = 0;
  static int fps = 0;

  oldIntSec = intSec;
  intSec = (int)m_elapsedSecs;
  if (intSec == oldIntSec)
  {
    frames++;
  }
  else
  {
#if defined(_DEBUG) && defined (SHOW_FPS_IN_CONSOLE)
    cout << "FPS: " << frames << "\n";
#endif
    fps = frames;
    frames = 0;
  }

  // Hack, clear any old stats.
  m_pCurrentState->ClearScrollText();

  char buf[10];
  sprintf(buf, "%d", fps);
  // Bottom left corner ?
  GetTextWriter()->Print(0.0, 15.0f, buf);
}

#if defined(_DEBUG)
bool Engine::DoDrawSceneSpheres()
{
  bool drawspheres = (GetConfigValue("scenespheres") == "y");
  return drawspheres;
}

bool Engine::DoDrawPlatformSpheres()
{
  bool drawspheres = (GetConfigValue("platformspheres") == "y");
  return drawspheres;
}

bool Engine::DoDrawCharacterSpheres()
{
  bool drawspheres = (GetConfigValue("charspheres") == "y");
  return drawspheres;
}
#endif // _DEBUG

void Engine::SendGameMessage(PMessage m)
{
#if !defined(_DEBUG)
  if (m->NoDelay())
  {
    // Don't queue this message, just process it.
    ProcessMessage(m);
    return;
  }
#endif

  m->SetRealTime(GetElapsedTime()); // adds any delay
  m_messageQueue.Push(m);
}

void Engine::Freeze(bool b)
{
  m_frozen = b;
  if (m_frozen)
  {
    m_frozenTime = GetElapsedTime();
  }
  else
  {
    SetElapsedTime(m_frozenTime);
  }
}

void Engine::ClearMessages(int recipientId)
{
  m_messageQueue.Clear(recipientId);
}

void Engine::UpdateMessageQueue()
{
  if (m_frozen)
  {
    return;
  }  

  while (!m_messageQueue.Empty() &&
         m_messageQueue.Top()->GetRealTime() < GetElapsedTime())
  {
    // It's time for the top message to be sent.
    PMessage pm = m_messageQueue.TopAndPop();
    // Send the message to the local game object..
    ProcessMessage(pm);

    // ..and send the message to the Server too.
    //m_pNetworkSerializer->WriteMessage(pm);
  }
}

void Engine::ProcessMessage(PMessage m)
{
  // If the recipient ID is one of the special built-in recipients,
  // give that recipient the message.
  // E.g. CutScenePlayer, SoundPlayer, etc.
  SpecialRecipientMap::iterator it = m_specialRecipients.find(m->GetRecipientId());
  if (it != m_specialRecipients.end())
  {
    PSpecialRecipient pRecip = it->second;
    pRecip->ReceiveMessage(*(m.GetPtr()));

    return;
  }

  // If the recipient is a player (for multiplayer games),
  // send the message to the player.
  // TODO use a map of IDs to players.
  if (m->GetRecipientId() == 0) // Local Player ID
  {
//    m_pPlayer->ReceiveMessage(*(m.GetPtr()));
#if defined(_DEBUG)
/*
    cout << "Player message: " << *(m.GetPtr()) << endl;
*/
#endif
    return;
  } 

  m_pCurrentState->ReceiveMessage(*(m.GetPtr()));
}

void Engine::ClearMessageQueue()
{
  m_messageQueue.Clear();
}

void Engine::SetViewport(int x, int y)
{
  m_viewportX = x;
  m_viewportY = y;
  if (m_viewportY == 0)
  {
    m_viewportY = 1;
  }

  AmjuGL::Viewport(0, 0, m_viewportX, m_viewportY);
}

void Engine::GameOver()
{
  m_pCurrentState->GameOver();

  // Set state of all Game Objects to some inactive state - the idea being we don't
  // want NPCs etc to behave stupidly during the death sequence.
  for (GameObjectMap::iterator it = m_objectMap.begin(); it != m_objectMap.end(); ++it)
  {
    PGameObject pGo = it->second;
    Assert(pGo.GetPtr());
    if (pGo.GetPtr())
    {
      pGo->SetState(UNKNOWN); // OUT_OF_PLAY);
    }
  }
  
}

void Engine::ForEachSpecialRecipient(void (SpecialRecipient::*pf)())
{
  for (SpecialRecipientMap::iterator it = m_specialRecipients.begin();
       it != m_specialRecipients.end();
       ++it)
  {
    SpecialRecipient* pSr = it->second.GetPtr();
    (pSr->*pf)();
  }
}

bool Engine::NetworkInit()
{
/*
  // TODO move to point where server name and port are configurable.
  m_pClientSocket = new ClientSocket;
  m_pNetworkSerializer = new MessageSerializer(m_pClientSocket.GetPtr());

  std::string servername = GetConfigValue("servername");
  int serverport = atoi(GetConfigValue("serverport").c_str());

  bool b = m_pClientSocket->Connect(servername, serverport);

  if (b)
  {
    m_pClientReceiveThread = new ClientReceiveThread;
    m_pClientReceiveThread->SetSocket(m_pClientSocket.GetPtr());
    m_pClientReceiveThread->Start();
  }

  return b;
*/
  return false;
}

void Engine::NetworkReceiveMessage(PMessage pm)
{
#if defined(_DEBUG)
  cout << "Got message from server: " << *(pm.GetPtr()) << endl;
#endif
  SendGameMessage(pm);
}

void Engine::SetEarthquake(float activeSeconds, float severity)
{
  m_earthquake.SetActive(activeSeconds, severity);
}

} // Schmicken

