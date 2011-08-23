/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: SoundFxManager.cpp,v $
Revision 1.1  2004/09/28 15:29:29  Administrator
Added SoundFxManager - rations repeating sound effects which can
otherwise use up all available channels, cutting off music.

*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <utility>
#include "SoundFxManager.h"
#include "Engine.h"

namespace Amju
{
void SingleSoundFxManager::PlayWav(const char* wav, float vol)
{
  float t = Engine::Instance()->GetElapsedTime();

  WavMap::iterator it = m_wavs.find(wav);
  if (it == m_wavs.end())
  {
    m_wavs[wav] = std::make_pair(1, t);
    Engine::Instance()->PlayWav(wav, vol);
    return;
  }

  // The idea is: if more than 3 copies of the same sound are playing,
  // wait 3 seconds before allowing any more copies of the sound to play.
  // After the 3 secs, up to 3 of the same sounds can be played again, etc.

  if (t - it->second.second > 3.0f) // TODO 
  {
    it->second.first = 0;
  }

  if (it->second.first > 3 )  // TODO
  {
    return;
  }

  ++it->second.first;
  it->second.second = t;
  Engine::Instance()->PlayWav(wav, vol);

}

}
