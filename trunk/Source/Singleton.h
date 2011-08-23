/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Singleton.h,v $
Revision 1.1  2004/09/08 15:43:20  jay
Added to repository
  
*/

#if !defined(SINGLETON_T_H_INCLUDED)
#define SINGLETON_T_H_INCLUDED

namespace Amju
{
template <class T>
class Singleton
{
public:
  static T* Instance()
  {
    static T* t = 0;
	if (!t)
	{
	  t = new T;
	}
	return t;
  }

private:
  Singleton<T>();
  Singleton<T>(const Singleton<T>&);
  Singleton<T>& operator=(const Singleton<T>&);
};
}
#endif
