#ifndef NODE_H
#define NODE_H

#include "nodeconfig.h"
#include "gate.h"
#include <QObject>
#include <QSharedPointer>

class CNodeFactory;
class CDataFactory;

class CNode : public QObject
{
  Q_OBJECT

  friend CNodeFactory;

  protected:
    // Collection of input gates.
    QList<QSharedPointer<CGate>> m_input_gates;
    // Collection of output gates.
    QList<QSharedPointer<CGate>> m_output_gates;

  public:
    explicit CNode(const CNodeConfig &config, QObject *parent = 0);
    virtual ~CNode();

    // Get the configuration parameters of this node.
    const CNodeConfig &getConfig() const;
    // Connect the output of this node to the input of another node.
    bool connect(QString output_name, CNode &target, QString input_name);

  public slots:
    // Function that will process data sent to the node.
    virtual void data() = 0;


  protected:
    // Let the Node perform some initialization tasks. Data structures must
    // ... be obtained through the 'data_factory' handle.
    virtual void init(const CDataFactory &data_factory) = 0;

  private:
    const CNodeConfig m_config;

    // Do not allow instantiations of this class through the default
    // ... constructor.
    explicit CNode() {}

    // Establish the Gate arrangements in the node, e.g., number of inputs,
    // ... outputs, type of messages received by gates, etc
    void setupGates(const CNodeConfig &config);
    // Find a particular gate by name.
    QSharedPointer<CGate> findInputGate(QString name);
    QSharedPointer<CGate> findOutputGate(QString name);
};

#endif // NODE_H
