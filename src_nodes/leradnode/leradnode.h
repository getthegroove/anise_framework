#ifndef LERADNODE_H
#define LERADNODE_H

#include "node/node.h"
#include "node/nodeconfig.h"
#include "types.h"
#include "consequent.h"
#include "rule.h"
#include "tabledata/tabledata.h"
#include <QObject>
#include <QString>

class CLeradNode: public CNode
{
  Q_OBJECT

  typedef QMap<Antecedent, CRule> Ruleset;

  private:
    // Data Structures

  public:
    // Constructor
    explicit CLeradNode(const CNodeConfig &config, QObject *parent = 0);
    // Set the configuration template for this Node.
    static void configure(CNodeConfig &config);

  protected:
    // Function called when the simulation is started.
    virtual bool start();
    // Receive data sent by other nodes connected to this node.
    virtual void data(QString gate_name, const CConstDataPointer &data);

    // The LERAD algorithm
    void lerad(const QSharedPointer<const CTableData> &table);
    inline qint32 rnd() const;
};

qint32 CLeradNode::rnd() const
{
    // 30-31 bit random number
    return (qrand() + (qrand() << 15)) & 0x7fffffff;
}

#endif // LERADNODE_H

