#include "interface.h"
#include "tcpstreamextractornode.h"

extern "C"
{
    void configure(CNodeConfig &config)
    {
        CTcpStreamExtractorNode::configure(config);
    }

    CNode *maker(const CNodeConfig &config)
    {
        return new CTcpStreamExtractorNode(config);
    }
}
