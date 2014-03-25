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
    // Data is sent to this function.
    virtual void data();

  protected:
    // The place where we are able to initialize "Data" data structures which we
    // ... would like to use within the Node.
    virtual void init(const CDataFactory &data_factory);

  public: // TODO: Remove this line.
    // Start reading the file the user selected.
    virtual void start();
};

#endif // FILENODE_H
