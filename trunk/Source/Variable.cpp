/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Variable.cpp,v $
Revision 1.1  2004/09/08 15:43:20  jay
Added to repository
  
*/

#include "Variable.h"
#include "StringUtils.h"
#include "SchAssert.h"

namespace Amju
{
Variable::Variable()
{
  m_type = VARIABLE_NULL;
}

Variable::Variable(const Variable& rhs)
{
  m_type = rhs.m_type;
  switch (m_type)
  {
  case VARIABLE_STRING:
    m_data.pString = new std::string(*(rhs.m_data.pString));
    break;
  case VARIABLE_VECTOR:
    m_data.pVec = new VariableVec(*(rhs.m_data.pVec));
    break;
  default:
    m_data = rhs.m_data;
  }
}

Variable::Variable(bool b)
{
  m_type = VARIABLE_BOOL;
  m_data.b = b;
}

Variable::Variable(int i)
{
  m_type = VARIABLE_INT;
  m_data.i = i;
}

Variable::Variable(float f)
{
  m_type = VARIABLE_FLOAT;
  m_data.f = f;
}

Variable::Variable(const std::string& s)
{
  m_type = VARIABLE_STRING;
  m_data.pString = new std::string(s);
}

Variable::Variable(const VariableVec& v)
{
  m_type = VARIABLE_VECTOR;
  m_data.pVec = new VariableVec(v);
}

Variable::~Variable()
{
  Clear();
}

void Variable::Swap(Variable* pOther)
{
  Type t = m_type;
  m_type = pOther->m_type;
  pOther->m_type = t;

  Data d = m_data;
  m_data = pOther->m_data;
  pOther->m_data = d;
}

Variable& Variable::operator=(const Variable& var)
{
  Variable temp(var);
  Swap(&temp);
  return *this;
}

Variable& Variable::operator=(bool b)
{
  Clear();
  m_type = VARIABLE_BOOL;
  m_data.b = b;
  return *this;
}

Variable& Variable::operator=(int i)
{
  Clear();
  m_type = VARIABLE_INT;
  m_data.i = i;
  return *this;
}

Variable& Variable::operator=(float f)
{
  Clear();
  m_type = VARIABLE_FLOAT;
  m_data.f = f;
  return *this;
}

Variable& Variable::operator=(const std::string& s)
{
  Clear();
  m_type = VARIABLE_STRING;
  m_data.pString = new std::string(s);
  return *this;
}

Variable& Variable::operator=(const VariableVec& v)
{
  Clear();
  m_type = VARIABLE_VECTOR;
  m_data.pVec = new VariableVec(v);
  return *this;
}

void Variable::Clear()
{
  switch (m_type)
  {
  case VARIABLE_STRING:
    delete m_data.pString;
    break;
  case VARIABLE_VECTOR:
    delete m_data.pVec;
    break;
  default:
    ;
  }
  m_type = VARIABLE_NULL;
}

Variable::Type Variable::GetType() const
{
  return m_type;
}

bool Variable::IsNull() const
{
  return m_type == VARIABLE_NULL;
}

bool Variable::IsBoolType() const
{
  return m_type == VARIABLE_BOOL;
}

bool Variable::IsIntType() const
{
  return m_type == VARIABLE_INT;
}

bool Variable::IsFloatType() const
{
  return m_type == VARIABLE_FLOAT;
}

bool Variable::IsStringType() const
{
  return m_type == VARIABLE_STRING;
}

bool Variable::IsVectorType() const
{
  return m_type == VARIABLE_VECTOR;
}

bool Variable::GetBool() const 
{
  Assert(IsBoolType());
  return m_data.b;
}

int Variable::GetInt() const
{
  Assert(IsIntType());
  return m_data.i;
}

float Variable::GetFloat() const
{
  Assert(IsFloatType() || IsIntType());
  if (IsFloatType())
  {
    return m_data.f;
  }
  else
  {
    return m_data.i;
  }
  return 0;
}

std::string Variable::GetString() const
{
  Assert(IsStringType());
  return *(m_data.pString);
}

std::vector<Variable> Variable::GetVector() const
{
  Assert(IsVectorType());
  return *(m_data.pVec);
}

std::string Variable::ToString() const
{
  switch (m_type)
  {
  case VARIABLE_NULL:
    return "<null>";

  case VARIABLE_BOOL:
    return m_data.b ? "true" : "false";

  case VARIABLE_INT:
    return Amju::ToString(m_data.i);

  case VARIABLE_FLOAT:
    return Amju::ToString(m_data.f);

  case VARIABLE_STRING:
    return *(m_data.pString);

  case VARIABLE_VECTOR:
    return VecToString();
  }

  Assert(0);
  return "<unknown type>";
}

std::string Variable::VecToString() const
{
  Assert(IsVectorType());

  if (m_data.pVec->empty())
  {
    return "<empty list>";
  }

  const int size = m_data.pVec->size();

  std::string res = "list [";
  res += Amju::ToString(size);
  res += "]: (";

  for (int i = 0; i < (int)size; i++)
  {
    res += m_data.pVec->at(i).ToString();
    if (i < (int)size - 1)
    {
      res += ", ";
    }
  }
  res += ")";
  return res;
}

}
