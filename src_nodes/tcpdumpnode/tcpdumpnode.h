#ifndef TCPDUMPNODE_H
#define TCPDUMPNODE_H

#include "node/node.h"
#include "node/nodeconfig.h"
#include "tcpdumpdata/tcpdumpdata.h"
#include <QObject>
#include <QString>

class CTcpDumpNode: public CNode
{
  Q_OBJECT

  private:
    QSharedPointer<CTcpDumpData> m_tcpdump;

  public:
    // Constructor
    explicit CTcpDumpNode(const CNodeConfig &config, QObject *parent = 0);
    // Set the configuration template for this Node.
    static void configure(CNodeConfig &config);

  protected:
    // Function called when the simulation is started.
    // ... Read the file set in the parameters.
    virtual bool start();
    // Receive data sent by other nodes connected to this node.
    virtual void data(QString gate_name, const CConstDataPointer &data);
};

#endif // TCPDUMPNODE_H
