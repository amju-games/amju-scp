/*
Amju Games source code (c) Copyright Jason Colman 2006
$Log: PoolGuiTable.h,v $
Revision 1.1.2.2  2007/03/09 21:48:56  jay
Copy latest GUI library from trunk

Revision 1.2  2006/01/16 13:27:55  jay
Add new PoolGuiElements for future use

*/

#ifndef AMJU_GUI_TABLE
#define AMJU_GUI_TABLE

#include "GuiComposite.h"

namespace Amju
{
class PoolGuiTable;

class PoolGuiTableCell : public PoolGuiElement
{
};

class PoolGuiTableColumn : public PoolGuiComposite
{
public:
  void SetWidth();

  float GetWidth() const;

protected:
  // Column no in table
  int m_index;

  // Table in which this column lives
  PoolGuiTable* m_pTable;

  float m_width;
};

class PoolGuiTableRow : public PoolGuiComposite
{
public:
  void SetHeight();

  float GetHeight() const;

protected:
  // Row no in table
  int m_index;

  // Table in which this column lives
  PoolGuiTable* m_pTable;

  float m_height;
};

class PoolGuiTable : public PoolGuiComposite
{
public:
  PoolGuiTable();

  // Conceptually allow client code to add a column of data.
  void AddColumn(PoolGuiTableColumn*);

  void AddRow(PoolGuiTableRow*);

  int GetNumColumns() const;
  
  int GetNumRows() const;

  virtual void DrawImpl();

protected:
  int m_numCols;
  int m_numRows;

  // Width of each column
  std::vector<float> m_colWidths;

  // Height of each row
  std::vector<float> m_rowHeights;
};
}

#endif

