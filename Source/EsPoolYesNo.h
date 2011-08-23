/*
Amju Games source code (c) Copyright Jason Colman 2007
*/

#ifndef ES_POOL_YES_NO_H_INCLUDED
#define ES_POOL_YES_NO_H_INCLUDED

#include "EngineStateText.h"
#include "GuiButton.h"

namespace Amju
{
// Yes/no question: base class
class EsPoolYesNo : public EngineStateText
{
public:
  EsPoolYesNo();
  virtual void SetActive(bool);
  virtual bool Load();
  virtual void Update();
  virtual void Draw();
  virtual void DrawOverlays();
  virtual void MouseButton(bool, bool, bool);

  virtual void OnYes() = 0;
  virtual void OnNo() = 0;

  void AddQ(const std::string& s);

protected:
  PGuiElement m_pYesButton;
  PGuiElement m_pNoButton;
  std::string m_yesFilename;
  std::string m_noFilename;
  float m_firstLineY;

  typedef std::vector<std::string> Strings;
  Strings m_question;
};
}


#endif
