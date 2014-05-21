#include "interface.h"
#include "skeleton.h"

extern "C"
{
    void configure(CNodeConfig &config)
    {
        Skeleton::configure(config);
    }

    CNode *maker(const CNodeConfig &config)
    {
        return new Skeleton(config);
    }
}
