#include "interface.h"
#include "tcpstreamfeaturesnode.h"

extern "C"
{
void configure(CNodeConfig &config)
{
    CTcpStreamFeaturesNode::configure(config);
}

CNode *maker(const CNodeConfig &config)
{
    return new CTcpStreamFeaturesNode(config);
}
}

