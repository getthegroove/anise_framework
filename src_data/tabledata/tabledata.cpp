#include "tabledata.h"

//------------------------------------------------------------------------------
// Constructor and Destructor

CTableData::CTableData()
{

}


//------------------------------------------------------------------------------
// Public Functions

void CTableData::addRow(QList<int> row)
{
    m_table.append(row);
}


const QList<int> &CTableData::getRow(int i_row) const
{
    return m_table.at(i_row);
}

//------------------------------------------------------------------------------
// Public Slots


//------------------------------------------------------------------------------
// Private Functions


//------------------------------------------------------------------------------
// Private Slots

