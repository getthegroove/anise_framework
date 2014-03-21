#include "interface.h"
#include "tabledata.h"

extern "C"
{
    CData *maker()
    {
        return new CTableData();
    }
}
