/*
Amju Games source code (c) Copyright Jason Colman 2006
$Log: GuiTable.cpp,v $
Revision 1.1.2.6  2007/03/09 21:48:56  jay
Copy latest GUI library from trunk

Revision 1.2  2006/01/16 13:27:55  jay
Add new GuiElements for future use

*/

#include <iostream>
#include "GuiTable.h"

namespace Amju
{
static const float MIN_ROW_HEIGHT = 2.0f;
static const float MIN_COL_WIDTH = 5.0f; // TODO CONFIG

GuiTable::GuiTable()
{
  m_numRows = 0;
  m_numCols = 0;
}

void GuiTable::AddColumn(GuiTableColumn* pCol)
{
  // Add elements to every row
}

void GuiTable::AddRow(GuiTableRow* pRow)
{
#ifdef _DEBUG
std::cout << "GuiTable: Adding a row\n";
#endif

  // Add elements to the end of m_children
  int numChildren = pRow->GetNumberOfChildren();
  float height = 0;
  for (int i = 0; i < numChildren; i++)
  {
    float childHeight = pRow->GetChild(i)->GetHeight();
    if (childHeight > height)
    {
      height = childHeight;
    }
    
    AddChild(pRow->GetChild(i).GetPtr());
  }
  // Add height for this new row.
  if (height < MIN_ROW_HEIGHT)
  {
    height = MIN_ROW_HEIGHT;
  }
#ifdef _DEBUG
std::cout << "GuiTable: adding row height: " << height << "\n";
#endif
  m_rowHeights.push_back(height);

  m_numRows++;
#ifdef _DEBUG
std::cout << "GuiTable: no of rows is now " << m_numRows << "\n";
#endif

  if (m_numCols == 0)
  {
    // No columnns yet, so just set the no of columns to the number of 
    // elements in this row.
    m_numCols = numChildren;
    Assert(m_colWidths.empty());
    for (int i = 0; i < numChildren; i++)
    {
      // Set the width of each column
      float width = pRow->GetChild(i)->GetWidth();
      if (width < MIN_COL_WIDTH)
      {
        width = MIN_COL_WIDTH;
      }
#ifdef _DEBUG
std::cout << "GuiTable: adding column width: " << width << "\n";
#endif
      m_colWidths.push_back(width);
    }

#ifdef _DEBUG
std::cout << "GuiTable: first row, so set no of cols: " << m_numCols << "\n";
#endif
  }
  else
  {
    if (m_numCols != numChildren)
    {
      // Number of elements in this row doesn't match the number of 
      // columns in the table - need to re-arrange the table!
    }
    Assert(m_colWidths.size() == numChildren);
    for (int i = 0; i < numChildren; i++)
    {
      // Set the width of each column
      float width = pRow->GetChild(i)->GetWidth();
      if (width > m_colWidths[i])
      {
#ifdef _DEBUG
std::cout << "GuiTable: resizing column: " << i << " from " << m_colWidths[i]
  << " to " << width << "\n";
#endif

        m_colWidths[i] = width;
      }
    }
  }
}

int GuiTable::GetNumColumns() const
{
  return m_numCols;
}

int GuiTable::GetNumRows() const
{
  return m_numRows;
}

void GuiTable::DrawImpl()
{
#ifdef TABLE_DRAW_DEBUG
std::cout << "GuiTable::DrawImpl\n";
#endif

  // Draw each element, from top to bottom, left to right.
  float top = 0;
  for (int r = 0; r < m_numRows; r++)
  {
    Assert(r < m_rowHeights.size());
    float height = m_rowHeights[r];

    float left = 0;
    for (int c = 0; c < m_numCols; c++)
    {
      int i = r * m_numCols + c;
#ifdef TABLE_DRAW_DEBUG
std::cout << "GuiTable: drawing element " << i << "\n";
#endif

      Assert(i < m_children.size());
      SharedPtr<GuiElement> pElem = m_children[i];
      // Set width and height of this element
      Assert(pElem.GetPtr());

      Assert(c < m_colWidths.size());
      float width = m_colWidths[c];

      pElem->SetSize(width, height);
      pElem->SetRelPos(top, left);
      pElem->Draw();

      left += width;
    }
    top += height; 
  }
}

}


