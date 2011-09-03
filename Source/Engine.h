/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Engine.h,v $
Revision 1.2.6.4  2007/03/09 23:01:58  jay
POOL_ONLINE: special version of GetDeltaTime

Revision 1.2.6.3  2006/03/18 23:23:13  Administrator
StopSong() takes one param only

Revision 1.2.6.2  2005/10/29 10:15:10  jay
Add support for OS-specific special events.
E.g. Paste command.

Revision 1.2.6.1  2005/07/17 23:00:53  jay
Handle Right and Middle mouse buttons

Revision 1.2  2004/09/15 09:18:25  Administrator
Added two convenience functions

Revision 1.1  2004/09/08 15:42:48  jay
Added to repository
  
*/

#if !defined(ENGINE_H_INCLUDED)
#define ENGINE_H_INCLUDED

#include <map>
#include <stack>
#include <string>
#include <utility> // pair
#include "ConfigKeys.h"
#include "EngineState.h"
#include "ConfigFile.h"
#include "TexturedQuad.h"
#include "Message.h"
#include "GameObject.h"
#include "SpecialRecipient.h"
#include "SoundPlayer.h"
#include "MessageQueue.h"
#include "Earthquake.h"
#include "Camera.h"
#include "Colour.h"
#include "DayNightSky.h"
#include "Keymap.h"
#include "ProgressCaller.h"
#include "LoadResult.h"
#include "SpecialEvent.h"
#include <GameState.h>

namespace Amju
{
class Level;
class Character;
class PoolGameState;
class ClientSocket;
class ClientReceiveThread;
class TextWriter;
class Player;

class Engine : public ProgressCaller, public GameState
{
public:
  static Engine* Instance(); // Singleton
  ~Engine();

  virtual void Draw2d(); // GameState override

  // EventHandler overrides
  bool OnCursorEvent(const CursorEvent&);
  bool OnMouseButtonEvent(const MouseButtonEvent&);
  bool OnButtonEvent(const ButtonEvent&);

  // Initialisation
  // --------------

  // Call to load the config file, so we can get game options.
  bool LoadConfigFile(const std::string& configFileName);

  // Load the game data.
  bool Load();

  // Load the minimum data needed for the Engine to work. This is used by
  // the editing tool.. it's very tightly coupled with the game code.
  bool LoadMinimum();

  // Load the font. This is separate from loading other things because if the
  // font doesn't load, you can't report errors.
  bool LoadFont();

  // Delete everything allocated. This is really to check that there are no
  // unexplained leaks.
  void CleanUp();

  // Once-only GL initialisation code. Called from main(), once window is set up.
  void InitGl();
 
  // Called when game is started. The filename for a saved game is given.
  void StartGame(const std::string& savedGameName);

  // Look up a value in the game config file. 
  std::string GetConfigValue(const Cfg::Key& key) const;
  // Convert string value to float for convenience.
  float GetConfigFloat(const Cfg::Key& key) const;

  // Set a value in the config. This is used to set up defaults, which can then
  // be overridden from a file.
  void SetConfigValue(const char* key, const char* value);

  // Reload everything. Used for debugging.
  bool ReLoad();

  // Network Initialization. Connects to server if there is one.
  bool NetworkInit();


  // Update: called every frame after SetElapsedTime().
  // --------------------------------------------------

  // Separated from Draw(). The idea is that the graphics card can be busy with
  // drawing, which the CPU does the updates.
  // May need to experiment with best calling order..?
  void Update();


  // Handle events - called from OS-specific code
  // --------------------------------------------

  // Draw a frame. 
  void Draw();

  // Set viewport size: call on window resize.
  void SetViewport(int x, int y);
  // Get window size
  int GetWindowX() const;
  int GetWindowY() const;


  // Error reporting/clearing error text
  // -----------------------------------

  // Write an error mesage. Used in initial loading state.
  void ReportError(const std::string& errorMessage) const;

  // Clear textbox text, e.g. loading messages.
  void ClearText();


  // Colour functions
  // ----------------

  // Modulate whatever the current colour is. The colour starts off as (1, 1, 1, 1).
  // Each call should be matched with a PopColour().
  void PushColour(float r, float g, float b, float a);
  void PushColour(const Colour& c) { PushColour(c.m_r, c.m_g, c.m_b, c.m_a); }

  // This colour does not modulate the current colour.
  // Useful for picking (render to back buffer only).
  void PushAbsColour(float r, float g, float b, float a);

  // Restores the current colour to whatever it was before the last call to 
  // PushColour().
  void PopColour();

