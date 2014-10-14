#include "interface.h"
#include "filedata.h"

extern "C"
{
    CData *maker()
    {
        return new CFileData();
    }
}

