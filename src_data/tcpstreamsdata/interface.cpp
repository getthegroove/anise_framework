#include "interface.h"
#include "tcpstreamsdata.h"

extern "C"
{
    CData *maker()
    {
        return new CTcpStreamsData();
    }
}