  // Set OpenGL clear colour.
  void SetClearColour(float r, float g, float b);


  // Special effects
  // ---------------

  // Set the letterbox/wide screen look.
  void SetLetterbox(bool);

  // Access the Earthquake object owned by the engine.
  Earthquake& GetEarthquake() { return m_earthquake; }
  void SetEarthquake(float, float);
 

  // Get access to objects managed by Engine
  // ---------------------------------------

  // Writes text to screen.
  TextWriter* GetTextWriter();
  void SetTextWriter(TextWriter* pTextWriter);

  // Get background box, for drawing Caption Boxes, Menus, etc.
//  TexturedQuad* GetBgBox() { return &m_bgBox; }

  // Get the player character.
  Player* GetPlayer(); 
  
  // Point to the object holding the game status - i.e. the player score,
  // health, inventory, etc.
  PoolGameState* GetGameState();

  // The skybox
  DayNightSky* GetDayNightSky() { return &m_dayNightSky; }

  // The obligatory lens flare.
//  Lensflare* GetLensflare() { return &m_lensflare; }

  // Allows access to the current camera. May return zero.
  PCamera GetCamera();


  // Game Object functions
  // ---------------------
  // Game Objects are stateful, usually visible objects in the Game. All Game
  // Objects are owned by the Engine. They are passed to the Engine using
  // HoldGameObject(), by whatever loads the object.
  // Game Objects can then be looked up by unique ID, using GetGameObject.
  // You can also get a map of all Game Objects in a particular room.

  // Access the map of all Game Objects
  GameObjectMap& GetGameObjects();

  // Get a map of Game object Ids to Game Objects, for a given room in a level.
  GameObjectMap& GetGameObjects(int levelId, int roomId);

  // Look up one game object from its ID.
  PPoolGameObject GetGameObject(GameObjectId gid);

  // This engine owns all the Game Objects. When a Game Object is loaded it is
  // handed over using this function.
  void HoldGameObject(int levelId, int roomId, PPoolGameObject pGo);

  // Clear all Game Objects. This is required by the Editor, and
  // by LevelFactory for creating levels on the fly.
  void ClearGameObjects();

  // Call to change the room which a game object is in.
  void GameObjectChangeRoom(GameObjectId id, int newRoomId);

  // Remove one Game Object from the Engine.
  // Used for Level Editing, not playing the Game.
  void ClearGameObject(int id);

  // Engine State functions
  // ----------------------

  // Possible ways of changing from one state to another.
  // These are used as arguments to ChangeState().
  enum ChangeMethod
  {
    FADE, IMMEDIATE 
  };

  void ChangeState(const std::string& newStateName, ChangeMethod);
  void ChangeState(PEngineState pState, ChangeMethod);

  // Get an Engine State. This is not the player's state, 
  // but the states the Engine can be in. See State pattern.
  PEngineState GetEngineState(const std::string& stateName);

  // Get the current engine state.
  PEngineState GetEngineState();

  // Register an Engine State with the Engine. Once registered, the Engine can be
  // put into the state using ChangeState().
  bool RegisterEngineState(const std::string& stateName, EngineState* pState);

  // Set name of next state to go to after the current state.
  void SetNextStateName(const std::string stateName);
  std::string GetNextStateName() const;

  // Return true if we are fading out of one state to another.
  bool IsFading() const { return m_isFading; }


  // Message Queue functions
  // -----------------------

  // Send a Message to a Game Object. This will change the state of the 
  // recipient.
  void SendGameMessage(PMessage);

  // Handle a message received from the network server.
  void NetworkReceiveMessage(PMessage);

  // Clear all game messages in the message queue intended for the given recipient.
  void ClearMessages(int recipientId);

  // Clear all game messages for all recipients.
  void ClearMessageQueue();
  

  // Cut Scene functions
  // -------------------

  // Play a Cut Scene. The scene should have loaded as part of a level. The 
  // level adds the scene to the Engine.
//  void PlayCutScene(int seqId);
  // Maybe don't need this ?
//  void FinishCutScene();


  // Sound functions
  // ---------------

  // Sound player should be set up by OS-specific main().
  void SetSoundPlayer(SoundPlayer* pSp) { m_pSoundPlayer = pSp; }
  PSoundPlayer GetSoundPlayer() { return m_pSoundPlayer; }

  // Play a wav file. Volume is from 0 (silent) to 1.0 (full volume)
  bool PlayWav(const std::string& file, float volume = 1.0f);

  // Play a song file.
  bool PlaySong(const std::string& file);

