/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ClipSequence.cpp,v $
Revision 1.1  2004/09/08 15:42:46  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "ClipSequence.h"
#include "Clip.h"
#include "Engine.h" // We use the engine to play sound.
#include "File.h"
#include "ClipFactory.h"
#include "SchAssert.h"

using namespace std;

namespace Amju
{
ClipSequence::ClipSequence()
{
  m_time = 0;
  m_currentClip = 0;
}

ClipSequence::~ClipSequence()
{
  Clear();
}

bool ClipSequence::Load(File* pf)
{
  Clear();

  // Get the MIDI filename.
  string midifile;
  if (!pf->GetDataLine(&midifile))
  {
    pf->ReportError("Expected MIDI filename.");
    return false;
  }

  // Get number of clips in this sequence
  int numClips = 0;
  if (!pf->GetInteger(&numClips))
  {
    pf->ReportError("Expected number of clips in sequence.");
    return false;
  }

  for (int i = 0; i < numClips; i++)
  {
    // Get type of this clip
    string clipTypeName;
    if (!pf->GetDataLine(&clipTypeName))
    {
      pf->ReportError("Expected clip type name.");
      return false;
    }
    // Create a new clip object from the type name.
    Clip* pClip = ClipFactory::CreateClip(clipTypeName);
    if (!pClip)
    {
      pf->ReportError("Bad clip type name.");
      return false;
    }

    if (!pClip->Load(pf))
    {
      delete pClip;
      return false;
    }
    m_clips.push_back(pClip);
  }

  return true;
}

bool ClipSequence::Start()
{
  m_time = 0;
  m_currentClip = 0;
  m_isFinished = false;

  Assert(m_clips.size() > 0);
  Assert(m_clips[0].GetPtr());

  if (m_clips.size() == 0 || !m_clips[0].GetPtr())
  { 
    return false;
  }

  m_clips[0]->Start();
  
  /*
  if (!Engine::Instance()->PlaySong(m_midiFile))
  {
    // Don't let MIDI failure stop everything else.
    //return false;
  }
  */

  return true;
}

void ClipSequence::Stop()
{
  //Engine::Instance()->FinishCutScene(); 
}

void ClipSequence::Draw()
{
  if (m_clips.empty())
  {
    return;
  }

  PClip pClip = m_clips[m_currentClip];
  if (!pClip.GetPtr())
  {
    return;
  }

  pClip->Update();

  if (IsFinished())
  {
    // Stop animating it.
    //pClip->Draw();
    return;
  }

  pClip->Draw();
}

void ClipSequence::DrawOverlays()
{
  GetCurrentClip()->DrawOverlays();
}

void ClipSequence::Clear()
{
  m_clips.clear();
}

PClip ClipSequence::GetCurrentClip()
{
  return m_clips[m_currentClip];
}

Level* ClipSequence::GetLevel()
{
  return GetCurrentClip()->GetLevel();
}

bool ClipSequence::IsFinished()
{
  PClip pClip = m_clips[m_currentClip];

  if (!pClip.GetPtr())
  {
    return false;
  }

  if (!pClip->IsActive())
  {
    // Current clip is no longer active - go on to the next one. 
    m_time = 0;
    m_currentClip++;
    if ((unsigned int)m_currentClip >= m_clips.size())
    {
      // We've shown all the clips. 
      m_isFinished = true;
      m_currentClip--;
      //m_currentClip = 0;
    }
    else
    {
      m_clips[m_currentClip]->Start();
    }
  }

  return m_isFinished;
}

void ClipSequence::Blue(bool down)
{
  GetCurrentClip()->Blue(down);
}

void ClipSequence::Green(bool down)
{
  GetCurrentClip()->Green(down);
}

void ClipSequence::Red(bool down)
{
  GetCurrentClip()->Red(down);
}

void ClipSequence::JoyX(float f)
{
  GetCurrentClip()->JoyX(f);
}

void ClipSequence::JoyY(float f)
{
  GetCurrentClip()->JoyY(f);
}
}


