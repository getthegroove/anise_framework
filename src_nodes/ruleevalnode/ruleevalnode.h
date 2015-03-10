#ifndef RULEEVALNODE_H
#define RULEEVALNODE_H

#include "node/node.h"
#include "node/nodeconfig.h"
#include "tabledata/tabledata.h"
#include "rulesetdata/rulesetdata.h"
#include <QObject>
#include <QString>


class CRuleEvalNode: public CNode
{
    Q_OBJECT

  private:
    // Data Structures
    // Anomalies issued by the evaluation.
    QSharedPointer<CTableData> m_anomalies_data;
    // The table of tuples to evaluate against the rules.
    QSharedPointer<const CTableData> m_table_data;
    // Local copy of the ruleset used for evaluating m_table_data.
    QSharedPointer<CRulesetData> m_ruleset_data;

  public:
    // Constructor
    explicit CRuleEvalNode(const CNodeConfig &config, QObject *parent = 0);
    // Set the configuration template for this Node.
    static void configure(CNodeConfig &config);

  protected:
    // Function called when the simulation is started.
    virtual bool start();
    // Receive data sent by other nodes connected to this node.
    virtual void data(QString gate_name, const CConstDataPointer &data);
    // Do the evaluation of the Ruleset on the Table.
    void evaluate();
};

#endif // RULEEVALNODE_H

