#ifndef FILENODE_H
#define FILENODE_H

#include "node.h"
#include "nodeconfig.h"
#include "tabledata/tabledata.h"
#include <QObject>
#include <QString>

class CFileNode: public CNode
{
  Q_OBJECT

  private:
    QSharedPointer<CTableData> m_table;

  public:
    // Constructor
    explicit CFileNode(const CNodeConfig &config, QObject *parent = 0);
    // Set the configuration template for this Node.
    static void configure(CNodeConfig &config);

  public slots:
    // Receive data sent by other nodes connected to this node.
    virtual void data(QSharedPointer<CData> data);

  protected:
    // The place where we are able to initialize "Data" data structures which we
    // ... would like to use within the Node.
    virtual void init(const CDataFactory &data_factory);
    // Function called when the simulation is started.
    // ... Read the file set in the parameters.
    virtual void start();
};

#endif // FILENODE_H
