/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: SortScene.h,v $
Revision 1.1  2004/09/08 15:42:57  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_SORT_SCENE_H_INCLUDED)
#define SCHMICKEN_SORT_SCENE_H_INCLUDED

#include "VisibleGameObject.h"
#include "SolidComponent.h"
#include "Vertex.h"

namespace Amju
{
// This Game Object shows SolidComponents, drawn in order from furthest to
// closest to the camera.
// This is used to draw partly transparent objects, e.g. billboarded trees.
// Make SortScenes the last GameObjects in a room so everything behind them
// can be seen through transparent areas.
class SortScene : public VisibleGameObject
{
public:
  SortScene();

  virtual void Draw();
  virtual void Update();
  virtual const BoundingSphere* GetBoundingSphere() const;
  virtual void CreateBoundingSphere(const Matrix& cm);
  virtual const char* GetTypeName() const;
  virtual void DrawShadow() const {}
  virtual void CullDraw(Frustum*);
  virtual void RefreshHeightServer() {}
  virtual bool Load(File*);
  virtual void RecalcCollisionVol();

  void AddSolid(PSolidComponent pSolid);

protected:
  // Re-sort vector of solids if eye position has changed.
  void Resort();

protected:
  // Vector of SolidComponents. This is sorted whenever the eye position changes.
  CompVector m_solids;
  VertexBase m_lastEyePos;

  struct SceneSorter
  {
    SceneSorter(const VertexBase& v) : m_v(v) {}
    bool operator()(const PSolidComponent& p1, const PSolidComponent& p2) const;

    VertexBase m_v;
  };

  BoundingSphere m_bsphere;  

#ifdef SCENE_EDITOR
public:
  virtual bool Save(File*);

  virtual PropertyMap GetProperties() const;
  virtual void SetProperties(const PropertyMap&);
#endif

protected:
  // List of solids loaded from file. m_solids is an expanded version of this.
  std::vector<std::string> m_unexpandedNames;
};
}

#endif


