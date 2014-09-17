#ifndef TABLEDATA_H
#define TABLEDATA_H

#include "data/data.h"
#include <QList>

class CTableData: public CData
{

  private:
    QList<QList<int>> m_table;

  public:
    explicit CTableData();

    void addRow(QList<int> row);
    const QList<int> &getRow(int irow) const;
};

#endif // TABLEDATA_H
