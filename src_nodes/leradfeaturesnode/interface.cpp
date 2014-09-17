#include "interface.h"
#include "leradfeaturesnode.h"

extern "C"
{
    void configure(CNodeConfig &config)
    {
        CLeradFeaturesNode::configure(config);
    }

    CNode *maker(const CNodeConfig &config)
    {
        return new CLeradFeaturesNode(config);
    }
}
