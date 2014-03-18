#ifndef NODE_H
#define NODE_H

#include "nodeconfig.h"
#include "message.h"
#include "gate.h"
#include "nodeinfo.h"
#include <QObject>
#include <QSharedPointer>

class CNode : public QObject
{
  Q_OBJECT

  protected:
    // Collection of input gates.
    QList<QSharedPointer<CGate>> m_input_gates;
    // Collection of output gates.
    QList<QSharedPointer<CGate>> m_output_gates;

  public:
    explicit CNode(const CNodeConfig &config, QObject *parent = 0);
    virtual ~CNode();

    // Functions that must be overwritten by derived nodes.
    // -----------------------------------------------------------

    // This is called whenever the inputs have received something and the
    // ... received messages are ready to be processed.
    virtual void process() = 0;

    // Facilities for fetching messages from the inputs.
    // -----------------------------------------------------------

    // Facilities for sending messages to the outputs.
    // -----------------------------------------------------------

  private:
    // Do not allow instantiations of this class through the default
    // ... constructor.
    explicit CNode() {};

    // Establish the Gate arrangements in the node, e.g., number of inputs,
    // ... outputs, type of messages received by gates, etc
    void setupGates(const CNodeConfig &config);
};

#endif // NODE_H
