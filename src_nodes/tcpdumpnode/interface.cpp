#include "interface.h"
#include "tcpdumpnode.h"

extern "C"
{
    void configure(CNodeConfig &config)
    {
        CTcpDumpNode::configure(config);
    }

    CNode *maker(const CNodeConfig &config)
    {
        return new CTcpDumpNode(config);
    }
}
