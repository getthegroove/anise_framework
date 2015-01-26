#include "interface.h"
#include "tcpstreamfeaturesdata.h"

extern "C"
{
CData *maker()
{
    return new CTcpStreamFeaturesData();
}
}

