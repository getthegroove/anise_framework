#include "tabledata.h"
#include <QDebug>


//------------------------------------------------------------------------------
// Constructor and Destructor

CTableData::CTableData()
    : CData()
    , m_columns(1)
    , m_rows(1)
{

}

qint32 CTableData::getColCount() const
{
    if(m_table.size() > 0) {
        return m_table[0].size();
    }

    return 0;
}

void CTableData::addHeader(QString attr)
{
    m_header.append(attr);
}

const QList<QString> &CTableData::getHeader() const
{
    return m_header;
}

qint32 CTableData::headerSize() const
{
    return m_header.size();
}

//------------------------------------------------------------------------------
// Public Functions

QList<QVariant> &CTableData::newRow()
{
    m_table.append(QList<QVariant>());
    m_table.last().reserve(m_columns);

    return m_table.last();
}

const QList<QVariant> &CTableData::getRow(int i_row) const
{
    return m_table.at(i_row);
}

CDataPointer CTableData::clone() const
{
    qCritical() << "Cloning not implemented.";

    return CDataPointer();
}

void CTableData::sort(qint32 field1, qint32 field2)
{
    // Lambda sort function.
    auto f_less_than = [&] (const QList<QVariant> &r1, const QList<QVariant> &r2)
    {
        if(r1.at(field1).toString() < r2.at(field1).toString()) {
            return true;
        }
        else if(r1.at(field1).toString() == r2.at(field1).toString()) {
            if(r1.at(field2).toString() < r2.at(field2).toString()) {
                return true;
            }
            else {
                return false;
            }
        }
        else {
            return false;
        }
    };
    std::sort(m_table.begin(), m_table.end(), f_less_than);
}


//------------------------------------------------------------------------------
// Private Functions
