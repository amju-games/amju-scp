/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: CameraPath.h,v $
Revision 1.1  2004/09/08 15:42:45  jay
Added to repository
  
*/

#if !defined(CAMERA_PATH_H_INCLUDED)
#define CAMERA_PATH_H_INCLUDED

#include "FirstPersonCamera.h"
#include "Function.h"

namespace Amju
{
class File;

// A camera path shows a scene, from a first person view.
// The Orientation changes w.r.t. time, using a Function.
class CameraPath : public FirstPersonCamera
{
public:
  CameraPath();
  ~CameraPath();

  virtual bool Load(File* pf);
  virtual void Draw();
  virtual void Reset();

  void SetFunction(PFunction pFunc) { m_pFunc = pFunc; }
  PFunction GetFunction() { return m_pFunc; }

protected:
  PFunction m_pFunc;
};
}
#endif

