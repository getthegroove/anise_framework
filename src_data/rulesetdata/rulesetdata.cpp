#include "rulesetdata.h"
#include <QDebug>


//------------------------------------------------------------------------------
// Constructor and Destructor

CRulesetData::CRulesetData()
    : CData()
    , m_attributes(0)
{

}


//------------------------------------------------------------------------------
// Public Functions

CDataPointer CRulesetData::clone() const
{
    // Clone using the implicit copy constructor.
    CRulesetData *ruleset_clone = new CRulesetData(*this);
//    ruleset_clone->m_nominal2string = this->m_nominal2string;
//    ruleset_clone->m_string2nominal = this->m_string2nominal;
//    ruleset_clone->m_ruleset = this->m_ruleset;
//    ruleset_clone->m_attributes = this->m_attributes;
//    ruleset_clone->m_tuples = this->m_tuples;

    return QSharedPointer<CRulesetData>(ruleset_clone);
}


//------------------------------------------------------------------------------
// Private Functions

