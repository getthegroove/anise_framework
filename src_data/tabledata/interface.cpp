#include "interface.h"
#include "tabledata.h"

extern "C"
{
    const char *name()
    {
        return CTableData::name();
    }

    CData *maker()
    {
        return new CTableData();
    }
}
