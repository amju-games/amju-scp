/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Lighting.h,v $
Revision 1.1  2004/09/08 15:42:52  jay
Added to repository
  
*/

#if !defined(LIGHTING_H_INCLUDED)
#define LIGHTING_H_INCLUDED

#include <string>

namespace Amju
{
// Performs lighting required for a Room. 
class Lighting
{
public:
  Lighting();

  // Load from a file. Returns true if loading was successful.
  bool Load(const std::string& filename);
 
  // Initialise lighting. Called when Room is entered.
  void Init();

  // Draw the light for one frame. 
  // For the light position to remain constant w.r.t. the scene, call this
  // after setting up the camera. 
  void DrawLight();

  void SetPosition(float x, float y, float z);

  void SetGlobalAmbient(float r, float g, float b, float a);

protected:
  float m_globalAmbient[4];
  // These params are stored for every light.
  float m_position[4], m_ambient[4], m_diffuse[4], m_specular[4]; 
};
}
#endif



