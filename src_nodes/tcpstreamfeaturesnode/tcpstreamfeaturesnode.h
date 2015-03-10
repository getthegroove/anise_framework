#ifndef TCPSTREAMFEATURESNODE_H
#define TCPSTREAMFEATURESNODE_H

#include "node/node.h"
#include "node/nodeconfig.h"
#include "tcpstreamsdata/tcpstream.h"
#include <QObject>
#include <QString>
#include <QSharedPointer>
#include <tabledata/tabledata.h>


class CTcpStreamFeaturesNode: public CNode
{
  Q_OBJECT

  private:
    // Data Structures
    QSharedPointer<CTableData> m_table;
    // How many streams have we processed.
    qint32 m_processed_streams;

  public:
    // Constructor
    explicit CTcpStreamFeaturesNode(const CNodeConfig &config, QObject *parent = 0);
    // Set the configuration template for this Node.
    static void configure(CNodeConfig &config);

  protected:
    // Function called when the simulation is started.
    virtual bool start();
    // Receive data sent by other nodes connected to this node.
    virtual void data(QString gate_name, const CConstDataPointer &data);

  private:
    bool createFeaturesTable();
    void extractFeatures(const CTcpStream &tcp_stream);
    QString buildFlagsString(quint8 flags);
    QStringList extractStrings(const QVector<quint8> &data);
};

#endif // TCPSTREAMFEATURESNODE_H

