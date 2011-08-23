/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: KeymapPrinter.cpp,v $
Revision 1.1  2004/09/08 15:42:51  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "KeymapPrinter.h"
#include "Keymap.h"
#include "Engine.h"
#include "TextWriter.h"
#include "Common.h"
#include "File.h"
#include "ExplosionEffect.h"
#include <string>

using namespace std;

namespace Amju
{
extern void ReportError(const std::string&);

bool KeymapPrinter::Init()
{
  // Load keyboard key quad
  if (!m_buttonQuad.Load(Engine::Instance()->GetConfigValue("button"), 
                         Engine::Instance()->GetConfigValue("button_alpha")))
  {
    Engine::Instance()->ReportError("Couldn't load button icon.");
    return false;
  }

  // Load joystick button quads: one for button A, one for button B
  if (!m_joyAQuad.Load(Engine::Instance()->GetConfigValue("joy_a"), 
                       Engine::Instance()->GetConfigValue("joy_a_alpha")))
  {
    Engine::Instance()->ReportError("Couldn't load joystick button A icon.");
    return false;
  }

  if (!m_joyBQuad.Load(Engine::Instance()->GetConfigValue("joy_b"), 
                       Engine::Instance()->GetConfigValue("joy_b_alpha")))
  {
    Engine::Instance()->ReportError("Couldn't load joystick button B icon.");
    return false;
  }


  // Load text. We expect three lines of text: "ACTION", "VIEW" and "Hit Space for MENU".
  string lang = Engine::Instance()->GetConfigValue("language");
  string dash = "-"; //Engine::Instance()->GetConfigValue("slash");
  string filename = Engine::Instance()->GetConfigValue("keys_text");
  string langFileName = lang + dash + filename;
  File f;
  if (!f.OpenRead(langFileName))
  {
    Engine::Instance()->ReportError("Couldn't open keys text file.");
    return false;
  }
  if (!m_textAction.Load(&f))
  {
    Engine::Instance()->ReportError("Couldn't load text for action key.");
    return false;
  }
  if (!m_textView.Load(&f))
  {
    Engine::Instance()->ReportError("Couldn't load text for view key.");
    return false;
  }
  if (!m_textMenu.Load(&f))
  {
    Engine::Instance()->ReportError("Couldn't load text for menu key.");
    return false;
  }
  if (!m_joyTextAction.Load(&f))
  {
    Engine::Instance()->ReportError("Couldn't load text for j/s Action.");
    return false;
  }
  if (!m_joyTextView.Load(&f))
  {
    Engine::Instance()->ReportError("Couldn't load text for j/s View.");
    return false;
  }

  // "Explosion" effect for when a key mapping changes
  std::string solidName = Engine::Instance()->GetConfigValue("key_particle");
  // Load the explosion effect
  PSolidComponent pBb = SolidComponent::LoadSolid(solidName);
  if (!pBb.GetPtr())
  {
    ReportError("Bad particle name for keymap.");
    return false;
  }

  // TODO CONFIG num particles, max vel/time 
  int numParticles = (int)Engine::Instance()->GetConfigFloat("key_num_particles");
  if (numParticles == 0)
  {
    return false;
  }
  ExplosionEffect* pEffect = new ExplosionEffect;
  pEffect->Init(numParticles, pBb, 10.0f, 10.0f);
  SetEffect(pEffect);

  return true;
}

void KeymapPrinter::SetEffect(Effect* p)
{
  m_pEffect = p;
}

void KeymapPrinter::DrawOneKey(
  char k, // key character 
  float r, float g, float b, // key colour (R, G, B)
  bool black, // if true, letter colour is black, else white
  float x, float y, // position of key
  float size) // size of key
{
  Engine::Instance()->PushColour(r, g, b, 1);
  m_buttonQuad.Draw(y, x, y + size, x + size); // t,l,b,r
  Engine::Instance()->PopColour();

  if (black)
  {
    Engine::Instance()->PushColour(0, 0, 0, 1);
  }
  else
  {
    Engine::Instance()->PushColour(1, 1, 1, 1);
  }
  char buf[10];
  sprintf(buf, "%c", k);

  // Centre the letter on the key.
  x += 0.25f - Engine::Instance()->GetTextWriter()->GetCharacterWidth(k) * 0.5f;

  Engine::Instance()->GetTextWriter()->PrintNoBlend(x, y, buf);
  Engine::Instance()->PopColour();
}

void KeymapPrinter::Draw(float x, float y)
{
  switch (m_mode)
  {
  case JOYSTICK:
    DrawJoystick(x + 1.5f, y);
    break;
  case KEYS:
    DrawKeyboard(x, y);
    break;
  }

  m_pEffect->Draw();
}

void KeymapPrinter::DrawJoystick(float x, float y)
{
  const float s = 1.5f;
  m_joyAQuad.Draw(y, x, y + s, x + s);
  m_joyBQuad.Draw(y + s, x + s, y + s + s, x + s + s);
  m_joyTextAction.Draw();
  m_joyTextView.Draw();
}

void KeymapPrinter::DrawKeyboard(float x, float y)
{
  // Draw picture of game buttons. Write the keyboard letter on top of each one.
  const float grey = 1.0f;
  const float s = 1.0f; // key size
  Keymap* km = Engine::Instance()->GetKeymap();

  // Plus pad
  DrawOneKey(km->GetChar(Keymap::PLUS_UP), grey, grey, grey, true, x + s * 2.5, y, s);
  DrawOneKey(km->GetChar(Keymap::PLUS_DOWN), grey, grey, grey, true, x + s * 2.5, y + s * 2, s);
  DrawOneKey(km->GetChar(Keymap::PLUS_LEFT), grey, grey, grey, true, x + s * 1.5, y + s, s);
  DrawOneKey(km->GetChar(Keymap::PLUS_RIGHT), grey, grey, grey, true, x + s * 3.5, y + s, s);
  // Z
  DrawOneKey(km->GetChar(Keymap::Z), 1, 0.2, 0.2, false, x, y + s, s);
  // Green
  //DrawOneKey(km->GetChar(Keymap::GREEN), 0, 1.0f, 0, false, x + s * 3.5, y, s);
  //Blue
  DrawOneKey(km->GetChar(Keymap::BLUE), 0.2, 0.2, 1.0f, false, x, y, s);
  // Red
  //DrawOneKey(km->GetChar(Keymap::RED), 1.0f, 0, 0, false, x + s * 3.5, y + s * 2, s);
  // Yellow up
  //DrawOneKey(km->GetChar(Keymap::YELLOW_UP), 1.0f, 1.0f, 0, true, x + s * 5.5, y, s);

  m_textAction.Draw();
  m_textView.Draw();
  
  AmjuGL::PushMatrix();
  AmjuGL::Scale(0.7f, 0.7f, 0.7f);
  m_textMenu.Draw();
  AmjuGL::PopMatrix();

  // Get character for each button. If a character for a button has changed,
  // show an effect.
  static char oldPlusUp = km->GetChar(Keymap::PLUS_UP);
  static char newPlusUp;
  newPlusUp= km->GetChar(Keymap::PLUS_UP);
  if (newPlusUp != oldPlusUp)
  {
    ShowEffect(newPlusUp);
    oldPlusUp = newPlusUp;
  }

  static char oldPlusDown = km->GetChar(Keymap::PLUS_DOWN);
  static char newPlusDown;
  newPlusDown = km->GetChar(Keymap::PLUS_DOWN);
  if (newPlusDown != oldPlusDown)
  {
    ShowEffect(newPlusDown);
    oldPlusDown = newPlusDown;
  }

  static char oldPlusLeft = km->GetChar(Keymap::PLUS_LEFT);
  static char newPlusLeft;
  newPlusLeft = km->GetChar(Keymap::PLUS_LEFT);
  if (newPlusLeft != oldPlusLeft)
  {
    ShowEffect(newPlusLeft);
    oldPlusLeft = newPlusLeft;
  }

  static char oldPlusRight = km->GetChar(Keymap::PLUS_RIGHT);
  static char newPlusRight;
  newPlusRight = km->GetChar(Keymap::PLUS_RIGHT);
  if (newPlusRight != oldPlusRight)
  {
    ShowEffect(newPlusRight);
    oldPlusRight = newPlusRight;
  }

  static char oldBlue = km->GetChar(Keymap::BLUE);
  static char newBlue;
  newBlue = km->GetChar(Keymap::BLUE);
  if (newBlue != oldBlue)
  {
    ShowEffect(newBlue);
    oldBlue = newBlue;
  }

  static char oldZ = km->GetChar(Keymap::Z);
  static char newZ;
  newZ = km->GetChar(Keymap::Z);
  if (newZ != oldZ)
  {
    ShowEffect(newZ);
    oldZ = newZ;
  }
}

void KeymapPrinter::ShowEffect(char c)
{
  // Work out the key with character c on it.
  // Then get the coords for that key.
  m_pEffect->Reset(VertexBase(0, 4.0f, -4.0f));// TODO CONFIG
}

void KeymapPrinter::SetMode(Mode m)
{
  m_mode = m;
}

KeymapPrinter::Mode KeymapPrinter::GetMode() const
{
  return m_mode;
}

}
