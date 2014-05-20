#ifndef NODE_H
#define NODE_H

#include "nodeconfig.h"
#include "gate.h"
#include "data.h"
#include <QObject>
#include <QSharedPointer>
#include <QString>

class CNodeFactory;
class CDataFactory;
class CNodeMesh;

class CNode : public QObject
{
  Q_OBJECT

  friend CNodeFactory;
  friend CNodeMesh;

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
    bool connect(QString output_name, const CNode &target, QString input_name);
    // Return the number of gates.
    int inputGatesSize();
    int outputGatesSize();
    // Return how many input links a gate has.
    int inputLinkCount(QString gate_name);

  public slots:
    // Function that will process data sent to the node.
    virtual void data(QSharedPointer<CData> data) = 0;

  protected:
    // Let the Node perform some initialization tasks. Data structures must
    // ... be obtained through the 'data_factory' handle.
    virtual void init(const CDataFactory &data_factory) = 0;
    // Function that signal the Nodes that the simulation is starting. Nodes
    // ... that do not receive messages may start processing information now.
    virtual void start() = 0;
    // Forward a message through a particular gate.
    bool commit(QString gate_name, QSharedPointer<CData> data);

  private:
    const CNodeConfig m_config;

    // Do not allow instantiations of this class through the default
    // ... constructor.
    explicit CNode() {}
    // Establish the Gate arrangements in the node, e.g., number of inputs,
    // ... outputs, type of messages received by gates, etc
    void setupGates(const CNodeConfig &config);
    // Find a particular gate by name.
    QSharedPointer<CGate> findInputGate(QString name) const;
    QSharedPointer<CGate> findOutputGate(QString name) const;
};

#endif // NODE_H
