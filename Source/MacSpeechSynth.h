/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: MacSpeechSynth.h,v $
Revision 1.1  2004/09/08 15:43:08  jay
Added to repository
  
*/

#if !defined(MAC_SPEECH_SYNTH_H_INCLUDED)
#define MAC_SPEECH_SYNTH_H_INCLUDED

#include "SpeechSynth.h"

namespace Amju
{
class MacSpeechSynth : public SpeechImpl
{
public:
  virtual void Init();
  virtual void Say(const char *);
};
}

#endif
