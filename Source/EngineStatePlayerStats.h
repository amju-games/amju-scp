/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStatePlayerStats.h,v $
Revision 1.2  2004/09/17 13:56:20  jay
Improvements to player stats

Revision 1.1  2004/09/15 09:12:35  Administrator
Adding player stats to game code
  
*/

#if !defined(ENGINE_STATE_PLAYER_STATS_H_INCLUDED)
#define ENGINE_STATE_PLAYER_STATS_H_INCLUDED

#include <string>
#include <utility>
#include <vector>
#include "EngineStateText.h"
#include "GuiElement.h"
#include "Guage.h"
#include "CharacterGameObject.h"

namespace Amju
{
// Show stats for a player. This could be useful in a number of
// games, so this class should be overridden to do game-specific
// stuff.
// The stats are shown as guages, with a label identifying each
// guage.
// The guages may be read-only or the user may be able to change
// them - use a draggable PoolGuiElement for this.
class EngineStatePlayerStats : public EngineStateText
{
public:
  virtual void Draw();
  virtual void DrawOverlays();
  virtual bool Load();
  virtual void MouseButton(bool, bool, bool);
  virtual void MousePos(int, int);
  virtual void SetActive(bool);

  // Called when player clicks "OK" for his stats.
  virtual void Red(bool);
 
  // Clear any old stats we have stored.
  virtual void OnNewGame();

protected:
  // Set up stats display for current player
  void InitPlayer();

  // Final player has been displayed - go to next state.
  virtual void Finished() = 0;

  // Clear old stats
  void ClearStats();

  // Store stats for this player
  void StoreStats(int player, const std::vector<float>& stats);

  // Get previous stat values for player
  const std::vector<float>& GetStats(int player);

protected:
  // OK button - player clicks when done with stats.
  PPoolGuiElement m_pOkButton;
  // 3D ok text
  PSolidComponent m_pOkText;

  // Guage with name/label identifying it
  typedef std::pair<std::string, RCPtr<Guage> > NameGuage;
  // Collection of guages.
  typedef std::vector<NameGuage> NameGuages;
  NameGuages m_nameGuages;
  // Positions for guage labels
  // Pairs of (x, y) text coords.
  std::vector<std::pair<float, float> > m_textPositions;

  // The characters whose stats we are displaying.
  RCPtr<CharacterGameObject> m_pChar;

  // Player whose stats we are currently displaying.
  int m_currentPlayer;

  // Stored copies of player stats. We use these to transition from
  // the old values to the latest values.
  std::vector<std::vector<float> > m_prevStats;
};
}

#endif


