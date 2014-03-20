#ifndef INTERFACE_H
#define INTERFACE_H

class CData;

extern "C"
{
    const char *name();
    CData *maker();
}

#endif // INTERFACE_H
