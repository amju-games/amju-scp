/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: UniqueId.h,v $
Revision 1.1  2004/09/08 15:42:59  jay
Added to repository
  
*/

#if !defined(UNIQUE_ID_H_INCLUDED)
#define UNIQUE_ID_H_INCLUDED

namespace Amju
{
class UniqueId
{
public:
  // Test id to see if it is in use by an existing Game Object.
  static bool IsUnique(int id);

  // Get a new unique ID in the range given (incl).
  // Returns 0 if a unique ID in the range does not exist.
  static int GetNewUniqueId(int lowerBound, int upperBound);

  // Get an ID, with no range restriction.
  // This uses the IDs set in SetUsedId() to return an ID which is
  // higher than any previously used ID.
  static int GetNewUniqueId();

  // Call to set an ID so it will not be reused.
  static void SetUsedId(int id);

private:
  static int s_highest;
};
}

#endif

