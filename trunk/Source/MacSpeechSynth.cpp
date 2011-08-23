/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: MacSpeechSynth.cpp,v $
Revision 1.1  2004/09/08 15:43:08  jay
Added to repository
  
*/

#include "MacSpeechSynth.h"
#include "Engine.h"
// mac-specific includes
#include <ApplicationServices/ApplicationServices.h>

namespace Amju
{
void MacSpeechSynth::Init()
{
  // TODO This is probably required to set up a specific voice.
/*
  // Set up a voice
  VoiceSpec * voice = 0; // TODO

  // Create a speech channel
  SpeechChannel chan;
  OSErr res = NewSpeechChannel(voice, &chan);
*/ 
}

void MacSpeechSynth::Say(const char * text)
{
  static bool speech = (Engine::Instance()->GetConfigValue("speech") == "y");
  if (!speech)
  {
    return;
  }
  
  // Enforce a time delay between separate speech calls. 
  static float timer = 3.0f;
  timer += Engine::Instance()->GetDeltaTime();
  if (timer < 2.0f)
  {
    return;
  }
  timer = 0;
  
  // Make a "pascal-style" string. By trial and error I found that this means with an 8-bit
  // string length at the start.
  static char buf[500];
  short len = strlen(text);
  buf[0] = len & 0x00ff;
  //buf[1] = (len & 0xff00) >> 8;
  strcpy(&buf[1], text);
  OSErr res = SpeakString((const unsigned char*)buf);//"Test.");//text);
}
}
