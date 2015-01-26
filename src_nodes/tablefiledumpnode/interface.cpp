#include "interface.h"
#include "tablefiledumpnode.h"

extern "C"
{
void configure(CNodeConfig &config)
{
    CTableFileDumpNode::configure(config);
}

CNode *maker(const CNodeConfig &config)
{
    return new CTableFileDumpNode(config);
}
}

