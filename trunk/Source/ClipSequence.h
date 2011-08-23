/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ClipSequence.h,v $
Revision 1.1  2004/09/08 15:42:46  jay
Added to repository
  
*/

#if !defined(CLIP_SEQUENCE_H_INCLUDED)
#define CLIP_SEQUENCE_H_INCLUDED

#include "Clip.h"
#include "SharedPtr.h"
#include <string>
#include <vector>

namespace Amju
{
class File;
class Level;

// A ClipSequence is a sequence of clips, with a soundtrack - a MIDI file.
class ClipSequence : public Shareable
{
public:
  ClipSequence();
  virtual ~ClipSequence();

  virtual bool Load(File* pf);

  virtual bool Start();
  virtual void Stop();

  // Clip Sequences may contain clips which respond to user input.
  virtual void Blue(bool); // e.g. to "ok" a caption
  virtual void Green(bool);
  virtual void Red(bool);
  virtual void JoyX(float); // e.g. Menus use joystick input.
  virtual void JoyY(float);

  void Clear();

  virtual void Draw();
  virtual void DrawOverlays();
 
  bool IsFinished();

  PClip GetCurrentClip();
  Level* GetLevel();

protected:

  std::vector<PClip> m_clips;

  //std::string m_midiFile;
  float m_time; // time from start of clip sequence.
  int m_currentClip; 

  bool m_isFinished;
};

typedef SharedPtr<ClipSequence> PClipSequence;
}
#endif

