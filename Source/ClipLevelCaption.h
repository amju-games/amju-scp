/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ClipLevelCaption.h,v $
Revision 1.1  2004/09/08 15:42:46  jay
Added to repository
  
*/

#if !defined(CLIP_LEVEL_CAPTION_H_INCLUDED)
#define CLIP_LEVEL_CAPTION_H_INCLUDED


#include "ClipLevel.h"

namespace Amju
{
class CaptionBox;

// ClipLevelCaption
// A Clip (part of a cut scene), showing a level and a Caption Box.
// The Clip ends when the user dismisses the Caption Box.
class ClipLevelCaption : public ClipLevel
{
public:
  ClipLevelCaption();

  virtual void DrawOverlays();
  virtual bool IsActive();
  virtual bool Load(File* pf);
  virtual void Start();

  virtual void Red(bool);
  virtual void Green(bool);
  virtual void Blue(bool);
  virtual void JoyX(float);
  virtual void JoyY(float);

protected:
  CaptionBox* m_pCaptionBox;
  bool m_isActive;
};
}
#endif

