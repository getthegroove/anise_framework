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
    , m_linked_gates()
    , m_input_count(0)
{

}


//------------------------------------------------------------------------------
// Public Functions

// Connect with a Node.
void CGate::link(CNode *node)
{
    // Remove linked connection to gate if exists.
    if(m_linked_gates.size() > 0) {
        qWarning() << "Linking gate to another gate already linked to gates.";
        m_linked_gates.clear();
    }

    m_linked_node = node;
}

// Connect to another gate.
bool CGate::link(QSharedPointer<CGate> &gate)
{
    // Verify the compatibility of the gate types.
    if(type() != gate->type()) {
        qWarning() << "Uncompatible gates tried to be linked."
                   << "(" << type() << ") -> (" << gate->type() << ")" << endl;
        return false;
    }

    // Unlink the node that could have been linked to keep the state consistent.
    if(m_linked_node != nullptr) {
        qWarning() << "Linking gate to another gate while already linked to a node.";
    }
    m_linked_node = nullptr;

    // Add linked gate.
    m_linked_gates.append(gate);

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
    else if(m_linked_gates.size() > 0) {
        // Gate linked to other gates.
        auto it = m_linked_gates.begin();
        while(it != m_linked_gates.end()) {
            QSharedPointer<CGate> &gate = *it;
            gate->inputData(data);
            ++it;
        }
    }
}


//------------------------------------------------------------------------------
// Private Functions
