/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: MacGlutMain.cpp,v $
Revision 1.1.2.11  2007/12/14 11:10:40  jay
Update version

Revision 1.1.2.10  2007/07/16 08:21:24  jay
Version number in title bar

Revision 1.1.2.9  2007/03/27 20:59:04  jay
Never erase all player data

Revision 1.1.2.8  2007/03/21 10:26:35  jay
Update version number from 1.4 to 1.5

Revision 1.1.2.7  2007/03/09 21:49:29  jay
Debug output

Revision 1.1.2.6  2006/08/27 19:21:38  jay
Online: server URL is configurable in gs.cfg

Revision 1.1.2.5  2006/08/26 21:26:50  jay
Added #ifdef POOL_ONLINE

Revision 1.1.2.4  2006/08/07 20:44:11  jay
Use fixed-interval timer, local server for now

Revision 1.1.2.3  2006/07/26 21:25:27  jay
Use Pool/, not Golf/

Revision 1.1.2.2  2006/07/25 08:06:58  jay
Don't use MACOSX_BUNDLE as this is for debug use only.
Do use GLUT mouse functions, as this is for debug use only (these functions
 don't work properly with Mighty Mice).

Revision 1.1.2.1  2006/04/23 21:18:05  jay
Native Carbon application wrapper, not GLUT
First check in so this is backed up, not properly working yet.

Revision 1.3.2.1.2.18  2006/04/11 17:08:39  jay
Update version number in Title bar

Revision 1.3.2.1.2.17  2006/03/24 10:21:29  jay
Added more debug output for Mouse input

Revision 1.3.2.1.2.16  2006/03/23 08:30:27  jay
Added DEBUG diagnostics for intel testing

Revision 1.3.2.1.2.15  2006/03/19 23:01:19  jay
Must set Glue file for SoundPlayer; fix Bundle bug in prev revision

Revision 1.3.2.1.2.14  2006/03/09 22:50:45  jay
Swap FMOD for BASS

Revision 1.3.2.1.2.13  2006/03/08 19:08:36  jay
Initial state is now EsLogo

Revision 1.3.2.1.2.12  2005/11/08 10:31:58  jay
Register a handler so we can handle paste commands. When we get one we
create a Paste special event and hand it to the Engine.

Revision 1.3.2.1.2.11  2005/10/28 12:52:55  jay
New version no, 1.3

Revision 1.3.2.1.2.10  2005/10/28 12:45:08  jay
Use info returned from glutGameGet functions. Hopefully this will make
full screen mode more robust.
Currently initialises PoolOnline manager - we may need to remove this for
the next release if Online games are not ready.

Revision 1.3.2.1.2.9  2005/10/21 17:48:33  jay
Enable full-screen; check for first time and go to Test screen.

Revision 1.3.2.1.2.8  2005/09/05 20:09:31  jay
Turn off debug output

Revision 1.3.2.1.2.7  2005/08/26 22:08:08  jay
Update version no., trash player data if no reg file

Revision 1.3.2.1.2.6  2005/08/02 18:36:50  jay
Updated app name

Revision 1.3.2.1.2.5  2005/07/17 23:07:15  jay
Handle right and middle mouse buttons

Revision 1.3.2.1.2.4  2005/07/11 07:33:18  jay
Disable fullscreen

Revision 1.3.2.1.2.3  2005/07/09 06:44:37  jay
Remove dependency on REG_VERSION

Revision 1.3.2.1.2.2  2005/05/09 05:56:58  jay
New application name

Revision 1.3.2.1.2.1  2005/04/21 17:38:45  jay
Minor fixes

Revision 1.3.2.1  2004/11/09 16:02:49  jay
Merged XING full screen functions to here - removed "always registered"
Xing code!

Revision 1.3.4.3  2004/11/09 13:40:39  jay
Updated version number in application name

Revision 1.3.4.2  2004/11/08 13:57:01  jay
Added fullscreen functions

Revision 1.3.4.1  2004/10/14 11:38:59  jay
Xing branch only: set registered on startup

Revision 1.3  2004/09/17 14:06:15  jay
Updated application version

Revision 1.2  2004/09/09 11:12:47  jay
Look for local Data subdirectory, not a fixed location

Revision 1.1  2004/09/08 15:43:08  jay
Added to repository
  
*/

#if defined(MACOSX)
#include <GLUT/Glut.h>
#else
#include <GL/glut.h>
#endif

#if defined(MACOSX)

// GLUT is now only used for non-release builds, so no bundle.
#ifdef MACOSX_BUNDLE
#undef MACOSX_BUNDLE
#endif

#include <CoreFoundation/CFString.h>
#include <CoreFoundation/CFBundle.h>
#include <Carbon/Carbon.h>
#endif

#include "GameEngine.h"
#include "SocketService.h"
#include "FileImplGlue.h"
#include "GlueFileMem.h" 
#include "StringUtils.h"
//#include "Registration.h"
//#include "SpeechSynth.h"
#include "GameState.h"
#include "PlayerNames.h"
#include "PoolOnline.h"
#include "SpecialEventPaste.h"
//#include "BassSoundPlayer.h"
#include "EsLogo.h"

#if defined(WIN32)
#include "../Win32/WinSoundPlayer.h" 
#include "../Win32/WinTimer.h" 
#include "../Win32/WinTimerFixed.h" 
#include "../Win32/WinJoystick.h" 
#endif

#if defined(MACOSX)
#include "MacTimer.h"
#endif

//#include "BassSoundPlayer.h"
#include <AmjuGL-OpenGL.h>
#include <AmjuGLWindowInfo.h>

#include <iostream>

// Turn on debug output for mouse control.
//#define MOUSE_DEBUG

#if defined(WIN32)
// Windows: we want to link to winsock library
#pragma comment(lib, "ws2_32.lib")
#endif // WIN32

#define WANT_FLICKER

using namespace Amju;
using namespace std;

Engine* engine;

#if defined(WIN32)
#if defined(WANT_FLICKER)
WinTimer theTimer;
#else
WinTimerFixed theTimer;
#endif
WinJoystick joystick;
const char* GAME_CFG = "game.cfg";
#endif

#if defined(MACOSX)
MacTimer theTimer;
//MacJoystick joystick;
// TODO TEMP for debugging
const char* GAME_CFG = "./game.cfg";
#endif

const char* GAME_TITLE = "Amju Super Cool Pool 2.2";
// Initial window size
const int GAME_INIT_X = 600;
const int GAME_INIT_Y = 400;
// Joystick poll interval, ms.
const int GAME_JOY_POLL_INTERVAL = 50;

namespace Amju
{
const char * APPLICATION_NAME = "Amju Super Cool Pool";

AmjuGLWindowInfo w(640, 480, false);

// TODO Make this a GLUT version in AmjuGLOpenGL
bool AmjuGLOpenGL::CreateWindow(AmjuGLWindowInfo*)
{
  return true;
}
}

// Handle Mouse Events
// Copied from Apple sample 
static OSStatus handleWindowMouseEvents (EventHandlerCallRef myHandler, EventRef event)
{
  OSStatus result = eventNotHandledErr; 
  UInt32 kind = GetEventKind(event);
  EventMouseButton button = 0;
  HIPoint location = {0.0f, 0.0f};
  UInt32 modifiers = 0;

  result = CallNextEventHandler(myHandler, event);
  if (eventNotHandledErr == result)
  { // only handle events not already handled (prevents wierd resize interaction)
    switch (kind) 
    {
    case kEventMouseDown:
std::cout << "Mouse handler: got mouse down event!\n";
      GetEventParameter(event, kEventParamMouseButton, typeMouseButton, 
        NULL, sizeof(EventMouseButton), NULL, &button);
      GetEventParameter(event, kEventParamMouseLocation, typeHIPoint, 
        NULL, sizeof(HIPoint), NULL, &location);
      GetEventParameter(event, kEventParamKeyModifiers, typeUInt32, 
        NULL, sizeof(UInt32), NULL, &modifiers);
      if ((button == kEventMouseButtonSecondary) || 
          ((button == kEventMouseButtonPrimary) && (modifiers & controlKey))) 
      { 
        // RIGHT MOUSE BUTTON DOWN
        //gDollyPanStartPoint[0] = location.x;
        //gDollyPanStartPoint[1] = location.y;
        //gTrackingContextInfo = pContextInfo->aglContext;
      } 
      else if ((button == kEventMouseButtonTertiary) || 
        ((button == kEventMouseButtonPrimary) && (modifiers & optionKey))) 
      { 
        // MIDDLE MOUSE BUTTON DOWN
      } 
      else if (button == kEventMouseButtonPrimary)  
      { 
        // LEFT MOUSE BUTTON DOWN
      }
      break;

    case kEventMouseUp:
std::cout << "Mouse handler: got mouse up event!\n";
      // MOUSE BUTTON UP: Check to see which one
      break;
  
    case kEventMouseDragged:
std::cout << "Mouse handler: got mouse drag event!\n";
      // MOUSE MOVED: Get position
      GetEventParameter(event, kEventParamMouseLocation, typeHIPoint, 
        NULL, sizeof(HIPoint), NULL, &location); 
      break;

    }
    result = noErr;
  }      
  return result;
}

// Mac OS X - specific: register an event handler so we can react
// when user clicks paste, or uses option-V.
// This relies on the NIB file matching the unique ID for the command.
//
// This callback function gets called whenever the Application gets a
// Command event.
pascal OSStatus AmjuApplicationEventHandler(
  EventHandlerCallRef  nextHandler,
  EventRef             event,
  void*                userData)
{
std::cout << "AmjuEventHandler: getting an event of some kind!\n";

  OSStatus result = eventNotHandledErr;
  UInt32 whatHappened = GetEventKind(event);
  UInt32 eventClass = GetEventClass(event);
  HICommand commandStruct;

  if (eventClass == kEventClassMouse)
  {
std::cout << "AmjuEventHandler: getting a mouse event!\n";
    return handleWindowMouseEvents(nextHandler, event);
  }

  if (whatHappened == kEventCommandProcess)
  {
    GetEventParameter(event, kEventParamDirectObject,
      typeHICommand, 0, sizeof(HICommand), 0, &commandStruct);

    if (commandStruct.commandID == 'mypa') // Command ID, defined in NIB file 
    {
      // Get text from clipboard, pass SpecialEvent to Engine
      result = noErr; 
/* TODO 
      // Get text data (if any) from the clipboard ("scrap").
      ScrapRef scrap = 0;
      GetCurrentScrap(&scrap);
      // NB We don't need to get the size, as we give the buf size to the next fn.
      //GetScrapFlavorSize(scrap, kScrapFlavorTypeText, &scrapSize);
      static int BUF_SIZE = 100; // big enough for names and serial nos.
      Size scrapSize = BUF_SIZE - 1;
      char buf[BUF_SIZE];
      OSStatus s = GetScrapFlavorData(scrap, kScrapFlavorTypeText, &scrapSize, buf);
      if (s == noErr)
      {
        buf[scrapSize] = 0; // terminate string

        // TODO
        // NB Currently SpecialEvents shouldn't be on the heap, as they will
        // not be deleted automatically.
        // If we change to use SharedPtr<SpecialEvent>, this won't work, 
        // as SpecialEvents will then _have_ to go on the heap.
        SpecialEventPaste sep(buf);
        engine->OnSpecialEvent(&sep);
      }
      else
      {
#ifdef _DEBUG
std::cout << "PASTE: got a paste command, but the clipboard data is no good.\n";
#endif
      }
*/
    }
  }
  return result;
}

// This function registers the above handler
void RegisterPasteHandler()
{
#ifdef _DEBUG
std::cout << "Entering RegisterPasteHandler()\n";
#endif

  // The event target is the Application; the type is a Command. 
  EventTypeSpec typeList[] = 
  { 
    { kEventClassMouse, kEventMouseDown },
    { kEventClassMouse, kEventMouseUp },
    { kEventClassMouse, kEventMouseDragged },
    { kEventClassKeyboard, kEventRawKeyDown },
    {kEventClassCommand, kEventCommandProcess}
  };

  EventTargetRef target = GetApplicationEventTarget();
  void* userData = 0;
  EventHandlerRef* handlerRef = 0;

std::cout << "RegisterHandlers: num types: " << GetEventTypeCount(typeList) << "\n";
  /* OSStatus s = */  
  InstallEventHandler(
    target,
    NewEventHandlerUPP(&AmjuApplicationEventHandler),
    GetEventTypeCount(typeList),
    typeList,
    userData,
    handlerRef);

#ifdef _DEBUG
std::cout << "Leaving RegisterPasteHandler()\n";
#endif
}

bool isFullScreen;
void GoFullScreen()
{
  if (isFullScreen)
  {
    return;
  }
  engine->GetGameState()->Set("fullscreen", "y");
  isFullScreen = true;
}

void GoWindowed()
{
  if (!isFullScreen)
  {
    return;
  }
  engine->GetGameState()->Set("fullscreen", "n");
  isFullScreen = false;
}

bool IsFullScreen()
{
  return isFullScreen;
}

std::string GetRoot()
{
#ifdef _DEBUG
std::cout << "Entering GetRoot()\n";
#endif

  std::string root;
  std::string glueFileName;

#if defined(MACOSX_BUNDLE)
  // Get the main bundle for the app
  CFBundleRef mainBundle = CFBundleGetMainBundle();

#ifdef _DEBUG
std::cout << "  GetRoot: got main bundle: " << (void*)mainBundle << "\n";
#endif

  // Find data.glue in the bundle
  // TODO we can get the required file name from the Config.
  CFURLRef gluefileUrl = CFBundleCopyResourceURL(mainBundle,
	CFSTR("data"), CFSTR("glue"), 0);

#ifdef _DEBUG
std::cout << "  GetRoot: got glue file url: " << (void*)gluefileUrl << "\n";
#endif

  /* one of 
    kCFURLPOSIXPathStyle = ,
    kCFURLHFSPathStyle = 1,
    kCFURLWindowsPathStyle = 2
  */
  CFURLPathStyle pathStyle = kCFURLPOSIXPathStyle;
  // Convert URL to path
  CFStringRef macGluefilePath = CFURLCopyFileSystemPath (gluefileUrl, pathStyle);

#ifdef _DEBUG
std::cout << "GetRoot(): got macGluefilePath\n";
#endif

  // Convert (unicode ?) CFStringRef to a const char *
  const char*  bytes = CFStringGetCStringPtr(macGluefilePath, CFStringGetSystemEncoding()); 
  // NB The above may return 0 if we request an encoding which would result 
  // in copying.
  if (bytes)
  {
#ifdef _DEBUG
std::cout << "  GetRoot: got glue file name: " << bytes << "\n";
#endif
    glueFileName = bytes;
  }
  else
  {
    // We must copy the string into a buffer
    char buf[200];
    // TODO
#ifdef _DEBUG
std::cout << "  GetRoot: Unexpected: didn't get glue file name: " << bytes << "\n";
#endif
  }
  root = Amju::GetFilePath(glueFileName);
#else
#if defined(WIN32)
  root = "./Data/";  
#else
  root = "./"; //./../Data";
#endif
#endif

#ifdef _DEBUG
std::cout << "Leaving  GetRoot(), result: " << root.c_str() << "\n";
#endif

  return root;
}


namespace Amju
{
// Auto-generated file created from config.
void ConfigHack();

// Global preferences config file.
ConfigFile* g_pPrefs = 0;
}

void draw()
{
  engine->SetElapsedTime(theTimer.GetElapsedTime());
  engine->Draw();
  engine->Update();
  glutSwapBuffers();
}

void idle() 
{
  // Mac: first time around, set up the menu bar. This is done by GLUT but we don't want
  // the glut menu.
  static bool first = true;
  if (first)
  {
    first = false;
#ifdef _DEBUG
std::cout << "First time in idle(): load NIB, register handlers\n";
#endif

#if defined(MACOSX_BUNDLE)
    IBNibRef 		nibRef;
    WindowRef 		window;
    
    OSStatus		err;

    // Create a Nib reference passing the name of the nib file (without the .nib extension)
    // CreateNibReference only searches into the application bundle.
    err = CreateNibReference(CFSTR("mynib"), &nibRef);
//    require_noerr( err, CantGetNibRef );
    
    // Once the nib reference is created, set the menu bar. "MainMenu" is the name of the menu bar
    // object. This name is set in InterfaceBuilder when the nib is created.
    err = SetMenuBarFromNib(nibRef, CFSTR("MenuBar"));
//    require_noerr( err, CantSetMenuBar );
   
    // TODO TEMP TEST: try to get my cursor resource
    /*
    CursHandle ch = GetCursor(128); // the ID ResEdit seems to have allocated
    if (ch)
    {
      SetCursor(*ch);
    }
    else
    {
      cout << "no cursor.\n";
    }
    */
#endif

    // Register handler for events
    RegisterPasteHandler();
  }

  glutPostRedisplay();
}

void resize(int w, int h)
{
  engine->SetViewport(w, h);
}

#ifdef MOUSE_DEBUG
const char* MouseButton(int button)
{
  if (button == GLUT_LEFT_BUTTON)
  {
    return "LEFT";
  }
  else if (button == GLUT_RIGHT_BUTTON)
  {
    return "RIGHT";
  }
  else if (button == GLUT_MIDDLE_BUTTON)
  {
    return "MIDDLE";
  }
  else
  {
    return "Unexpected code!";
  }
}
#endif

void mousedown(int button, int state, int x, int y)
{
  engine->MousePos(x, y);

  // NB button refers to ONE button, it is not a bit field.

#ifdef MOUSE_DEBUG
std::cout << "** MOUSE BUTTON DOWN: " << MouseButton(button) 
  << " (" << button << ") " 
  << " state: " << (state == GLUT_DOWN ? "down" : "up")  << "\n";
#endif

  bool ctrl = (glutGetModifiers() == GLUT_ACTIVE_CTRL);
  bool shift = (glutGetModifiers() == GLUT_ACTIVE_SHIFT);
  bool down = (state == GLUT_DOWN);

  // This may be a mac only problem:
  // because right and middle buttons are simulated by pressing
  // keys as well as the mouse button, a right- or middle-button
  // down message may not get a corresponding up message, because
  // the keys may not be pressed when the mouse button goes up.

  // Remember the last button down message. If the new state is up,
  // then make sure the appropriate button-up function is called.

  // NB Should this be a bit field ? Currently it is just the code
  // of the last button pressed, not the sum of all buttons currently
  // held down.
  static int lastbutton = -1; // i.e. no button pressed

  if (!down)
  {
#ifdef MOUSE_DEBUG
std::cout << "** lastbutton is " << lastbutton << "...\n";
#endif

    if (lastbutton == GLUT_MIDDLE_BUTTON)
    {
#ifdef MOUSE_DEBUG
std::cout << "** ..so calling MouseMidButton(false)\n";
#endif
      engine->MouseMidButton(false, ctrl, shift);
    }
    else if (lastbutton == GLUT_LEFT_BUTTON)
    {
#ifdef MOUSE_DEBUG
std::cout << "** ..so calling Mouse(L)Button(false)\n";
#endif
      engine->MouseButton(false, ctrl, shift);
    }
    else if (lastbutton == GLUT_RIGHT_BUTTON)
    {
#ifdef MOUSE_DEBUG
std::cout << "** ..so calling MouseRButton(false)\n";
#endif
      engine->MouseRButton(false, ctrl, shift);
    }
    else
    {
#ifdef MOUSE_DEBUG
std::cout << "** ..so not calling anything, and RETURNING\n";
#endif
    }
    lastbutton = -1;
    return;
  }

  lastbutton = button;
#ifdef MOUSE_DEBUG
std::cout << "** setting lastbutton to " << lastbutton 
  << " button is " << button << "\n"; 
#endif

  // NB Middle button: hold down alt and option + mouse button
  // Right button: hold down ctrl + button
  if (button == GLUT_MIDDLE_BUTTON)
  {
#ifdef MOUSE_DEBUG
std::cout << "** ..so calling MouseMidButton(true)\n";
#endif
    engine->MouseMidButton(down, ctrl, shift);
  }
  else if (button == GLUT_LEFT_BUTTON)
  {
#ifdef MOUSE_DEBUG
std::cout << "** ..so calling Mouse(L)Button(true)\n";
#endif
    engine->MouseButton(down, ctrl, shift);
  }
  else if (button == GLUT_RIGHT_BUTTON)
  {
#ifdef MOUSE_DEBUG
std::cout << "** ..so calling MouseRButton(true)\n";
#endif
    engine->MouseRButton(down, ctrl, shift);
  }
  else
  {
#ifdef MOUSE_DEBUG
std::cout << "** ..so not calling anything.. and returning.\n";
#endif
  }
}

void mousemove(int x, int y)
{
  engine->MousePos(x, y);
}

void key(char c, bool down)
{
#ifdef KEY_DEBUG
std::cout << "ON KEY : " << (int)c << "\n";
#endif

  engine->OnKey(c, down); // map char codes to engine functions.
}

void special(int c, bool d)
{
  switch(c)
  {
  case GLUT_KEY_UP:
    engine->PlusUp(d);
    return;
  case GLUT_KEY_DOWN:
    engine->PlusDown(d);
    return;
  case GLUT_KEY_LEFT:
    engine->PlusLeft(d);
    return;
  case GLUT_KEY_RIGHT:
    engine->PlusRight(d);
    return;
  }
}

void specialkeyup(int c, int, int)
{
  special(c, false);
}

void specialkeydown(int c, int, int)
{
  special(c, true);
}

void keydown(unsigned char c, int, int)
{
  /*
  // Escape -> quit immediately
  if (c == 27)
  {
    if (glutGameModeGet(GLUT_GAME_MODE_ACTIVE))
    {
      glutLeaveGameMode();
    }
    exit(0);
  }
  */

  key(c, true);

#if defined(_DEBUG)
  // Draw height polygons
  if (c == 'D' || c == 'd')
  {
    engine->SetDrawHeights(!engine->GetDrawHeights());
  }
  // Show stats
  if (c == 'S' || c == 's')
  {
    engine->SetShowStats(!engine->GetShowStats());
  }
  // Show bounding spheres
  if (c == 'U' || c == 'u')
  {
    BoundingSphere::SetDraw(!BoundingSphere::GetDraw());
  }
#endif
}

void keyup(unsigned char c, int, int)
{
  key(c, false);
}

void SetGlutCallbacks()
{
  glutDisplayFunc(draw);
  glutIdleFunc(idle);
  glutReshapeFunc(resize);
  glutKeyboardFunc(keydown);
  glutKeyboardUpFunc(keyup);
  // "Special" arrow keys
  glutSpecialFunc(specialkeydown);
  glutSpecialUpFunc(specialkeyup);

#ifndef MACOSX_BUNDLE
  // GLUT mouse functions don't work with Mighty Mouse.
  // So this is for non-release builds only.
  glutMouseFunc(mousedown);
  glutMotionFunc(mousemove);
  glutPassiveMotionFunc(mousemove);
#endif
}

void SetupWindow(bool fullscreen)
{
  // Window size
  int gamex = atoi(engine->GetConfigValue("game_x").c_str());
  if (gamex < 64)
  {
    gamex = 64;
  }
  int gamey = atoi(engine->GetConfigValue("game_y").c_str());
  if (gamey < 64)
  {
    gamey = 64;
  }

  // If we want fullscreen, use GlutGameMode.
  if (fullscreen)
  {
    glutGameModeString("640x480:32@60");
    
    // See what values we actually get, then amend game mode string.
    // Hopefully this will allow full screen to work for everyone.
    int width = glutGameModeGet( GLUT_GAME_MODE_WIDTH );
    int height = glutGameModeGet( GLUT_GAME_MODE_WIDTH );
    int pixelDepth = glutGameModeGet( GLUT_GAME_MODE_PIXEL_DEPTH );
    int refreshRate = glutGameModeGet( GLUT_GAME_MODE_REFRESH_RATE );
    // NB This value is 0 for laptop - maybe LCD screens ?
    // Try on CRT -- TODO
   
#ifdef _DEBUG
std::cout << "Glut game results: width: " << width
  << " height: " << height
  << " depth: " << pixelDepth
  << " rate: " << refreshRate
  << "\n";
#endif

    std::string gameMode = ToString(width);
    gameMode += "x";
    gameMode += ToString(height);
    gameMode += ":";
    gameMode += ToString(pixelDepth);
    gameMode += "@";
    gameMode += ToString(refreshRate);
#ifdef _DEBUG
std::cout << "New game mode string: \"" << gameMode << "\"\n";
#endif
    glutGameModeString(gameMode.c_str());

    glutEnterGameMode(); 
  }
  else
  {
    // Not required, we are not in game mode.
    //glutLeaveGameMode();

    glutInitWindowSize(gamex, gamey); 
    glutCreateWindow(GAME_TITLE);
  }

  AmjuGL::SetImpl(new AmjuGLOpenGL);
  AmjuGL::Init();

  SetGlutCallbacks();
  

  if (fullscreen)
  {
    int width = glutGameModeGet( GLUT_GAME_MODE_WIDTH );
    int height = glutGameModeGet( GLUT_GAME_MODE_WIDTH );
    engine->SetViewport(width, height);
  }
  else
  {
    engine->SetViewport(gamex, gamey);
  }  
}

int main(int argc, char **argv)
{
  time_t tt;
  time(&tt);
  srand(tt);

  theTimer.SetInterval(1.0 / 30.0);

  // Initialize sockets for this process.
  //SocketService s;


  engine = Engine::Instance();
  engine->GetGameState()->SetName("gs.cfg");
  engine->GetGameState()->Load();

#ifdef POOL_ONLINE
  // Set Server for online games
  std::string server = engine->GetEngineState()->Get("server");
  if (server == "")
  {
    server = "www.amju.com/cgi-bin/";
    engine->GetEngineState()->Set("server", server);
  }
  ThePoolOnlineManager::Instance()->SetServerUrl(server);
#ifdef _DEBUG
std::cout << "POOL ONLINE server: " << server.c_str() << "\n";
#endif
#endif

  ConfigHack(); // set config from auto-generated source file.

  std::string root = GetRoot();
  std::cout << "Root is " << root.c_str() << endl;
  File::SetRoot(root, "/");

#ifdef _DEBUG
std::cout << "main(): got root\n";
#endif

  engine->LoadConfigFile(GAME_CFG);
  // If there is no config file, there will be error messages, so delete them.
  engine->GetTextWriter()->ClearScrollingItems();

  // Set up the "Glue" file, (the data archive file), if required.
  // Generally this should be on for a release build, off for development.
  bool glue = (engine->GetConfigValue("glue") == "y"); 

  if (glue)
  { 
    cout << "Glue file is enabled." << endl;
    string gluefile = "data.glue"; // TODO CONFIG

    GlueFile* pGf = new GlueFileMem;
    if (FileImplGlue::OpenGlueFile(gluefile, pGf)) 
    {
      cout << "Opened glue file ok." << endl;
    }
    else
    {
      cout << "Failed to open glue file." << endl;
      cout << "Path: " << File::GetRoot().c_str() << "\n";
      return -1;
    }
  }
  else
  {
    cout << "Glue file is disabled." << endl;
  }

/*
  // Load preferences config file.
  g_pPrefs = new ConfigFile;
  if (!g_pPrefs->Load("prefs.cfg"))
  {
    cout << "Couldn't load preferences config file.\n";
    // Continue
  }
*/

  // ONLINE Pool: we don't want to change the frame rate!
#if 0 
  // Set the framerate if defined in config file.
  float fps = atof(engine->GetValue("fixed_frame_rate").c_str());
  if (fps > 0)
  {
    cout << "Setting fixed framerate: " << fps << " fps\n";
    theTimer.SetInterval(1.0f / fps);
  }
#endif

  /*
  // For now, network multiplayer is not supported.
  if (!engine->NetworkInit())
  {
    cout << "No server found." << endl;
  }
  */

  // Fullscreen flag
  bool fullscreen = (engine->GetGameState()->Get("fullscreen") == "y");
  isFullScreen = fullscreen;

  // Set initial state.
  // FULLSCREEN: If this is the first time we have started up in fullscreen,
  // show the full screen test state first.
  bool firstTimeFullScreen = false; 
  if (isFullScreen)
  {
#ifdef _DEBUG
std::cout << "main(): fullscreen flag is set!\n";
#endif

    // Check for "first time" flag. 
    firstTimeFullScreen = !(engine->GetGameState()->Get("fsconfirmed") == "y");
  }

/*
  if (firstTimeFullScreen)
  {
    Engine::Instance()->ChangeState(EngineStateFullScreenTest::Name, Engine::IMMEDIATE);
  }
  else
  {
    Engine::Instance()->ChangeState(EsLogo::Name, Engine::IMMEDIATE);
  }
*/
  Engine::Instance()->ChangeState(EsLogo::Name, Engine::IMMEDIATE);

#ifdef _DEBUG
std::cout << "main(): changed state to EsLogo ok\n";
#endif

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

  SetupWindow(fullscreen);

#ifdef _DEBUG
std::cout << "main(): called SetupWindow()\n";
#endif
  
  engine->InitGl(); 

  if (!engine->LoadFont())
  {
    cout << "Couldn't load font." << endl;
    return -1;
  }
#ifdef _DEBUG
  std::cout << "main(): Loaded font.\n";
#endif
  engine->SetJoystickEnabled(false);

  // Set the leaf renderer. We can choose a renderer which sorts the textures, to test
  // this for any speed increase.
  bool sortedTextures = (engine->GetConfigValue("sort_textures") == "y");
  if (sortedTextures)
  {
    //LeafData::SetRenderer(new LeafRenderOglSorted);
  }
  else
  {
    LeafData::SetRenderer(new LeafRenderOpenGL);
  }

#ifdef _DEBUG
  std::cout << "main(): setting sound player...\n";
#endif

  // Set the sound player.
//  SoundPlayer* sp = new BassSoundPlayer;
//  sp->SetGlueFile(FileImplGlue::GetGlueFile());
//  engine->SetSoundPlayer(sp);

#ifdef _DEBUG
  std::cout << "main(): set sound player ok\n";
#endif

  // Set the Polygon loader and drawer.
  Amju::Polygon::SetDrawer(new PolyDrawOpenGL);
  Amju::Polygon::SetLoader(new PolyLoader);

  engine->SetShowStats(true);

#if defined(MACOSX)
  //SpeechSynth::Instance()->SetImpl(new MacSpeechSynth);
#endif
#ifdef _DEBUG
  std::cout << "Ready to run\n";
#endif

  glutMainLoop();
  return 0;
}
