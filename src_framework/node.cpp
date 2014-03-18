#include "node.h"
#include <QDebug>
#include <QCoreApplication>

//------------------------------------------------------------------------------
// Constructor and Destructor

CNode::CNode(const CNodeConfig &config, QObject *parent/*= 0*/)
 : QObject(parent)
 , m_input_gates()
 , m_output_gates()
{
    // Create the gates and gate boxes of this node.
    setupGates(config);
}

CNode::~CNode()
{

}

//------------------------------------------------------------------------------
// Public Functions


//------------------------------------------------------------------------------
// Public Slots


//------------------------------------------------------------------------------
// Private Functions

void CNode::setupGates(const CNodeConfig &config)
{
    // Iterate the input templates.
    auto input_templates = config.getInputTemplates();
    for(auto it = input_templates.begin(); it != input_templates.end(); ++it) {
        CGate *gate = new CGate(it->name, it->msg_type);
        m_input_gates.append(QSharedPointer<CGate>(gate));
    }

    // Iterate over the output templates.
    auto output_templates = config.getOutputTemplates();
    for(auto it = output_templates.begin(); it != output_templates.end(); ++it) {
        CGate *gate = new CGate(it->name, it->msg_type);
        m_output_gates.append(QSharedPointer<CGate>(gate));
    }
}


//------------------------------------------------------------------------------
// Private Slots



