#ifndef LERADFEATURESDATA_H
#define LERADFEATURESDATA_H

#include "data/data.h"
#include <QList>

class CLeradFeaturesData: public CData
{
  private:
    // Structures and variables.
    //QList<QList<int>> m_table;

  public:
    explicit CLeradFeaturesData();
    virtual CDataPointer clone() const { return CDataPointer(); }
};

#endif // LERADFEATURESDATA_H
