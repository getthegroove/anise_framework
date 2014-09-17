#ifndef SKELETON_H
#define SKELETON_H

#include "node.h"
#include "nodeconfig.h"
#include <QObject>
#include <QString>

class CSkeleton : public CNode
{
  Q_OBJECT

  private:
    QSharedPointer<CTableData> m_table;

  public:
    // Constructor
    explicit CSkeleton(const CNodeConfig &config, QObject *parent = 0);
    // Set the configuration template for this Node.
    static void configure(CNodeConfig &config);

  public slots:
    // Receive data sent by other nodes connected to this node.
    virtual void data(QSharedPointer<CData> data);

  protected:
    // Initialize "Data" structures which we would like to use within the Node.
    virtual void init(const CDataFactory &data_factory);
    // Function called when the simulation is started.
    virtual void start();
};

#endif // SKELETON_H
