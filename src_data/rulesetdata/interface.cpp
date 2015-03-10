#include "interface.h"
#include "rulesetdata.h"

extern "C"
{
CData *maker()
{
    return new CRulesetData();
}
}

