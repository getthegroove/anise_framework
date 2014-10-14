#include "nodegatetask.h"

//------------------------------------------------------------------------------
// Constructor and Destructor

CNodeGateTask::CNodeGateTask(CNode &node, QString gate_name,
    QSharedPointer<CData> &data, QObject *parent /* = 0 */)
    : QObject(parent)
    , m_node(node)
    , m_gate_name(gate_name)
    , m_data(data)
{

}


//------------------------------------------------------------------------------
// Public Functions

void CNodeGateTask::run()
{
    // Enable the usage of the commit functions only while the data function
    // ... is called.
    m_node.m_allow_commit = true;
    m_node.data(m_gate_name, m_data);
    // Dissalow the commit functions outside of the nodes' data function.
    m_node.m_allow_commit = false;

    // Tell whoever is interested that the task has been finished.
    emit taskFinished();
}

//------------------------------------------------------------------------------
// Private Functions
