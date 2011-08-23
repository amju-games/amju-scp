/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateText.cpp,v $
Revision 1.1  2004/09/08 15:42:49  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Common.h"
#include "EngineStateText.h"
#include "Engine.h"
#include "FirstPersonCamera.h"
#include "FunctionFactory.h"
#include "TextFactory.h"
#include "SolidComponent.h"
#include "SolidComposite.h"

// Undefine stupid windows macro
#if defined(WIN32)
#if defined(DrawText)
#undef DrawText
#endif
#endif

namespace Amju
{
EngineStateText::EngineStateText()
{
  m_pCam = new FirstPersonCamera;  
  m_pCam->SetOrientation(Orientation(0, 0, 8, 0, 180, 0));

  m_verticalPos = 1.2f; // for 2 lines of text
}

void EngineStateText::DrawText(const char* text, float x, float y, float size)
{
  PSolidComponent pText;
  TextMap::iterator it = m_textmap.find(text);
  if (it == m_textmap.end())
  {
    // Create the text solid
    pText = TextFactory::Instance()->Create(text);
    m_textmap[text] = pText;
  }
  else
  {
    pText = it->second; 
  }

  pText->Draw();
}

void EngineStateText::SetText(const char* text)
{
  // TODO word wrap the text if necessary.
  // Split the text into multiple lines. Create each line of text separately.
  // Join the lines into a Composite.
  // Get the max extent of the text.

  std::vector<std::string> choppedText;
  choppedText.push_back(text); // TODO

  SolidComposite* pComp = new SolidComposite;

  float z = -(float(choppedText.size())) / 2.0f;
  float r = 0;

  for (unsigned int i = 0; i < choppedText.size(); i++)
  {
    const std::string& thisLine = choppedText[i];
    if ((float)thisLine.size() > r)
    {
      r = (float)thisLine.size();
    }

    PSolidComponent lineComp = TextFactory::Instance()->Create(thisLine.c_str());
    lineComp->GetOrientation()->SetZ(z);
    
    pComp->AddComponent(lineComp);

    z += 1.0f;
  }

  m_pComp = pComp; 

  // Adjust the camera so all the text fits on the screen.
  // r is the width of the line of text.
  // Adjust the camera until it fits in the view frustum.
  float dist = 2 + r * 1.0f; // TODO TEMP TEST
  m_pCam->SetOrientation(Orientation(0, 0, dist, 0, 180, 0));

}

void EngineStateText::SetActive(bool active)
{
  m_time = 0;

  if (active)
  {
    GetEngine()->SetLetterbox(true); 

    m_lighting.Init();
  }
  else
  {
    GetEngine()->SetLetterbox(false);
  }
}

void EngineStateText::DrawBg()
{
  // Draw background - don't write to the z buffer.
  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_WRITE);
  m_bg.Draw(-3, -3, 19, 28); // t, l, b, r
  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_WRITE);
}

void EngineStateText::Draw()
{
  AmjuGL::PushMatrix();
  DrawBg();

  // Do camera work
  m_pCam->Update();
  m_pCam->Draw();

  // Lighting
  m_lighting.DrawLight();

  // Position the text
  m_or.Draw();
  // Draw the text
  if (m_pComp.GetPtr())
  {
    AmjuGL::PushMatrix();
    // position
    AmjuGL::Translate(0, m_verticalPos, 0);
    m_pComp->Draw();
    AmjuGL::PopMatrix();
  }
  AmjuGL::PopMatrix();

  m_time += GetEngine()->GetDeltaTime();
  if (m_time > m_maxTime)
  {
    TimerExpired();
  }
}

}

