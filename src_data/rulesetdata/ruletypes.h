#ifndef TYPES
#define TYPES

#include <QtGlobal>
#include <QMap>
#include <QList>


typedef qint32 Nominal;  // Index of attribute value
typedef QMap<Nominal, qint32> Values;  // Set of allowed values
typedef QList<Nominal> Antecedent;  // Conditions: 0=don't care, 1=cons.


#endif // TYPES

