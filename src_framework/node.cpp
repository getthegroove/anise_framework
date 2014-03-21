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


bool CNode::connect(QString output_name, CNode &target, QString input_name)
{
    auto src_gate = findGate(output_name);
    if(src_gate.isNull()) {
        return false;
    }

    auto dest_gate = target.findGate(input_name);
    if(dest_gate.isNull()) {
        return false;
    }

    // Link the output of the src gate with the dest gate of another node.
    if(!src_gate->link(dest_gate)) {
        qWarning() << "CNode::connect() Warning: Could not connect nodes."
                   << "(" << endl; // TODO: add the node names in here.
    }
    return true;
}

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
        // Point the gate to this object, as it owns the gate.
        gate->link(this, it->slot);
        m_input_gates.append(QSharedPointer<CGate>(gate));
    }

    // Iterate over the output templates.
    auto output_templates = config.getOutputTemplates();
    for(auto it = output_templates.begin(); it != output_templates.end(); ++it) {
        CGate *gate = new CGate(it->name, it->msg_type);
        m_output_gates.append(QSharedPointer<CGate>(gate));
    }
}


QSharedPointer<CGate> CNode::findGate(QString name)
{
    // Use C++11 for range loops.
    for(auto gate : m_output_gates) {
        if(gate->name() == name) {
            // Connect the node with the other node.
            return gate;
        }
    }

    // Nothing found, return a null pointer.
    return QSharedPointer<CGate>();
}

//------------------------------------------------------------------------------
// Private Slots



