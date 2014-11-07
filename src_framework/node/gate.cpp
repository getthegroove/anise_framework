#include "gate.h"
#include "node.h"
#include "nodegatetask.h"
#include <QDebug>
#include <QThreadPool>


//------------------------------------------------------------------------------
// Constructor and Destructor

CGate::CGate(QString name, QString msg_type, QObject *parent)
    : QObject(parent)
    , m_name(name)
    , m_msg_type(msg_type)
    , m_linked_node(nullptr)
    , m_linked_gate(nullptr)
    , m_input_count(0)
{

}


//------------------------------------------------------------------------------
// Public Functions

// Connect the Gate with a Node.
void CGate::link(CNode *node)
{
    // Remove linked connection to gate if exists.
    if(m_linked_gate != nullptr) {
        m_linked_gate->m_input_count--;
        m_linked_gate.clear();
    }

    m_linked_node = node;
}

// Connect to the gate of another Node.
bool CGate::link(QSharedPointer<CGate> &gate)
{
    // Unlink the node that could have been linked to keep the state consistent.
    m_linked_node = nullptr;

    // Remove linked connection to gate if exists.
    if(!m_linked_gate.isNull()) {
        m_linked_gate->m_input_count--;
        m_linked_gate.clear();
    }

    if(type() != gate->type()) {
        qWarning() << "CGate::link(): Uncompatible gates tried to be linked."
                   << "(" << type() << ") -> (" << gate->type() << ")" << endl;
        return false;
    }

    m_linked_gate = gate;

    // Register in the target gate that a new input is being linked.
    gate->m_input_count++;

    return true;
}


//------------------------------------------------------------------------------
// Public Slots

void CGate::inputData(CConstDataPointer &data)
{
    // Verify if this gate is linked to a node or a gate.
    if(m_linked_node != nullptr) {
        // The gate is linked to a node. Let the node process the data in
        // ... another thread.
        CConstDataPointer cdata(data);
        m_linked_node->processData(m_name, cdata);
    }
    else if(!m_linked_gate.isNull()) {
        // The node is linked to another gate.
        m_linked_gate->inputData(data);
    }
}


//------------------------------------------------------------------------------
// Private Functions
