#ifndef SKELETONDATA_H
#define SKELETONDATA_H

#include "data.h"
#include <QList>

class CSkeletonData: public CData
{
  private:
    // Structures and variables.
    //QList<QList<int>> m_table;

  public:
    explicit CSkeletonData(QString name);
};

#endif // SKELETONDATA_H
