#ifndef FILENODE_H
#define FILENODE_H

#include "node/node.h"
#include "node/nodeconfig.h"
#include "filedata/filedata.h"
#include <QObject>
#include <QString>

class CFileNode: public CNode
{
  Q_OBJECT

  private:
    QSharedPointer<CFileData> m_file;

  public:
    // Constructor
    explicit CFileNode(const CNodeConfig &config, QObject *parent = 0);
    // Set the configuration template for this Node.
    static void configure(CNodeConfig &config);

  protected:
    // The place where we are able to initialize "Data" data structures which we
    // ... would like to use within the Node.
    virtual void init(const CDataFactory &data_factory);
    // Function called when the simulation is started.
    // ... Read the file set in the parameters.
    virtual bool start();
    // Receive data sent by other nodes connected to this node.
    virtual void data(QString gate_name, QSharedPointer<CData> data);
};

#endif // FILENODE_H
