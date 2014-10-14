#include "nodestarttask.h"

//------------------------------------------------------------------------------
// Constructor and Destructor

CNodeStartTask::CNodeStartTask(CNode &node, QObject *parent /* = 0 */)
    : QObject(parent)
    , m_node(node)
{

}


//------------------------------------------------------------------------------
// Public Functions

void CNodeStartTask::run()
{
    bool success;

    success = m_node.start();

    // Tell whoever is interested that the task has been finished (either
    // ... successfull or not).
    emit taskFinished(success);
}

//------------------------------------------------------------------------------
// Private Functions
