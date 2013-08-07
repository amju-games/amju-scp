/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Guage.h,v $
Revision 1.1  2004/09/08 15:42:51  jay
Added to repository
  
*/

#if !defined(GUAGE_H_INCLUDED)
#define GUAGE_H_INCLUDED

#include "TexturedQuad.h"
#include "RCPtr.h"

namespace Amju
{
class File;

// An on-screen indicator, showing e.g. time or power running out,
// fuel left, speed, progress, etc. etc.
class Guage : public RefCounted
{
public:
  Guage();
  virtual ~Guage();

  // Update and draw the guage
  virtual void Draw() = 0;

  virtual void Update() {}

  // Load the textured quads. Subclasses can call this then load any
  // extra stuff.
  virtual bool Load(File* );
  
  // Set guage value.
  void Set(float val) { m_val = val; }

  float Get() const { return m_val; }

protected:
  // Two textured quads are needed for this. One is the background,
  // e.g. a blank speedo. The second is the value indicator, e.g.
  // a speedo needle.
  // Code in subclasses must arrange the two textured quads, e.g. in
  // this case, drawing the needle in the right place.
  TexturedQuad m_backQuad;
  TexturedQuad m_foreQuad;

  // The value the guage is displaying.
  float m_val;
  // Coords for guage.
  float m_top, m_left, m_bottom, m_right;
};
}

#endif

