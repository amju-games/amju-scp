/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Signpost.h,v $
Revision 1.1  2004/09/08 15:42:41  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_SIGNPOST_H_INCLUDED)
#define SCHMICKEN_SIGNPOST_H_INCLUDED

namespace Amju
{
// A Signpost tells NPCs what direction to go in.
class SignPost : public GameObject
{
public:
  virtual void Draw();
  virtual const BoundingSphere* GetBoundingSphere() const;
  virtual void CreateBoundingSphere(const Matrix& cm);
  virtual bool Load(File* pf);
  virtual bool Save(File*);
  virtual const char* GetTypeName() const;
  // Redirect NPCs when they intersect the SignPost bounding sphere.
  virtual void HandleObjectCollision(GameObject* pObj);

  //void SetSphere(const BoundingSphere& b) { m_bsphere = b; }

protected:
  BoundingSphere m_bsphere;

};
}

#endif

