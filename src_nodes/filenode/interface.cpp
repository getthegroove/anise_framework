#include "interface.h"
#include "filenode.h"

extern "C"
{
    void configure(CNodeConfig &config)
    {
        CFileNode::configure(config);
    }

    CNode *maker(const CNodeConfig &config)
    {
        return new CFileNode(config);
    }
}
