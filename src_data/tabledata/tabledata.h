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
    // String representations of the table columns.
    QList<QString> m_header;

  public:
    explicit CTableData();

    void reserveRows(qint32 size) { m_table.reserve(size); }
    void setCols(qint32 cols) { m_columns = cols; }
    qint32 getRowCount() const { return m_table.size(); }
    qint32 getColCount() const;
    void addHeader(QString attr);
    void addHeader(const QList<QString> &attrs);
    const QList<QString> &getHeader() const;
    qint32 headerSize() const;
    QList<QVariant> &newRow();
    const QList<QVariant> &getRow(int irow) const;
    virtual CDataPointer clone() const;

    void sort(qint32 field1, qint32 field2);
};

#endif // TABLEDATA_H
