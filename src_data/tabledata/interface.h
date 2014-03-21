#ifndef INTERFACE_H
#define INTERFACE_H

#include <QString>

class CData;

extern "C"
{
    CData *maker(QString name);
}

#endif // INTERFACE_H
