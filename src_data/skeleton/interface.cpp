#include "interface.h"
#include "skeleton.h"

extern "C"
{
    CData *maker(QString name)
    {
        return new CSkeletonData(name);
    }
}