  // Stop playing the current song 
  void StopSong();

  // Callback: this function is called when the specified Song file finishes.
  void CallbackSongFinished(const std::string& file);


  // Keyboard input functions
  // ------------------------

  // Keyboard input.
  void OnKey(char key, bool down);
  
  // Get the Keymap - used for displaying keys, also could be used to configure.
  Keymap* GetKeymap() { return &m_keymap; }
  
  // Show keys flag.
  bool GetShowKeys() const { return m_showKeys; }
  void SetShowKeys(bool b) { m_showKeys = b; }

  // Special Key combination pressed, or some OS-specific event has occurred.
  // This is called from OS-specific event handlers.
  void OnSpecialEvent(SpecialEvent*);


  // Player Input functions
  // ----------------------

  // Call when Controller buttons are pressed/released.
  // Parameter is true when button is pressed, false when released.
  void Red(bool);
  void Blue(bool);
  void Green(bool);
  void YellowUp(bool);
  void YellowDown(bool);
  void YellowLeft(bool);
  void YellowRight(bool);
  void Z(bool);   // Trigger
  void PlusUp(bool); // '+' - shaped pad ('D' pad)
  void PlusDown(bool);
  void PlusLeft(bool);
  void PlusRight(bool);

  // Analogue joystick X position: 0 is centred; -1 is fully left;
  //  +1 is fully right.
  void JoyX(float f);
  // Analogue Y position: 0 is centred; -1 is fully back; +1 is fully forward.
  void JoyY(float f);

  // Joystick enabled flag.
  bool IsJoystickEnabled() const { return m_joystickEnabled; }
  void SetJoystickEnabled(bool b) { m_joystickEnabled = b; }

  // Mouse Input
  // Call to set mouse position: coords are relative to window origin, in 
  // OS-specific window size units.
  void MousePos(int x, int y);
  // Called when mouse button is pressed/released. 
  // (We only support one button.)
  // Parameter is true when botton is down, false when up.
  void MouseButton(bool down, bool ctrl, bool shift);
  // New for POOL: we now support right and middle buttons, in theory 
  // at least.
  void MouseRButton(bool down, bool ctrl, bool shift);
  void MouseMidButton(bool down, bool ctrl, bool shift);

  // Returns true if mouse has any effect on Engine.
  bool IsMouseEnabled() const;

  // Time functions
  // --------------

  // Call from OS-specific code. Set the number of seconds the executable has
  // been running.
  void SetElapsedTime(double secs);
  
  // Called by all time-dependent game elements.
  double GetElapsedTime() const { return m_elapsedSecs; }

  // Get interval between calls to SetElapsedTime().
  float GetDeltaTime() const;

  // Freeze the message queue and store current time if freeze is true;
  // if false, unfreeze queue and restore time.
  void Freeze(bool freeze);


  // Notify Engine of game events
  // ----------------------------

  // Called when player room changes
  void OnNewRoom();

  // Called by Level objects when the player has completed the objective.
  void LevelCompleted();

  // Called when player is dead.
  void GameOver();

  // ------------------------------------------------------------------------

  // Show Frames per sec 
  void Fps();

  // ------------------------------------------------------------------------


  // TODO temp function to set static scene as lines only, showing heights.
  void SetDrawHeights(bool b) { m_drawHeights = b; }
  bool GetDrawHeights() const { return m_drawHeights; }

  // Flag: set to true to show debug stats on screen.
  void SetShowStats(bool b);
  bool GetShowStats() const { return m_showStats; }

  // Flag: show bounding spheres for debugging
  void SetShowSpheres(bool b) { m_showSpheres = b; }
  bool GetShowSpheres() const { return m_showSpheres; }

  // Flags: if true, the corresponding bounding spheres should be drawn,
  // for debugging.
#if defined(_DEBUG)
  bool DoDrawSceneSpheres();
  bool DoDrawPlatformSpheres();
  bool DoDrawCharacterSpheres();
#else
  bool DoDrawSceneSpheres() { return false; }
  bool DoDrawPlatformSpheres() { return false; }
  bool DoDrawCharacterSpheres() { return false; }
#endif

protected:
  Engine(); // Singleton.

  // Initialise gl for drawing a frame.
  void InitFrame();

  bool LoadCharacters(const std::string& charList);

  void UpdateMessageQueue();
  void ProcessMessage(PMessage m);

  // Fade in/out effect.
  void Fade();
  void FinishedFading();
  void SetFadeState(PEngineState pNewState);
  // Immediately change the current state to the new state.
  void SetState(PEngineState pNewState);
  LoadResult  LoadEngineStates();

