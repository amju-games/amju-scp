/*
Amju Games source code (c) Copyright Jason Colman 2007
*/

#ifndef ES_POOL_DECLINED_H_INCLUDED
#define ES_POOL_DECLINED_H_INCLUDED

#include "EsPoolYesNo.h"

namespace Amju
{
class EsPoolDeclined : public EsPoolYesNo
{
public:
  static const char* Name;
  EsPoolDeclined();
  virtual void SetActive(bool);
  virtual void OnYes();
  virtual void OnNo() {}
};
}

#endif

