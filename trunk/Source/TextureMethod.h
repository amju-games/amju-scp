/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: TextureMethod.h,v $
Revision 1.1.10.2  2005/03/19 22:05:11  Administrator
Scene Editor fixes, mainly to new NullWithCoords method

Revision 1.1.10.1  2005/03/18 22:02:38  jay
Added NullWithCoords method

Revision 1.1  2004/09/08 15:42:58  jay
Added to repository
  
*/

#if !defined(TEXTURE_METHOD_H_INCLUDED)
#define TEXTURE_METHOD_H_INCLUDED

#include "RCPtr.h"

namespace Amju
{
class PoolTexture;
class File;
class LeafData;

// LeafData holds a TextureMethod subclass. This handles the loading/saving of 
// texture-related info, and the binding, etc.
class TextureMethod : public RefCounted
{
public:
  TextureMethod() : m_pTexture(0) {}
  virtual ~TextureMethod() {}

  void Bind();
  PoolTexture* GetTexture();
  void SetTexture(PoolTexture* p);

  virtual bool Load(File* pf) = 0;

#ifdef SCENE_EDITOR
  virtual bool Save(File* pf) = 0;
#endif

  virtual void BeginDisplayList(LeafData*) {}
  virtual void EndDisplayList(LeafData*) {}
  virtual void SetTexGenMode() const {}
  // Returns true if this Texture Method uses Texture Coords.
  virtual bool HasTextureCoords() const { return false; }

protected:
  // Not shared ptr because all Texture lifetimes are handled by TextureServer.
  PoolTexture* m_pTexture;
};

// This Texture Method has no Texture! This is so we can specify geometry
// without tying it to a particular texture. 
class TexMethodNull : public TextureMethod
{
public:
  virtual bool Load(File* pf) { return true; }
#ifdef SCENE_EDITOR
  virtual bool Save(File* pf);
#endif
};

// Null, meaning that a decorator specifies the texture.
// But we still specify texture coords in the leaf data.
class TexMethodNullWithCoords : public TexMethodNull
{
public:
  virtual bool Load(File* pf) { return true; }
  virtual bool HasTextureCoords() const { return true; }

#ifdef SCENE_EDITOR
  virtual bool Save(File* pf);
#endif
};

// Texture coords are specified in leaf data.
class TexMethodRegular : public TextureMethod
{
public:
  virtual bool Load(File* pf);
#ifdef SCENE_EDITOR
  virtual bool Save(File* pf);
#endif
  virtual void SetTexGenMode() const;
  virtual bool HasTextureCoords() const { return true; }
};

// Auto-generate texture coords.
class TexMethodAuto : public TextureMethod
{
public:
  TexMethodAuto();
  virtual bool Load(File* pf);
#ifdef SCENE_EDITOR
  virtual bool Save(File* pf);
#endif

  virtual void BeginDisplayList(LeafData*);
  virtual void EndDisplayList(LeafData*);
  virtual void SetTexGenMode() const;

  float GetSA() const { return m_coeffSA; }
  float GetSB() const { return m_coeffSB; }
  float GetSC() const { return m_coeffSC; }
  float GetSD() const { return m_coeffSD; }
  float GetTA() const { return m_coeffTA; }
  float GetTB() const { return m_coeffTB; }
  float GetTC() const { return m_coeffTC; }
  float GetTD() const { return m_coeffTD; }

  void SetSA(float f) { m_coeffSA = f; }
  void SetSB(float f) { m_coeffSB = f; }
  void SetSC(float f) { m_coeffSC = f; }
  void SetSD(float f) { m_coeffSD = f; }
  void SetTA(float f) { m_coeffTA = f; }
  void SetTB(float f) { m_coeffTB = f; }
  void SetTC(float f) { m_coeffTC = f; }
  void SetTD(float f) { m_coeffTD = f; }

protected:
  // Implement guts of Load() but without binding the texture. This is
  // for subclassing.
  bool LoadNoBinding(File*);

protected:
  float m_coeffSA, m_coeffSB, m_coeffSC, m_coeffSD,
        m_coeffTA, m_coeffTB, m_coeffTC, m_coeffTD;
};

// Environment mapped texture coords.
class TexMethodEnv : public TextureMethod
{
public:
  virtual bool Load(File* pf);
#ifdef SCENE_EDITOR
  virtual bool Save(File* pf);
#endif

  virtual void BeginDisplayList(LeafData*);
  virtual void EndDisplayList(LeafData*);
  virtual void SetTexGenMode() const;
};

// Auto-generated texture coord with clamp
class TexMethodRegularClamp : public TexMethodRegular
{
public:
  virtual bool Load(File* pf);
#ifdef SCENE_EDITOR
  virtual bool Save(File* pf);
#endif

};
}
#endif
