/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: DayNightSky.h,v $
Revision 1.1  2004/09/08 15:42:47  jay
Added to repository
  
*/

#if !defined(DAY_NIGHT_SKY_H_INCLUDED)
#define DAY_NIGHT_SKY_H_INCLUDED

#include "BillBoard.h"
#include "Lighting.h"
#include "SolidComponent.h"

namespace Amju
{
class DayNightSky
{
public:
  DayNightSky();

  bool Init();

  // Draw the sun or moon, and set the light colour and position. This requires
  // that there is no current transformation.
  void DrawLight();
  
  // Draw the skybox(es), which requires camera rotation.
  void DrawSkybox();

  // Updates Game Time and position of sun and moon.
  void Update();

  // Get the positon of the sun
  VertexBase GetSunPosition() const { return m_sunpos; }
  
  // Returns true if it's daylight.
  bool IsSunVisible() const { return (m_daylight > 0); }
  bool IsSparkleVisible() const;

  float GetTimeOfDay() const { return m_timeOfDay; }
  void SetTimeOfDay(float f) { m_timeOfDay = f; }

protected:
  float m_timeOfDay; // <=0, <24.0
  float m_gameHoursPerSec;
  float m_dawnBegin, m_dawnEnd, m_duskBegin, m_duskEnd, m_sparkleBegin, m_sparkleEnd;

  // Measure of how much daylight there is. 1.0 for day, 0 for night.
  float m_daylight;

  Lighting m_lighting;
  PSolidComponent m_sun;
  PSolidComponent m_moon;

  BillBoard m_sunsparkle;
  BillBoard m_shootingStar;

  PSolidComponent m_pDayBox;
  PSolidComponent m_pNightBox;

  // sin and cos values to get light/sun/moon position
  float m_s, m_c;
  VertexBase m_sunpos;
  VertexBase m_sunsparklepos;
  VertexBase m_moonpos;
  VertexBase m_shootstarpos;

  float m_nightLevel, m_dayLevel;

  // true when there is a shooting star active.
  bool m_isShootingStar;

  // Time till next shooting star.
  float m_shootStarTimer;
  // Angle of shooting star across sky
  float m_shootstarAngle;
};
}

#endif
