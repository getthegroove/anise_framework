#ifndef TABLEDATA_H
#define TABLEDATA_H

#include "data.h"
#include <QList>

class CTableData: public CData
{

  private:
    QList<QList<int>> m_table;

  public:
    explicit CTableData(QString name);

    void addRow(QList<int> row);
    const QList<int> &getRow(int irow) const;
};

#endif // TABLEDATA_H
