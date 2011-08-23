/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: SoundFxManager.h,v $
Revision 1.1  2004/09/28 15:29:29  Administrator
Added SoundFxManager - rations repeating sound effects which can
otherwise use up all available channels, cutting off music.

*/

#if !defined(SOUND_FX_MANAGER_H_INCLUDED)
#define SOUND_FX_MANAGER_H_INCLUDED

#include <map>
#include <string>
#include "Singleton.h"

namespace Amju
{
// This class is here to manage the situation where lots of copies of the
// same sound effect are played in quick succession. 
class SingleSoundFxManager
{
public:
  void PlayWav(const char*, float vol = 1.0f);

protected:
  // Map of the number of copies of each wav being played, and the last 
  // time the wav was played.
  typedef std::map<std::string, std::pair<int, float> > WavMap;
  WavMap m_wavs;
};

typedef Singleton<SingleSoundFxManager> SoundFxManager;
}

#endif
