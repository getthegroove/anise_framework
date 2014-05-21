#include "interface.h"
#include "skeleton.h"

extern "C"
{
    void configure(CNodeConfig &config)
    {
        CSkeleton::configure(config);
    }

    CNode *maker(const CNodeConfig &config)
    {
        return new CSkeleton(config);
    }
}
