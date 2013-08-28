#include <AmjuFirst.h>
#include <Game.h>
#include <ResourceManager.h>
#include <File.h>
#include <Font.h>
#include <SoundManager.h>
#include <FileImplGlue.h>
#include <GlueFileMem.h>
#include <Localise.h>
#include <ReportError.h>
#include <Screen.h>
#include <Directory.h>
#ifdef GEKKO
#include <CursorManager.h>
#endif
#include "PoolGameState.h"
#include "Engine.h"
#include "EsLogo.h"
#include "LoadButton.h"

#ifdef AMJU_USE_BASS
#include <BassSoundPlayer.h>
#endif

#include <iostream>
#include <AmjuGLWindowInfo.h>
#include <AmjuFinal.h>

#ifdef MACOSX
#define GLUE_FILE "data-Mac.glue"
#define MUSIC_GLUE_FILE "music-Mac.glue"
const char* layoutfilename = "layout_default.txt";
#endif

#ifdef WIN32
#define GLUE_FILE "data-win.glue"
#define MUSIC_GLUE_FILE "music-win.glue"
const char* layoutfilename = "layout_default.txt";
#endif

#ifdef GEKKO
#define GLUE_FILE "data-wii.glue"
#define MUSIC_GLUE_FILE "music-wii.glue"
const char* layoutfilename = "layout_wii.txt";
#endif

#ifdef AMJU_IOS
#define GLUE_FILE "data-iphone.glue"
#define MUSIC_GLUE_FILE "music-iphone.glue"
#endif

namespace Amju
{
#ifdef AMJU_IOS
// But TODO what about different res devices
static const int WIDTH = 480; 
static const int HEIGHT = 320;

#else
static const int WIDTH = 640; 
static const int HEIGHT = 480;
#endif

static const char* GAME_CFG = "game.cfg";

void ConfigHack();

AmjuGLWindowInfo w(WIDTH, HEIGHT, false, "Amju Super Cool Pool");

const char * APPLICATION_NAME = "Amju Super Cool Pool";

void StartUpBeforeCreateWindow()
{
#if defined(IPHONE)
  std::string dataDir = GetDataDir();
  std::cout << "Data Dir: " << dataDir << "\n";
  File::SetRoot(dataDir, "/");
#endif

#ifdef GEKKO
  // TODO Better to put this in library main() if we can get the app's directory
  File::SetRoot("/apps/amju_scp/data/", "/");
#endif

#if defined (AMJU_USE_BASS)
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

  Engine* engine = Engine::Instance();
  engine->ChangeState(EsLogo::Name, Engine::IMMEDIATE);

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

  TheGame::Instance()->SetCurrentState(engine);	

}

void StartUpAfterCreateWindow()
{
  Texture::SetDefaultFilter(AmjuGL::AMJU_TEXTURE_NICE);
  Texture::SetDefaultWrapMode(AmjuGL::AMJU_TEXTURE_WRAP);

  Engine* engine = Engine::Instance();

  // This is no good, screen size is not known yet!!!
  // But the idea is that we get the device resolution so we can work on iphone, ipad, etc. etc.

#ifndef AMJU_IOS
  Screen::SetSize(WIDTH, HEIGHT);
  engine->SetViewport(Screen::X(), Screen::Y());
#endif

#ifdef IPHONE
  const char* layoutfilename = "layout_iphone.txt";
  // TODO Find a better way to work out the physical screen size
  if (Screen::Y() > 640)
  {
    layoutfilename = "layout_ipad.txt";
  }
#endif
  if (!LoadButtonLayout(layoutfilename))
  {
    ReportError(std::string("Failed to load button layout file ") + layoutfilename);
  }

  if (!engine->LoadFont())
  {
    ReportError("Failed to load fonts");
  }
}
}

