#include "interface.h"
#include "leradnode.h"

extern "C"
{
void configure(CNodeConfig &config)
{
    CLeradNode::configure(config);
}

CNode *maker(const CNodeConfig &config)
{
    return new CLeradNode(config);
}
}

