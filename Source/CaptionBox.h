/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: CaptionBox.h,v $
Revision 1.1  2004/09/08 15:42:45  jay
Added to repository
  
*/

#if !defined(CAPTION_BOX_H_INCLUDED)
#define CAPTION_BOX_H_INCLUDED

#include <string>

namespace Amju
{
class File;

// A rectangular area containing text.
class CaptionBox
{
public:
  CaptionBox();
  virtual ~CaptionBox();

  virtual void Draw();
  virtual bool Load(File* pf);

  virtual void Red(bool);
  virtual void Green(bool);
  virtual void Blue(bool);
  virtual void JoyX(float);
  virtual void JoyY(float);

  void SetActive(bool active) { m_isActive = active; }
  bool IsActive() { return m_isActive; }

protected:
  bool LoadSize(File* pf);

  bool m_isActive;
  // Top left position, width and height of caption box.
  int m_x, m_y, m_w, m_h;

  std::string m_text; // TODO a list of text sections. Line separators etc.
};
}
#endif

