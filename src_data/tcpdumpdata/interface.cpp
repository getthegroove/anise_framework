#include "interface.h"
#include "tcpdumpdata.h"

class CData;

extern "C"
{
    CData *maker()
    {
        return new CTcpDumpData();
    }
}
