/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Earthquake.h,v $
Revision 1.1  2004/09/08 15:42:47  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_EARTHQUAKE_H_INCLUDED)
#define SCHMICKEN_EARTHQUAKE_H_INCLUDED

namespace Amju
{
// Used for Earthquake and explosions. 
// Jitters the camera randomly for some time, with activity
// decreasing with time.
class Earthquake
{
public:

  Earthquake();

  // Draw the earthquake: i.e. jitter the camera.
  void Draw();

  // Force inactive
  void Reset(); 
  
  // Sets the earthquake to active, and to last for the given time period.
  // Activity starts at maximum, and decreases to zero at the end of the
  // period.
  void SetActive(float activeSeconds, float severity = 1.0f);

  bool IsActive();

private:
  float m_time;
  float m_activeTime;
  bool m_active;
  float m_severity;
};
}

#endif

