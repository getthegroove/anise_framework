#ifndef TABLEDATA_H
#define TABLEDATA_H

#include "data/data.h"
#include <QList>
#include <QVariant>


class CTableData: public CData
{

  private:
    QList<QList<QVariant>> m_table;
    // Size hints to improve memory allocation efficiency.
    qint32 m_columns;
    qint32 m_rows;
    // String representations of the table columns.
    QList<QString> m_header;

  public:
    explicit CTableData();

    void setRows(qint32 rows) { m_rows = rows; }
    void setCols(qint32 cols) { m_columns = cols; }
    qint32 getRowCount() const { return m_table.size(); }
    void addHeader(QString attr);
    const QList<QString> &getHeader() const;
    qint32 headerSize() const;
    QList<QVariant> &newRow();
    const QList<QVariant> &getRow(int irow) const;
    virtual CDataPointer clone() const;

    void sort(qint32 field1, qint32 field2);
};

#endif // TABLEDATA_H
