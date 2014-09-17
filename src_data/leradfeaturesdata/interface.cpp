#include "interface.h"
#include "leradfeaturesdata.h"

extern "C"
{
    CData *maker()
    {
        return new CLeradFeaturesData();
    }
}
