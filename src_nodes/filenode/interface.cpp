#include "interface.h"
#include "filenode.h"

extern "C"
{
    const char *name()
    {
        return CFileNode::name();
    }

    void configure(CNodeConfig &config)
    {
        CFileNode::configure(config);
    }

    CNode *maker(const CNodeConfig &config)
    {
        return new CFileNode(config);
    }
}
