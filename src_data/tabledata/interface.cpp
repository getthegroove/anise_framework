#include "interface.h"
#include "tabledata.h"

extern "C"
{
    CData *maker(QString name)
    {
        return new CTableData(name);
    }
}
