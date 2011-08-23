/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: SpeechSynth.cpp,v $
Revision 1.1  2004/09/08 15:42:57  jay
Added to repository
  
*/

#include "SpeechSynth.h"

namespace Amju 
{
SingleSpeechSynth::SingleSpeechSynth() : m_pImpl(0)
{
}

void SingleSpeechSynth::SetImpl(SpeechImpl* p)
{
  m_pImpl = p;
}

void SingleSpeechSynth::Say(const char* text)
{
  if (m_pImpl)
  {
    m_pImpl->Say(text);
  }
}

void SingleSpeechSynth::Init()
{
  if (m_pImpl)
  {
    m_pImpl->Init();
  }
}

}

