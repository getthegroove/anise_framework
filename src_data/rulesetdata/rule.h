#ifndef RULE
#define RULE

#include "ruletypes.h"
#include "consequent.h"
#include <QDebug>

class CRule
{
  public:
    CConsequent consequent;
    Antecedent antecedent;  // 0 = *, 1 = ?, >1 = constraint
    qint32 i_consequent;   // index of ? in rule
    QList<qint32> i_antecedents;  // Indices of anteceedents >1

  public:
    CRule(): i_consequent(-1) {}
    CRule(qint32 n) : consequent(n), i_consequent(-1) {}
    CRule(const Antecedent &a): CRule(a, CConsequent()) {}
    CRule(const Antecedent &a, const CConsequent &c):
        consequent(c), antecedent(a)
    {
        i_consequent = -1;
        qint32 rule_size = antecedent.size();
        for(qint32 i = 0; i < rule_size; ++i) {
            if(antecedent[i] == 1) {
                i_consequent = i;
            }
            else if(antecedent[i] > i) {
                i_antecedents.append(i);
            }
        }
    }

    // For sorting rules by max n/r, and by min anteceedents if equal
    bool operator<(const CRule &r) const
    {
        const qint32 nr1 = consequent.n * r.consequent.values.size();
        const qint32 nr2 = r.consequent.n * consequent.values.size();
        return nr1 > nr2 ||
                (nr1 == nr2 &&
                 i_antecedents.size() < r.i_antecedents.size());
    }

    bool matchAntecedents(const Antecedent &a) const
    {
        qint32 ante_size = i_antecedents.size();
        for(qint32 i = 0; i < ante_size; ++i) {
            if(antecedent[i_antecedents[i]] != a[i_antecedents[i]]) {
                return false;
            }
        }
        return true;
    }

    bool matchConsequent(const Antecedent &a) const
    {
        if(i_consequent >= a.size() || i_consequent <= -1) {
            qWarning() << "Cannot match consequents.";
            return false;
        }
        // Return true if we find a match.
        return consequent.find(a[i_consequent]);
    }
};


#endif // RULE

