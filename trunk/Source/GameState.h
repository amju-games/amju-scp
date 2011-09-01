/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PoolGameState.h,v $
Revision 1.2.8.6  2006/08/26 21:27:40  jay
Added cue ball position to pool shot info

Revision 1.2.8.5  2006/08/14 17:37:30  jay
Rename "Pool"

Revision 1.2.8.4  2005/07/30 12:13:45  jay
Added SetPlayerInfo()

Revision 1.2.8.3  2005/05/24 20:35:27  jay
Add Player Stats for pool, and friendly games.

Revision 1.2.8.2  2005/05/15 17:12:06  jay
Add cue elevation angle

Revision 1.2.8.1  2005/03/31 22:22:19  jay
Added more player state for Pool game.

Revision 1.2  2004/09/15 09:16:35  Administrator
Added stats to PlayerInfo

Revision 1.1  2004/09/08 15:42:51  jay
Added to repository
  
*/

#if !defined(POOL_GAME_STATE_H_INCLUDED)
#define POOL_GAME_STATE_H_INCLUDED

#include <map>
#include <string>
#include <vector>
#include "ConfigFile.h"
#include "Orientation.h"
#include "TexturedQuad.h"
#include "PlayerStats.h"
#include "Vertex.h"

namespace Amju
{
// The state of the game. E.g. positions of moveable/takeable objects,
// player health, etc. Can be loaded and saved.
class PoolGameState
{
public:
  PoolGameState();
  virtual ~PoolGameState();

  void SetName(const std::string& name) { m_filename = name; }

  // Load and save use the object's name as the filename.
  virtual bool Load();
  virtual bool Save();

  void Set(const std::string& key, const std::string& value);
  void Set(const std::string& key, float f);

  std::string Get(const std::string& key);
  float GetFloat(const std::string& key);

  // Set default values when Load fails.
  void Init();

  // Multi-player functions
  // ----------------------
  struct PlayerInfo
  {
    PlayerInfo(int id, const std::string& name, const Orientation& o, bool isAi);

    int m_id;
    Orientation m_or;
    std::string m_name;
    bool m_isAi;
    int m_score;
    bool m_isPlaying; // false once level complete for this player
    bool m_reset; // if true, reset orientation to start
    int m_strokes; // number of strokes taken for current hole
    int m_numBalls; // number of balls this player has left (zero means game over)
    bool m_isHighPower; // (for animation) is this a putt or a swing

    // POOL: extra info for english and draw/roll.
    // NB Cue ball jump is taken care of with vertical vel.
    struct PoolStroke
    {
      PoolStroke(
        float yRot, 
        float vertVel, 
        float horVel, 
        float accel,
        float english,
        float drawRoll) :
        m_yRot(yRot), m_vertVel(vertVel), m_horVel(horVel), m_accel(accel),
        m_english(english), m_drawRoll(drawRoll), m_cueElevationDegs(0)  {}

      void SetCueBallPos(const VertexBase& v) { m_cueBallPos = v; }
      const VertexBase& GetCueBallPos() const { return m_cueBallPos; }

      float m_yRot, m_vertVel, m_horVel, m_accel;
      float m_english, m_drawRoll;
      float m_cueElevationDegs;
      VertexBase m_cueBallPos;
    };
    PoolStroke m_golfStroke;
 
    // Game Stats, so player can improve etc.
    int m_totalStrokes; // total no of strokes for course
    int m_totalBonuses; // total number of bonuses collected in this course
    int m_animalsHit; // total number of animals hit in this course
    int m_blockersMoved; // number of blockers moved in this course

    std::vector<float> m_stats;
  
    // POOL: List of textured quads, one for each ball potted by this player.
    std::vector<TexturedQuad> m_tqs;

    // Is this player online ?
    bool m_isOnline;

    // POOL: Player stats for all played games
    SharedPtr<PlayerStats> m_pPlayerStats;
    PlayerStats* GetPlayerStats() { return m_pPlayerStats.GetPtr(); }
    void SetPlayerStats(PlayerStats* p) { m_pPlayerStats = p; }
  };

  int GetNumberOfPlayers() const;

  PlayerInfo* GetCurrentPlayerInfo();

  PlayerInfo* GetPlayerInfo(int index);

  // Set player info - for restoring saved positions
  void SetPlayerInfo(int index, const PlayerInfo& info);

  void SetCurrentPlayer(int index);

  int GetCurrentPlayer() const;

  // Rotate the player ID to the next player. Wraps around.
  // Returns the new player ID.
  int RotateToNextPlayer();

  void AddPlayerInfo(const PlayerInfo&);
  void ClearPlayers();

  void SetIsFriendly(bool friendly);
  bool IsFriendly() const;

protected:
  int Checksum();

protected:
  ConfigFile m_cf;
  std::string m_filename;

protected:
  int m_currentPlayerId;
  std::vector<PlayerInfo> m_playersInfo;
  // Is friendly game - if so game stats are not recorded.
  bool m_isFriendly;
};

}
#endif

