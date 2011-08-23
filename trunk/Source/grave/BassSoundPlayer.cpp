/*
Amju Games source code (c) Copyright Jason Colman 2006
$Log: BassSoundPlayer.cpp,v $
Revision 1.4.2.8  2007/08/11 10:09:15  jay
Change flags so playing music works when built on Win Vista

Revision 1.4.2.7  2007/03/25 19:51:36  Administrator
Play song: return on failure

Revision 1.4.2.6  2006/08/30 21:09:05  Administrator
Get Pool to build in MSVC

Revision 1.4.2.5  2006/06/02 18:01:47  jay
Fix bug - volume was being ignored!

Revision 1.4.2.4  2006/03/25 12:49:29  jay
Fix set song volume

Revision 1.4.4.5  2006/03/25 12:46:31  jay
Fix Song volume - this was incorrectly setting the SYSTEM volume!

Revision 1.4.4.4  2006/03/22 21:19:21  Administrator
Make sure song volume starts at 1.0

Revision 1.4.4.3  2006/03/15 23:27:00  Administrator
More debug output

Revision 1.4.4.2  2006/03/15 07:42:23  jay
Add BASS Sound lib

Revision 1.4.2.2  2006/03/09 22:59:08  jay
Add BASS - to correct dir this time!

Revision 1.1.2.1  2006/03/09 21:49:08  jay
Add BASS to pool

Revision 1.3  2006/02/17 18:57:21  jay
Add support for Glue Files; fix StopSong()

Revision 1.2  2006/02/15 23:32:59  Administrator
Fix Windows compile

Revision 1.1  2006/02/14 13:23:01  jay
Added support for BASS sound system

*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <iostream>
#include "BassSoundPlayer.h"
#include "StringUtils.h"
#ifdef MACOSX
#include "bass.h"
#endif
#ifdef WIN32
#include "Bass2.2/Win/bass.h"
#endif

namespace Amju
{
void ReportError(const std::string&);

BassSoundPlayer::BassSoundPlayer()
{
  m_chan = (DWORD)-1; 

  // check that BASS 2.2 was loaded
  if (BASS_GetVersion()!=MAKELONG(2,2)) 
  {
    ReportError("BASS version 2.2 was not loaded");
    return;
  }

  // setup output - default device
  if (!BASS_Init(-1,44100,0,0,NULL))
  {
    ReportError("BASS: Can't initialize device");
  }

  SetSongMaxVolume(1.0f);
}

BassSoundPlayer::~BassSoundPlayer()
{
  // TODO Shut down to avoid stuttering or clicks
}

bool BassSoundPlayer::PlayWav(const std::string& wavFile, float volume)
{
  if (m_maxWavVol == 0)
  {
    return true; // ok, not an error
  }

  // max no of simultaneous playbacks (of same wav ? or all wavs ?)
  static const int MAX_PLAYBACKS = 6;

  HSAMPLE hs = 0;
  if (GetGlueFile())
  {
    std::string strippedFile = StripPath(wavFile);
    // Find the start of the wav in the glue file; and find the length
    uint32 wavPos = 0;
    if (!GetGlueFile()->GetSeekBase(strippedFile, &wavPos))
    {
      std::string s = "BASS: Wav: not in Glue File: ";
      s += strippedFile;
      ReportError(s);
    }
    uint32 wavLength = GetGlueFile()->GetSize(strippedFile);
    // TODO TEMP TEST copy the wav - this is wasteful but GlueFile interface
    // doesn't expose the mem location of the contents.
    GlueFileBinaryData data = GetGlueFile()->GetBinary(wavPos, wavLength);
    hs = BASS_SampleLoad(
      TRUE, // in mem ?
      data.GetBuffer(), // start of wav in memory 
      0, // not used 
      wavLength,
      MAX_PLAYBACKS, 
      BASS_SAMPLE_OVER_POS); // flags
  }
  else
  {
    hs = BASS_SampleLoad(
      FALSE, // in mem ?
      wavFile.c_str(), // filename
      0, // file offset
      0, // "use all data up to end of file": 2 'iterators' defining a range ?
      MAX_PLAYBACKS, 
      BASS_SAMPLE_OVER_POS); // flags
  }

  if (!hs)
  {
    std::string s = "BASS: Wav: Failed to load sample: ";
    s += wavFile;
    ReportError(s);
    return false;
  } 
  HCHANNEL hc = BASS_SampleGetChannel(hs, FALSE);
  if (!hc)
  {
    std::string s = "BASS: Wav: Failed to get sample channel: ";
    s += wavFile;
    ReportError(s);
    return false;
  }

  BASS_ChannelPlay(hc, FALSE);

  // Set vol
  int vol = (int)(volume * m_maxWavVol * 100.0f);
  BASS_ChannelSetAttributes(hc, -1, vol, -1);

  return true;
}

bool BassSoundPlayer::PlaySong(const std::string& songFile)
{
  // Play song even if song vol is currently zero - it may be turned up.

#ifdef _DEBUG
std::cout << "BASS: playing new song: " << songFile.c_str() << "\n";
#endif
  // TODO If Glue File is set, use it to load song into memory.
  // Else use file.

//  DWORD act,time,level;
//  BOOL ismod;
//  QWORD pos;
//  int a;

  if (GetGlueFile())
  {
    std::string strippedFile = StripPath(songFile);
    // Find the start of the song in the glue file; and find the length
    uint32 songPos = 0;
    if (!GetGlueFile()->GetSeekBase(strippedFile, &songPos))
    {
      std::string s = "BASS: Music: not in Glue File: ";
      s += strippedFile;
      ReportError(s);
      return false;
    }
    uint32 length = GetGlueFile()->GetSize(strippedFile);
    // TODO TEMP TEST copy the wav - this is wasteful but GlueFile interface
    // doesn't expose the mem location of the contents.
    GlueFileBinaryData data = GetGlueFile()->GetBinary(songPos, length);
    if (!(m_chan=BASS_MusicLoad(
      TRUE, // mem ?
      data.GetBuffer(), // start of song data 
      0, // offset
      length, // length
	  // If you build on Windows Vista one of these flags silences the music!
      BASS_SAMPLE_LOOP| BASS_MUSIC_SURROUND, ///BASS_MUSIC_RAMPS|BASS_MUSIC_PRESCAN,
      0)))  // sample rate - 0 => use default value
    {
      std::string s = "BASS: Music: Can't play song from Glue file: "; 
      s += strippedFile;
      ReportError(s);
      return false;
    }
  }
  else
  {
    if (!(m_chan=BASS_MusicLoad(
      FALSE, // mem ?
      songFile.c_str(), // file
      0, // offset
      0, // length
	  // If you build on Windows Vista one of these flags silences the music!
      BASS_SAMPLE_LOOP | BASS_MUSIC_SURROUND, /// |BASS_MUSIC_RAMPS|BASS_MUSIC_PRESCAN,
      0)))  // sample rate - 0 => use default value
    {
      std::string s = "BASS: Music: Can't play file: "; 
      s += songFile;
      ReportError(s);
      return false;
    }
  }

  BASS_ChannelPlay(m_chan,FALSE);

  // Set vol
  int vol = (int)(m_maxSongVol * 100.0f);
  BASS_ChannelSetAttributes(m_chan, -1, vol, -1);

#ifdef _DEBUG
std::cout << "BASS: new song: " << songFile.c_str() << " chan: " << m_chan << "\n";
#endif

  return true;
}

void BassSoundPlayer::StopSong()
{
  if (m_chan == -1)
  {
    return;
  }

#ifdef _DEBUG
std::cout << "BASS: Stopping song on channel " << m_chan << "\n";
#endif
  BASS_ChannelStop(m_chan); 
}

void BassSoundPlayer::Update()
{
}

void BassSoundPlayer::SetSongMaxVolume(float f)
{
  if (m_chan == -1)
  {
    return;
  }

  SoundPlayer::SetSongMaxVolume(f); 
  int newVol = (int)(f * 100.0f);
  BASS_ChannelSetAttributes(m_chan, -1, newVol, -1);
}
}

