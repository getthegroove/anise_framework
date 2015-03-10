#ifndef CONSEQUENT
#define CONSEQUENT

#include "ruletypes.h"
#include <QtGlobal>

class CConsequent
{
  public:
    qint32 n; // Support
    qint32 t; // Time of last anomaly
    Values values; // Allowed values

  public:
    CConsequent(): n(0), t(0) {}
    CConsequent(Nominal v): n(0), t(0) { values[v] = 1; }

    qint32 add(Nominal v) {++n; return ++values[v];}
    void clear() { values.clear(); n = 0; t = 0; }
    bool find(Nominal n) const { return values.find(n) != values.constEnd(); }
    qint32 valuesCount() const { return values.size(); }
};

#endif // CONSEQUENT

