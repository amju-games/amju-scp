/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Lighting.cpp,v $
Revision 1.1  2004/09/08 15:42:52  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Lighting.h"
#include "Common.h"
#include "File.h"

using namespace std;

namespace Amju
{
Lighting::Lighting()
{
  // Default values we can live with
  SetGlobalAmbient(0.9f, 0.6f, 0.6f, 1.0f);
  SetPosition(1.0f, 1.0f, 1.0f);

  m_ambient[0] = 0;
  m_ambient[1] = 0;
  m_ambient[2] = 0;
  m_ambient[3] = 1.0f;

  m_diffuse[0] = 1.0f;
  m_diffuse[1] = 1.0f;
  m_diffuse[2] = 1.0f;
  m_diffuse[3] = 1.0f;

  m_specular[0] = 1.0f;
  m_specular[1] = 1.0f;
  m_specular[2] = 1.0f;
  m_specular[3] = 1.0f;
}

void Lighting::SetPosition(float x, float y, float z)
{
  // Set light position x, y, z: 4th coord value is always zero, meaning
  // directional lightsource.
  m_position[0] = x;
  m_position[1] = y;
  m_position[2] = z;
  m_position[3] = 0;
}

void Lighting::SetGlobalAmbient(float r, float g, float b, float a)
{
  m_globalAmbient[0] = r;
  m_globalAmbient[1] = g;
  m_globalAmbient[2] = b;
  m_globalAmbient[3] = a;
}

bool Lighting::Load(const std::string& filename)
{
  File f;
  if (!f.OpenRead(filename))
  {
    f.ReportError("Couldn't open lighting file.");
    return false;
  }
  // Get the global ambient light.
  if (!f.GetFloat(&m_globalAmbient[0]) || 
      !f.GetFloat(&m_globalAmbient[1]) || 
      !f.GetFloat(&m_globalAmbient[2]) || 
      !f.GetFloat(&m_globalAmbient[3]) ) 
  {
    f.ReportError("Failed to get global ambient light.");
    return false;
  }

  // TODO Get the number of lights.
  // Currently only one light.

  // Get light position x, y, z: 4th coord value is always zero, meaning
  // directional lightsource.
  if (!f.GetFloat(&m_position[0]) ||
      !f.GetFloat(&m_position[1]) ||
      !f.GetFloat(&m_position[2]) )
  {
    f.ReportError("Failed to get light position.");
    return false;
  }
  m_position[3] = 0;

  // Get light ambient value
  if (!f.GetFloat(&m_ambient[0]) ||
      !f.GetFloat(&m_ambient[1]) ||
      !f.GetFloat(&m_ambient[2]) ||
      !f.GetFloat(&m_ambient[3]))
  {
    f.ReportError("Failed to get ambient light value.");
    return false;
  }

  // Get diffuse
  if (!f.GetFloat(&m_diffuse[0]) ||
      !f.GetFloat(&m_diffuse[1]) ||
      !f.GetFloat(&m_diffuse[2]) ||
      !f.GetFloat(&m_diffuse[3]))
  {
    f.ReportError("Failed to get diffuse light value.");
    return false;
  }

  // Get specular
  if (!f.GetFloat(&m_specular[0]) ||
      !f.GetFloat(&m_specular[1]) ||
      !f.GetFloat(&m_specular[2]) ||
      !f.GetFloat(&m_specular[3]))
  {
    f.ReportError("Failed to get specular light value.");
    return false;
  }
  return true;
}
 
void Lighting::Init()
{
  // Set the global ambient.
  //glLightModelfv(GL_LIGHT_MODEL_AMBIENT, m_globalAmbient);

  // TODO Set each defined light.
  // Disable all other lights.

  // Light 0: set parameters.
  //glLightfv(GL_LIGHT0, GL_AMBIENT, m_ambient);
  //glLightfv(GL_LIGHT0, GL_DIFFUSE, m_diffuse);	
  //glLightfv(GL_LIGHT0, GL_SPECULAR, m_specular);

  //AmjuGL::Enable(GL_LIGHT0);
  //AmjuGL::Enable(AmjuGL::AMJU_LIGHTING);
  //AmjuGL::Disable(AmjuGL::AMJU_LIGHTING); // NB lighting is Enabled or Disabled in Engine set up.

  // Use DrawLight() to set the position after setting up the camera,
  // to keep the light source contant w.r.t. the scenery.
}

void Lighting::DrawLight()
{
  float amb = 0.7f;

  AmjuGL::DrawLighting(
    AmjuGL::LightColour(0, 0, 0),
    AmjuGL::LightColour(amb, amb, amb), // Ambient light colour
    AmjuGL::LightColour(1, 1, 1), // Diffuse light colour
    AmjuGL::LightColour(1, 1, 1),
    AmjuGL::Vec3(1, 1, 1)); // Light direction

}
}
