/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateEnterText.cpp,v $
Revision 1.1.4.1.4.6  2006/04/27 08:25:18  jay
MAC OS X: Handle '\b' as delete character, needed for native wrapper

Revision 1.1.4.1.4.5  2005/11/08 10:29:13  jay
Handle Paste SpecialEvents

Revision 1.1.4.1.4.4  2005/10/29 10:16:51  jay
Added SpecialEvent handler so user can paste in text.

Revision 1.1.4.1.4.3  2005/10/21 17:03:49  jay
Check menu exists in MousePos

Revision 1.1.4.1.4.2  2005/07/09 05:53:43  jay
Check for valid characters only

Revision 1.1.4.1.4.1  2005/05/24 20:33:33  jay
Enforce max string length

Revision 1.1.4.1  2004/10/04 08:59:14  jay
Crashing bug in ResetText fixed.

Revision 1.1  2004/09/08 15:42:36  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EngineStateEnterText.h"
#include "TextFactory.h"
#include "Engine.h"
#include "TextColourizerColourList.h"
#include "SolidSize.h"
#include "TextMenu.h"
#include "SpecialEventPaste.h"

namespace Amju
{
bool EngineStateEnterText::Load()
{
  m_maxTextSize = (int)Engine::Instance()->GetConfigFloat("enter-text-max");
  if (m_maxTextSize == 0)
  {
    return false;
  }

  if (!m_bg.Load(GetEngine()->GetConfigValue("file_bg"), ""))
  {
    return false;
  }

  std::string cursorName = GetEngine()->GetConfigValue("cursor_solid");
  m_pCursor = SolidComponent::LoadSolid(cursorName);
  if (!m_pCursor.GetPtr())
  {
    return false;
  }

/*
  m_pMenu = new TextMenu;
  std::string menu = Engine::Instance()->GetConfigValue("menu_enter_text");
  if (!m_pMenu->Load(menu))
  {
    return false;
  }

  m_pMenu->SetSelected(0);
*/
  return true;
}

void EngineStateEnterText::ResetText()
{
  m_text = "";
  m_pos = 0;
  MakeText();
}

void EngineStateEnterText::MakeText()
{
  TextColourizerColourList col;
  // TODO CONFIG
  col.AddColour(Colour(1.0f, 1.0f, 1.0f, 1.0f));
  col.AddColour(Colour(0.2f, 0.2f, 1.0f, 1.0f));

  std::string s = m_text.substr(0, m_pos);
  m_pComp = TextFactory::Instance()->Create(s, &col, false);
  m_leftSize = GetSolidXSize(m_pComp.GetPtr());
  m_totalSize = m_leftSize;

  s = m_text.substr(m_pos);
  m_pComp2 = TextFactory::Instance()->Create(s, &col, false);
  m_totalSize += GetSolidXSize(m_pComp2.GetPtr());
}

void EngineStateEnterText::SetActive(bool b)
{
  if (b)
  {
    m_lighting.Init();
    m_text = "";
    m_pos = 0;
    MakeText();
    m_mouseDown = false;
  }
}

void EngineStateEnterText::DrawOverlays()
{
  m_greet.Draw();
}

void EngineStateEnterText::DrawCursor()
{
  static float a = 1.0f;
  a -= Engine::Instance()->GetDeltaTime();
  if (a < 0)
  {
    a = 1.0f;
  }
  Engine::Instance()->PushColour(1.0f, 1.0f, 1.0f, a);
  m_pCursor->Draw();
  Engine::Instance()->PopColour();
}

void EngineStateEnterText::Draw()
{
  AmjuGL::PushMatrix();
  DrawBg();

  m_pCam->Update();
  m_pCam->Draw();

  m_lighting.DrawLight();

  m_or.Draw();
  
  AmjuGL::PushMatrix();
  // Draw the text being edited:
  AmjuGL::Translate(-m_totalSize/2.0f, 0, -m_totalSize); 
  // Draw the text to the left of the cursor
  m_pComp->Draw();
  // Draw the text to the right of the cursor
  AmjuGL::Translate(m_leftSize + 0.5f, 0, 0);
  DrawCursor();
  m_pComp2->Draw();
  AmjuGL::PopMatrix();

  // Draw Menu
  // TODO CONFIG
  AmjuGL::Translate(0, -4.0f, -8.0f); // menu below text
  m_pMenu->Draw();

  AmjuGL::PopMatrix();

}

void EngineStateEnterText::Insert(char c)
{
  if ((int)m_text.size() > m_maxTextSize)
  {
    AlertMaxTextSize();
    return;
  }

  std::string s = m_text.substr(0, m_pos);
  s += std::string(1, c);
  s += m_text.substr(m_pos);

  m_text = s;

  ++m_pos;
}

void EngineStateEnterText::PlusLeft(bool down)
{
  if (!down)
  {
    return;
  }
  if (m_pos > 0)
  {
    m_pos--;
    MakeText();
  }
}

void EngineStateEnterText::PlusRight(bool down)
{
  if (!down)
  {
    return;
  }
  if (m_pos < (int)m_text.size())
  {
    m_pos++;
    MakeText();
  }
}

bool IsAlnum(char c)
{
  return ((c >= '0' && c <= '9') || 
          (c >= 'a' && c <= 'z') || 
          (c >= 'A' && c <= 'Z'));
}

void EngineStateEnterText::OnSpecialEvent(SpecialEvent* p)
{
  // Handle paste event.
  // NB Could do this with a double-dispatch, i.e. map special events
  // to States which can handle them...?
  // Is there a way to do this with no dynamic_cast ???
  SpecialEventPaste* pSep = dynamic_cast<SpecialEventPaste*>(p);
  if (pSep)
  {
#ifdef _DEBUG
std::cout << "Enter text: got a paste event: " << pSep->GetText().c_str() << "\n";
#endif

    std::string s = pSep->GetText();
    // Add each character
    for (int i = 0; i < (int)s.size(); i++)
    {
      char c = s[i];
      Insert(c); // may fail if m_text too long etc.
    }
    MakeText();
  }
}

void EngineStateEnterText::OnKey(char key, bool down)
{
  if (!down)
  {
    return;
  }

  if (key == '\n' || key == '\r')
  {
    // Enter - go to next state.
    // But Red() does nothing because the Space bar is mapped to this
    // function normally.
    Finished();
    return;
  }

  // TODO cursor pos

  if (IsAlnum(key) || key == ' ')
  {
    Insert(key);
    // Not here - Insert() may fail -- m_pos++;
  }

  // delete key
#ifdef MACOSX
  if (key == (char)127 || key == '\b')
#else
  if (key == '\b') // backspace
#endif
  {
    if (m_pos > 0)
    {
      std::string s = m_text.substr(0, m_pos - 1);
      s += m_text.substr(m_pos);
      m_text = s;
      m_pos--;
    }
  }
  MakeText();
}

void EngineStateEnterText::MousePos(int x, int y)
{
  if (m_pMenu.GetPtr())
  {
    m_pMenu->MousePos(x, y);
  }
}

void EngineStateEnterText::MouseButton(bool down, bool ctrl, bool shift)
{
  m_pMenu->MouseButton(down, ctrl, shift);

  if (down)
  {
    m_mouseDown = true;
  }
  
  if (m_mouseDown && !down)
  {
    std::cout << "Selecting menu item.\n";
/*
    int c = m_pMenu->Choose();
    if (c == 0)
    {
      // Ok
      Finished();
    }
    else if (c == 1)
    {
      Back();
    }
*/
    // The selected text is written to the editable text.
    m_text = "chosen text"; // TODO
    m_pos = m_text.size(); // TODO off by one or ok ?
    MakeText();
  }

  if (!down)
  {
    m_mouseDown = false;
  }
}
}
