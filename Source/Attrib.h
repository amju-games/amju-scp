/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Attrib.h,v $
Revision 1.1  2004/09/08 15:42:44  jay
Added to repository
  
*/

#if !defined(ATTRIB_H_INCLUDED)
#define ATTRIB_H_INCLUDED

#include "TextureMethod.h"
#include "SharedPtr.h"

namespace Amju
{
class File;
class Attrib;
class Function;

typedef SharedPtr<Attrib> PAttrib;

// A subclass of Attrib is owned by each AttribDecorator. The decorator applies
// the Attrib before drawing the Composite it owns. This lets Attribs change the
// appearance of Composites.
class Attrib : public Shareable
{
public:
  virtual ~Attrib() {}
  virtual void Draw() = 0; // Modify the attribute.
  virtual void Restore() = 0; // Restore the attribute to its old value.
  virtual bool Load(File* pf) = 0; // Load attribute data
  // Make deep copy on the heap.
  virtual PAttrib Clone() = 0;
  virtual const char* GetTypeName() const = 0;

#if defined(SCENE_EDITOR)
  virtual bool Save(File* pf) = 0;
#endif

};

// This attribute modifies the depth buffer state.
class DepthAttrib : public Attrib
{
public:
  static const char* TypeName;

  DepthAttrib();
  virtual void Draw();
  virtual void Restore();
  virtual bool Load(File* pf);
  virtual PAttrib Clone();
  virtual const char* GetTypeName() const;

protected:
  bool m_bDepthBufferEnabled;

#if defined(SCENE_EDITOR)
public:
  virtual bool Save(File* pf);

  void Set(bool b) { m_bDepthBufferEnabled = b; }
  bool Get() const { return m_bDepthBufferEnabled; }
#endif

};

class LightAttrib : public Attrib
{
public:
  static const char* TypeName;

  LightAttrib();
  virtual void Draw();
  virtual void Restore();
  virtual bool Load(File* pf);
  virtual PAttrib Clone();
  virtual const char* GetTypeName() const;

protected:
  bool m_bLightingEnabled;

#if defined(SCENE_EDITOR)
public:
  virtual bool Save(File* pf);

  void Set(bool b) { m_bLightingEnabled = b; }
  bool Get() const { return m_bLightingEnabled; }
#endif

};

// Modify the current colour.
class ColourAttrib : public Attrib
{
public:
  static const char* TypeName;

  ColourAttrib();
  virtual void Draw();
  virtual void Restore();
  virtual bool Load(File* pf);
  virtual PAttrib Clone();
  virtual const char* GetTypeName() const;

  void Set(float r, float g, float b, float a);

protected:
  // The new colour
  float m_r, m_g, m_b, m_a;

#if defined(SCENE_EDITOR)
public:
  virtual bool Save(File* pf);

  float GetR() const { return m_r; }
  float GetG() const { return m_g; }
  float GetB() const { return m_b; }
  float GetA() const { return m_a; }
#endif

};

// Turn blending function to additive.
// TODO allow other blend functions.
class BlendAttrib : public Attrib
{
public:
  static const char* TypeName;

  virtual void Draw();
  virtual void Restore();
  virtual bool Load(File* pf);
  virtual PAttrib Clone();
  virtual const char* GetTypeName() const;

#if defined(SCENE_EDITOR)
public:
  virtual bool Save(File* pf);
#endif

};

class TextureRotateAttrib : public Attrib
{
public:
  static const char* TypeName;

  TextureRotateAttrib() : m_angle(0), m_rotVel(0) {}
  TextureRotateAttrib(float v) : m_angle(0), m_rotVel(v) {}

  virtual void Draw();
  virtual void Restore();
  virtual bool Load(File* pf);
  virtual PAttrib Clone();
  virtual const char* GetTypeName() const;

protected:
  float m_angle;
  float m_rotVel;

#if defined(SCENE_EDITOR)
public:
  virtual bool Save(File* pf);
#endif

};

class TextureScrollAttrib : public Attrib
{
public:
  static const char* TypeName;

  TextureScrollAttrib();

  // Set scroll velocities from code
  TextureScrollAttrib(float x, float y, float z);

  virtual void Draw();
  virtual void Restore();
  virtual bool Load(File* pf);
  virtual PAttrib Clone();
  virtual const char* GetTypeName() const;

protected:
  float m_time;
  float m_xvel, m_yvel, m_zvel; // scroll velocities

#if defined(SCENE_EDITOR)
public:
  virtual bool Save(File* pf);

  void Set(float x, float y, float z) { m_xvel = x; m_yvel = y; m_zvel = z; }
  float GetX() const { return m_xvel; }
  float GetY() const { return m_yvel; }
  float GetZ() const { return m_zvel; }
#endif

};

class TextureScaleAttrib : public Attrib
{
public:
  static const char* TypeName;

  TextureScaleAttrib();

  // Set scale params from code
  // TODO TextureScaleAttrib(float x, float y, float z);

  virtual void Draw();
  virtual void Restore();
  virtual bool Load(File* pf);
  virtual PAttrib Clone();
  virtual const char* GetTypeName() const;

#if defined(SCENE_EDITOR)
public:
  virtual bool Save(File* pf);
#endif

protected:
  // Use a function to change Scale wrt time.
  SharedPtr<Function> m_pFunc;
};

// All the goodness of a TextureMethod in a Decorator - the idea is that leaf
// files have null texture methods, and we instead use one of these to set the 
// texture method. This decouples the geometry from the texture in a
// backward-compatible manner.
class TextureMethodAttrib : public Attrib
{
public:
  static const char* TypeName;

  TextureMethodAttrib();

  virtual void Draw();
  virtual void Restore();
  virtual bool Load(File* pf);
  virtual PAttrib Clone();
  virtual const char* GetTypeName() const;

protected:
  SharedPtr<TextureMethod> m_pTextureMethod;

#if defined(SCENE_EDITOR)
public:
  virtual bool Save(File* pf);

  SharedPtr<TextureMethod> Get() { return m_pTextureMethod; }
  void Set(SharedPtr<TextureMethod> p) { m_pTextureMethod = p; }
#endif

};
}
#endif


