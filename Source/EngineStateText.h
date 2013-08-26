/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateText.h,v $
Revision 1.1  2004/09/08 15:42:49  jay
Added to repository
  
*/

#if !defined(ENGINE_STATE_TEXT_H_INCLUDED)
#define ENGINE_STATE_TEXT_H_INCLUDED

#include <map>
#include "EngineState.h"
#include "TexturedQuad.h"
#include "SolidComponent.h"
#include "Camera.h"
#include "Function.h"
#include "Lighting.h"

namespace Amju
{
// Base class for States which can show some 3D Text. In 
// fact the "text" is just a SolidComponent, so could be anything.
// Subclasses must define/load the Camera, function controlling
// any movement of the text, background texture, etc.
class EngineStateText : public EngineState
{
public:
  EngineStateText();

  virtual void Update();
  virtual void Draw();
  virtual void DrawOverlays() {}
  virtual void SetActive(bool active);

  // Called when the time for the State has elapsed.
  virtual void TimerExpired() {}

  virtual void SetText(const char*);

protected:
  // Paint Textured Quad background
  void DrawBg();

  void DrawText(const char* text, float x, float y, float size = 1.0f);

protected:
  // Letters
  PSolidComponent m_pComp;

  // Background texture
  TexturedQuad m_bg;

  // Camera
  RCPtr<Camera> m_pCam;

  // Function which controls movement of the text.
  PFunction m_pFunc;

  // Current orientation of text.
  Orientation m_or; 

  // Translation in y-axis, to centre the text.
  float m_verticalPos;

  Lighting m_lighting;

  typedef std::map<const char*, PSolidComponent> TextMap;
  TextMap m_textmap;
};
}

#endif


