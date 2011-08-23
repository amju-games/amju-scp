/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: SoundPlayer.h,v $
Revision 1.1.10.1  2006/03/09 23:17:31  jay
Merge trunk improvements

Revision 1.2  2006/02/14 13:36:51  jay
Allow a Glue File for the Sound Player, which is separate from the main
Glue File.
StopSong() does not need a parameter.

Revision 1.1  2004/09/08 15:42:57  jay
Added to repository
  
*/

#if !defined(SOUND_PLAYER_H_INCLUDED)
#define SOUND_PLAYER_H_INCLUDED

#include <string>
#include "SharedPtr.h"
#include "GlueFile.h"

namespace Amju
{
// Base class - doesn't do anything. Subclasses implement sound-playing for 
// different platforms.
class SoundPlayer : public Shareable
{
public:
  // Get/Set the Glue File used by the file callbacks.
  // This lets us set a different Glue File to the one used for the bulk
  // of the game data.
  void SetGlueFile(GlueFile* glueFile) { m_pGlueFile = glueFile; }
  GlueFile* GetGlueFile() { return m_pGlueFile.GetPtr(); }

public:
  SoundPlayer() : m_maxWavVol(1.0f), m_maxSongVol(1.0f) {}
  virtual ~SoundPlayer() {}

  // Play a .WAV file. Return immediately, with false if sound can't be played.
  // volume parameter should vary between 0 (silent) and 1.0 (full volume).
  virtual bool PlayWav(const std::string& wavFile, float volume = 1.0f) { return true; }

  // Play a song file. Return immediately, with false if file can't be played.
  // Song files are expected to be MOD/S3M etc format.
  // MIDI is currently NOT supported.
  virtual bool PlaySong(const std::string& songFile) { return true; }

  // Stop playing the current song. This should be ok to call if the file wasn't
  // being played anyway. 
  virtual void StopSong() {}

  // Call this every time round the game loop. This is so we can tell the
  // Engine when a sound has finished: it allows us to call the 
  // Callback by polling if we are unable to do it any other way.
  virtual void Update() {}

  // Set volume for WAVS, between 1.0 (full volume) and 0 (silent)
  virtual void SetWavMaxVolume(float f) { m_maxWavVol = f; }

  // Set volume for songs, between 1.0 (full volume) and 0 (silent) 
  virtual void SetSongMaxVolume(float f) { m_maxSongVol = f; }

  float GetWavMaxVolume() const { return m_maxWavVol; }
  float GetSongMaxVolume() const { return m_maxSongVol; }

protected:
  float m_maxWavVol;
  float m_maxSongVol;

  SharedPtr<GlueFile> m_pGlueFile;
};

typedef SharedPtr<SoundPlayer> PSoundPlayer;
}
#endif
