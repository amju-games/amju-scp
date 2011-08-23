/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: FunctionCtrlPoints.h,v $
Revision 1.1  2004/09/08 15:42:50  jay
Added to repository
  
*/

#if !defined(FUNCTION_CTRL_POINTS_H_INCLUDED)
#define FUNCTION_CTRL_POINTS_H_INCLUDED

#include "Function.h"
#include "Interpolate.h"
#include "Orientation.h"
#include <string>
#include <utility> // pair 
#include <vector>

namespace Amju
{
// This function makes a curve through space using control point/time pairs.
class FunctionCtrlPoints : public Function
{
public:
  static const char* Name;

  virtual bool Load(File* pf);
  virtual void Recalc(Orientation* p);

  // Editing the path
  // ----------------

  int GetNumPoints() const;
  void GetPoint(int index, float* pTimeResult, Orientation* pOrientationResult); 

  void Clear();
  void AddPoint(float time, const Orientation&);

  // Set the y-rotate of each control point so that it is facing the next control point.
  // This is useful if the path is for steering an object, which should face the
  // direction of movement.
  void FaceDirection();

protected:
  // List of time, point pairs
  // Vector makes it easier to get points by index.
  typedef std::vector<TimePoint> TimePointList; 

  void Interp(TimePoint* pPoints, Orientation* pResult);

  // List of control points. Each control point consists of a time, a position
  // and an orientation. 
  // The position at some time is interpolated using a spline curve. 
  // The angles of rotation are interpolated linearly.
  // The time of the first control point should be zero. The time of subsequent
  // control points is the time taken to reach that point. I.e. the time for 
  // point <n> is the time from zero, not the delta time from point <n-1>.
  TimePointList m_points;


#if defined(SCENE_EDITOR)
public:
  virtual const char* GetTypeName() const;

  virtual bool Save(File*);
#endif

};
}

#endif
