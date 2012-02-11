#include <AmjuFirst.h>
#include <Game.h>
#include <ResourceManager.h>
#include "Engine.h"
#include "EsLogo.h"
#include <File.h>
#include <Font.h>
#include <SoundManager.h>
#include <FileImplGlue.h>
#include <GlueFileMem.h>
#include <Localise.h>
#include <ReportError.h>
#include <Screen.h>
#ifdef GEKKO
#include <CursorManager.h>
#endif
#include "PoolGameState.h"

#ifdef MACOSX
#include <BassSoundPlayer.h>
#endif

#include <iostream>
#include <AmjuGLWindowInfo.h>
#include <AmjuFinal.h>

#ifdef MACOSX
#define GLUE_FILE "data-Mac.glue"
#define MUSIC_GLUE_FILE "music-Mac.glue"
#endif

#ifdef WIN32
#define GLUE_FILE "data-win.glue"
#define MUSIC_GLUE_FILE "music-win.glue"
#endif

#ifdef GEKKO
#define GLUE_FILE "data-wii.glue"
#define MUSIC_GLUE_FILE "music-wii.glue"
#endif

#ifdef IPHONE
#define GLUE_FILE "data-iphone.glue"
#define MUSIC_GLUE_FILE "music-iphone.glue"
#endif

namespace Amju
{
#ifdef IPHONE
// But TODO what about different res devices
static const int WIDTH = 480; 
static const int HEIGHT = 320;

#else
static const int WIDTH = 640; 
static const int HEIGHT = 480;
#endif

static const char* GAME_CFG = "game.cfg";

void ConfigHack();

AmjuGLWindowInfo w(WIDTH, HEIGHT, false);

const char * APPLICATION_NAME = "Amju Super Cool Pool";

void StartUp()
{
#if defined(IPHONE) && defined(AMJU_CONSOLE)
  File::SetRoot("/Applications/Amju_SCP.app/", "/");
#endif

#ifdef GEKKO
  // TODO Better to put this in library main() if we can get the app's directory
  File::SetRoot("/apps/amju_scp/data/", "/");
#endif

#if defined (MACOSX)
  TheSoundManager::Instance()->SetImpl(new BassSoundPlayer);
#endif

  GlueFile* gf = new GlueFileMem;
  gf->SetPrintUnusedInDtor(true);

  if (!FileImplGlue::OpenGlueFile(GLUE_FILE, gf))
  {
    ReportError("Failed to open data glue file");
    return; // TODO false;
  }

  GlueFile* pMusicGlueFile = new GlueFileMem;
  if (pMusicGlueFile->OpenGlueFile(MUSIC_GLUE_FILE, true)) // true = read only 
  {
    TheSoundManager::Instance()->SetGlueFile(pMusicGlueFile);
  }
  else
  {
    ReportError("Failed to open music glue file");
    return; // TODO false;
  }

/*
  // TODO Other languages - preferences
  if (!Localise::LoadStringTable("english.txt"))
  {
    ReportError("No localise string table.");
  }

  // Add resource loaders
  TheResourceManager::Instance()->AddLoader("obj", ObjLoader);
  TheResourceManager::Instance()->AddLoader("font", FontLoader);
  TheResourceManager::Instance()->AddLoader("mod", BinaryResourceLoader);
  TheResourceManager::Instance()->AddLoader("snd", BinaryResourceLoader);
  TheResourceManager::Instance()->AddLoader("wav", BinaryResourceLoader);
	
  // Add SceneNode types to factory
  TheSceneNodeFactory::Instance()->Add(SceneNode::NAME, &SceneNode::Create);
  TheSceneNodeFactory::Instance()->Add(SceneMesh::NAME, &SceneMesh::Create);
	
	
  TheResourceManager::Instance()->LoadResourceGroup("2dtext-group");
  TheHud::Instance()->Load();

#ifdef BYPASS_TITLE
  // TODO Only needed if we bypass title
  TheResourceManager::Instance()->LoadResourceGroup("3dtext-group");
  TheResourceManager::Instance()->LoadResourceGroup("gui-group");
  TheLevelManager::Instance()->SetLevelId(1);
  StartGame(1, AMJU_MAIN_GAME_MODE); // TODO two player etc
  TheGame::Instance()->SetCurrentState(GSLoadLevel::NAME); 
#else
  TheGame::Instance()->SetCurrentState(TheGSLogo::Instance());	
#endif
*/

#ifdef GEKKO
  TheCursorManager::Instance()->Load(Vec2f(0.025f, -0.08f)); // position hotspot
#endif

  Engine::Instance()->ChangeState(EsLogo::Name, Engine::IMMEDIATE);

  Engine* engine = Engine::Instance();
  engine->GetGameState()->SetName("gs.cfg");
  engine->GetGameState()->Load();

  ConfigHack(); // set config from auto-generated source file.

#ifdef MACOSX
  std::string root = "./"; //GetRoot();
  std::cout << "Root is " << root.c_str() << "\n";
  File::SetRoot(root, "/");
#endif

#ifdef _DEBUG
std::cout << "main(): got root\n";
#endif

  engine->LoadConfigFile(GAME_CFG);
  engine->InitGl();

  // This doesn't work, screen size is not known yet!!!
  // But the idea is that we get the device resolution so we can work on iphone, ipad, etc. etc.
  //engine->SetViewport(Screen::X(), Screen::Y()); 
  engine->SetViewport(WIDTH, HEIGHT);

  if (!engine->LoadFont())
  {
    std::cout << "Couldn't load font.\n";
  }

  TheGame::Instance()->SetCurrentState(engine);	

}
}

