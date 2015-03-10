#ifndef RULESETDATA_H
#define RULESETDATA_H

#include "rule.h"
#include "ruletypes.h"
#include "data/data.h"
#include <QtGlobal>
#include <QtDebug>


class CRulesetData: public CData
{
  private:
    // Helpers for translating nominals to strings and viceversa.
    QList<QString> m_nominal2string;
    QMap<QString, qint32> m_string2nominal;
    // The collection of rules.
    QList<CRule> m_ruleset;
    // Number of attributes used by the rules.
    qint32 m_attributes;
    // Number of tuples used to build this ruleset.
    qint32 m_tuples;

  public:
    explicit CRulesetData();
    virtual CDataPointer clone() const;

    // Reserve space for the rules.
    void reserve(qint32 space) { m_ruleset.reserve(space); }
    // Retreive or add a string to nominal.
    inline qint32 string2nominal(QString string);
    QString nominal2string(qint32 nominal) { return m_nominal2string[nominal]; }
    // Create a rule from a list of antecedents and add it.
    inline void addRule(Antecedent &a);
    // Get a modifiable list of all rules.
    QList<CRule> &getRules() { return m_ruleset; }
    const QList<CRule> &getRules() const { return m_ruleset; }
    // Return the number of rules in the ruleset.
    qint32 size() { return m_ruleset.size(); }
    // Return the list of all available nominal attributes.
    QList<QString> &getNominals() { return m_nominal2string; }
    // Set the number of attributes used to create the ruleset.
    void attributeCount(qint32 count) { m_attributes = count; }
    // Get the number of attrbitues used for the ruleset.
    qint32 attributeCount() const { return m_attributes; }
    // Set the tuples used.
    void tuplesCount(qint32 count) { m_tuples = count; }
    // Get the tuples used.
    qint32 tuplesCount() const { return m_tuples; }
};


qint32 CRulesetData::string2nominal(QString string)
{
    qint32 n;

    if(!m_string2nominal.contains(string)) {
        n = m_nominal2string.size();
        m_nominal2string.append(string);
        m_string2nominal[string] = n;
    }
    else {
        n = m_string2nominal[string];
    }

    return n;
}

void CRulesetData::addRule(Antecedent &a) {
    if(a.size() != m_attributes) {
        qWarning() << "Will NOT add rule with non-matching attributes' size.";
        return;
    }
    m_ruleset.append(CRule(a));
}


#endif // RULESETDATA_H

