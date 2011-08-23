/*
Amju Games source code (c) Copyright Jason Colman 2006
$Log: BassSoundPlayer.h,v $
Revision 1.3.2.2  2006/03/09 22:59:08  jay
Add BASS - to correct dir this time!

Revision 1.1.2.1  2006/03/09 21:49:08  jay
Add BASS to pool

Revision 1.2  2006/02/17 18:57:34  jay
Fix StopSong()

Revision 1.1  2006/02/14 13:23:01  jay
Added support for BASS sound system

*/

#ifndef AMAJU_BASS_SOUND_PLAYER_H_INCLUDED
#define AMAJU_BASS_SOUND_PLAYER_H_INCLUDED

#include "SoundPlayer.h"

namespace Amju
{
class BassSoundPlayer : public SoundPlayer
{
public:
  BassSoundPlayer();
  ~BassSoundPlayer();

  virtual bool PlayWav(const std::string& wavFile, float volume = 1.0f);
  virtual bool PlaySong(const std::string& songFile);
  virtual void StopSong();
  virtual void Update();
  virtual void SetSongMaxVolume(float);

private:
  // Channel for current song
  unsigned long m_chan;
};
}

#endif

