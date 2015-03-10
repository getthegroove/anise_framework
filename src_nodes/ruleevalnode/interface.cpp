#include "interface.h"
#include "ruleevalnode.h"

extern "C"
{
void configure(CNodeConfig &config)
{
    CRuleEvalNode::configure(config);
}

CNode *maker(const CNodeConfig &config)
{
    return new CRuleEvalNode(config);
}
}

