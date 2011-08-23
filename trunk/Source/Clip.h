/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Clip.h,v $
Revision 1.1  2004/09/08 15:42:46  jay
Added to repository
  
*/

#if !defined(CLIP_H_INCLUDED)
#define CLIP_H_INCLUDED

#include "SharedPtr.h"
#include "CameraPath.h"

namespace Amju
{
class File;
class Level;

// A Clip is like a movie clip - a sequence of frames, of a set length.
// aka a Cut Scene.
class Clip : public Shareable
{
public:
  Clip() : m_pCameraPath(0) {}
  virtual ~Clip() {}

  void SetStartTime(float secs) { m_startTime = secs; }
  void SetEndTime(float secs) { m_endTime = secs; }

  // Call ONCE per game loop to update timer, etc.
  void Update();

  // Return true if this clip is showing now.
  // Override this if the Active state is not time-based, e.g. depends on a
  // keypress etc.
  virtual bool IsActive();

  virtual void Draw() = 0;
 
  virtual void DrawOverlays() {}

  virtual bool Load(File* pf) = 0;

  virtual Level* GetLevel() = 0;

  virtual SharedPtr<CameraPath> GetCameraPath() { return m_pCameraPath; }

  virtual void Start() { SetElapsedTime(0); }
  virtual void Stop();

  virtual void Red(bool) {}
  virtual void Green(bool) {}
  virtual void Blue(bool) {}
  virtual void JoyX(float) {}
  virtual void JoyY(float) {}

protected:
  void SetElapsedTime(float t) { m_elapsedTime = t; }

  float m_startTime;
  float m_endTime;
  float m_elapsedTime;

  // All clips must have a camera path. 
  // TODO But this could be any Camera.
  SharedPtr<CameraPath> m_pCameraPath;
};

typedef SharedPtr<Clip> PClip;
}
#endif

