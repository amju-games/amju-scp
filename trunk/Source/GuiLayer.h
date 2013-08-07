/*
Amju Games source code (c) Copyright Jason Colman 2000-2006
*/

#ifndef GUI_LAYER_H_INCLUDED
#define GUI_LAYER_H_INCLUDED

#include "RCPtr.h"
#include "TexturedQuad.h"
#include "Colour.h"
#include <vector>

namespace Amju
{
// A layer in a PoolGuiButton ( + other PoolGuiElements ?)
// A layer can be a TexturedQuad, or decorated with colours, etc.
class PoolGuiLayer : public RefCounted
{
public:
  virtual void Draw(float top, float left, float bottom, float right) = 0;
};

class PoolGuiLayerTq : public PoolGuiLayer
{
public:
  PoolGuiLayerTq(const TexturedQuad& tq);
  virtual void Draw(float top, float left, float bottom, float right);
private:
  TexturedQuad m_tq;
};

// Base class for decorators
class PoolGuiLayerDecorator : public PoolGuiLayer
{
public:
  PoolGuiLayerDecorator(PoolGuiLayer*);   
  virtual void Draw(float top, float left, float bottom, float right);
protected:
  RCPtr<PoolGuiLayer> m_pChild;
};

typedef std::vector<Colour> ColourList;

class PoolGuiLayerColourList : public PoolGuiLayerDecorator
{
public:
  PoolGuiLayerColourList(PoolGuiLayer* pChild, const ColourList& colours, float colourTimeSecs, bool interpolate = false);
  virtual void Draw(float top, float left, float bottom, float right);

protected:
  ColourList m_colours;
  float m_colourTime;
  float m_time; // time spent on current colour
  bool m_interpolate;  
  int m_index; // current index into m_colours
};
}

#endif

