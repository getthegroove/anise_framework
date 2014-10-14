#ifndef NODE_H
#define NODE_H

#include "nodeconfig.h"
#include "../data/data.h"
#include "gate.h"
#include <QObject>
#include <QSharedPointer>
#include <QString>

class CNodeFactory;
class CDataFactory;
class CNodeMesh;
class CNodeGateTask;
class CNodeStartTask;

class CNode : public QObject
{
  Q_OBJECT

  friend CNodeFactory;
  friend CNodeMesh;
  friend CNodeGateTask;
  friend CNodeStartTask;

  protected:
    // Collection of input gates.
    QList<QSharedPointer<CGate>> m_input_gates;
    // Collection of output gates.
    QList<QSharedPointer<CGate>> m_output_gates;

  signals:
    // Emitted with 'true' when the node starts processing, 'false'
    // ... when it becomes idle.
    void processing(bool not_idle);


  public:
    explicit CNode(const CNodeConfig &config, QObject *parent = 0);
    virtual ~CNode();

    // Get the configuration parameters of this node.
    const CNodeConfig &getConfig() const;
    // Connect the output of this node to the input of another node.
    bool connect(QString output_name, const CNode &target, QString input_name);
    // Return the number of gates.
    int inputGatesSize() const;
    int outputGatesSize() const;
    // Return how many input links a gate has.
    int inputLinkCount(QString gate_name) const;
    // Get the name of a gate by index.
    QString inputGateName(qint8 index) const;
    // Function that will process data sent to the node in another thread. When
    // ... finished, the signal processingFinished() is emitted.
    void processData(QString gate_name, QSharedPointer<CData> &data);
    // Is the node currently processing data?
    bool isProcessing() const;

  protected:
    // Let the Node perform some initialization tasks. Data structures must
    // ... be obtained through the 'data_factory' handle.
    virtual void init(const CDataFactory &data_factory) = 0;
    // Function that signal the Nodes that the simulation is starting.
    // ... Return true on successfull start.
    virtual bool start() = 0;
    // Function that processes data sent to this node. This processing is
    //... performed in another thread.
    virtual void data(QString gate_name, QSharedPointer<CData> data) = 0;
    // Forward a message through a particular gate.
    void commit(QString gate_name, QSharedPointer<CData> data);
    // Send an Error message through the specified gate.
    void commitError(QString gate_name, QString error_msg);

  private:
    const CNodeConfig m_config;
    // Flag that indicates if the node is in the middle of processing
    // ... something or not.
    bool m_processing;
    // Queue of data structures waiting to be processed.
    QQueue<QPair<QString,QSharedPointer<CData>>> m_processing_queue;
    // List of commits done by the node while it was processing data.
    QList<QPair<QString, QSharedPointer<CData>>> m_commit_list;
    // Allow to commit data only inside the "data()" function of the
    // ... node.
    bool m_allow_commit;

    // Do not allow instantiations of this class through the default
    // ... constructor.
    explicit CNode() {}
    // Establish the Gate arrangements in the node, e.g., number of inputs,
    // ... outputs, type of messages received by gates, etc
    void setupGates(const CNodeConfig &config);
    // Find a particular gate by name.
    QSharedPointer<CGate> findInputGate(QString name) const;
    QSharedPointer<CGate> findOutputGate(QString name) const;
    void startGateTask(QString gate_name, QSharedPointer<CData> &data);
    // Safely access and modify the 'm_processing' flag.
    void setProcessing(bool processing);

  private slots:
    void onTaskFinished();
};

#endif // NODE_H
