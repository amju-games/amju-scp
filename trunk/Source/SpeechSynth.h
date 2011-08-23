/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: SpeechSynth.h,v $
Revision 1.1  2004/09/08 15:42:57  jay
Added to repository
  
*/

#if !defined(SPEECH_SYNTH_H_INCLUDED)
#define SPEECH_SYNTH_H_INCLUDED

#include "Singleton.h"

namespace Amju
{
class SpeechImpl
{
public:
  virtual ~SpeechImpl() {}
  virtual void Init() = 0;
  virtual void Say(const char*) = 0;
};

class SingleSpeechSynth
{
public:
  SingleSpeechSynth();

  void SetImpl(SpeechImpl* );
  void Init();
  void Say(const char* text);

protected:
  SpeechImpl* m_pImpl;
};
typedef Singleton<SingleSpeechSynth> SpeechSynth;
}

#endif


