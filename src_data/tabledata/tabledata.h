#ifndef TABLEDATA_H
#define TABLEDATA_H

#include "data.h"
#include "datainfo.h"

class CTableData: public CData, public CDataInfo<CTableData>
{
  public:
    explicit CTableData();

    // The name of this type of data.
    static const char *name();
};

#endif // TABLEDATA_H
