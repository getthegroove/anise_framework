#include "interface.h"
#include "tcpdumpdata.h"

extern "C"
{
    CData *maker()
    {
        return new CTcpDumpData();
    }
}