  // Letterbox effect.
  void DoLetterbox();
  void UndoLetterbox();

  void Clear(); 

  // Iterate over the Special Recipients, and call function on each one.
  void ForEachSpecialRecipient(void (SpecialRecipient::*pf)());

private:
  // Data members
  SharedPtr<Player> m_pPlayer; // the player character.

  int m_numFrames;

  double m_elapsedSecs;
  double m_oldElapsedSecs; // to get delta
  float m_deltaTime;
  float m_maxDeltaTime;

  // ID of level we start Game in.
  int m_startLevel;


  // Draws Text to screen using a font loaded from a bitmap file.
  TextWriter* m_pTextWriter;

  // The available states. We map state names to objects.
  typedef std::map<std::string, PEngineState> EngineStateMap;
  EngineStateMap m_engineStateMap;

  // Point to current state.
  PEngineState m_pCurrentState;
  // Point to the next state, when we are fading from one state to another.
  PEngineState m_pNewState;
  // Allow this state access to Load().
  friend class FileCheckState;

  // Stack of states. This simplifies menus, and changing to Pick state then 
  // back to the previous state, etc.
  typedef std::stack<PEngineState> StateStack;
  StateStack m_stateStack;

  // Point to platform-specific instance.
  PSoundPlayer m_pSoundPlayer;

  // Fading: this affects everything drawn, including overlays.
  bool m_isFading; // true if currently fading.
  float m_fade; // modulates colour of frame, from 1.0 (fully lit) to 0 (black).
  // Called when fade has finished, to swap to new state.

  // Debug flags
  bool m_showStats;
  bool m_showSpheres;
  bool m_drawHeights;

  // Config file object: maps keys to values.
  ConfigFile m_config;
  // Name of file which is used to populate m_config.
  std::string m_configFileName;

  // Clear colour r, g, b. 
  float m_clearR, m_clearG, m_clearB;

  PoolGameState* m_pGameState;

  // Map of game object IDs to pointers to game objects.
  GameObjectMap m_objectMap;
  // Map of level/room pairs to Game Object Maps. The idea of this is to speed 
  // up iterating over Game Objects, by only considering the ones in the same
  // room as the player.
  typedef std::map<std::pair<int, int>, GameObjectMap> GameObjectLocationMap;
  GameObjectLocationMap m_objectMaps;

  // Background box for text boxes, menus etc.
//  TexturedQuad m_bgBox;

  // 2D Box
//  TexturedQuad m_boxQuad;

  // Queue of messages. Prioritised such that the top message should be sent
  // first. 
  MessageQueue m_messageQueue;

  // X, Y size of viewport.
  int m_viewportX;
  int m_viewportY;
  // Letterbox/"wide screen" effect - 0 means no letterbox.
  float m_letterbox;

  // "Global" colour. This affects everything except overlays.
  float m_globalR, m_globalG, m_globalB;

  // Number of millisecs to sleep between each frame. This can be zero, but
  // you may want to sleep for e.g. 10ms between frames to stop the cpu
  // maxing out. Also if there is no apparent difference in elapsed time
  // between frames, we may as well sleep.
  int m_sleepMs;

  // Joystick variables
  bool m_joystickEnabled;
  // Used to prevent key auto-repeat affecting left/right actions.
  bool m_prevLeft;
  bool m_prevRight;
  // Zone for which joystick values are treated as zero.
  float m_joystickZeroZone;

  // Maps keys (characters) to Engine functions, allowing configurable keys.
  Keymap m_keymap;

  // Colour stack. Pushing a colour onto the stack modulates the colour
  // underneath. This lets us build up the final colour from Fade, ambient
  // colour etc.
  typedef std::stack<Colour> ColourStack;
  ColourStack m_colourStack;

  bool m_frozen; // if true, message queue is frozen
  float m_frozenTime; // stored Elapsed Time

  // If true, show keys, so user knows what to press.
  bool m_showKeys;

  // The sky, sun and moon
  DayNightSky m_dayNightSky;
  // Lensflare, drawn if the sun is in view.
//  Lensflare m_lensflare;

  // Special Message Recipients: SoundPlayer, CutScenePlayer, etc.
  SpecialRecipientMap m_specialRecipients;

  Earthquake m_earthquake;

  // This flag is set when player completes the current level.
  bool m_levelIsCompleted;

  // Name of state to go to when the current state calls GoToNextState().
  std::string m_nextStateName;

};
}
#endif

