
#include "GuiTextEditable.h"

namespace Amju
{

void GuiTextEditable::DrawImpl()
{
  // Draw lines; draw cursor
  GuiText::DrawImpl();
}

void GuiTextEditable::MakeText()
{
  // Word wrap text into lines
}

void GuiTextEditable::OnEnterKey()
{
}

void GuiTextEditable::Insert(char c)
{
  if (m_text.size() > m_maxTextSize)
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

void GuiTextEditable::OnKey(char c)
{
  if (key == '\n' || key == '\r')
  {
    // Enter - start a new line or execute Command for this Element.
    OnEnterKey();
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
  // Make word-wrapped lines from full text.
  MakeText();

}

void GuiTextEditable::OnPlusUp(bool down)
{
  // Multi line text ?
}

void GuiTextEditable::OnPlusDown(bool down)
{
}

void GuiTextEditable::OnPlusLeft(bool down)
{
  if (!down)
  {
    return;
  }
  if (m_pos > 0)
  {
    m_pos--;
    // ?MakeText();
  }
}

void GuiTextEditable::OnPlusRight(bool down)
{
  if (!down)
  {
    return;
  }
  if (m_pos < m_text.size())
  {
    m_pos++;
    // ?MakeText();
  }
}
}

