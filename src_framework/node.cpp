#include "node.h"
#include <QDebug>
#include <QCoreApplication>

//------------------------------------------------------------------------------
// Constructor and Destructor

CNode::CNode(const CNodeConfig &config, QObject *parent/*= 0*/)
 : QObject(parent)
 , m_input_gates()
 , m_output_gates()
 , m_config(config)
{
    // Create the gates and gate boxes of this node.
    setupGates(config);
}

CNode::~CNode()
{

}

//------------------------------------------------------------------------------
// Public Functions

const CNodeConfig& CNode::getConfig() const
{
    return m_config;
}

bool CNode::connect(QString output_name, CNode &target, QString input_name)
{
    auto src_gate = findOutputGate(output_name);
    if(src_gate.isNull()) {
        qWarning() << "CNode::connect() Warning: Could not find gate"
                   << output_name << "in output Node" << m_config.getName() << "."
                   <<  endl;
        return false;
    }

    auto dest_gate = target.findInputGate(input_name);
    if(dest_gate.isNull()) {
        qWarning() << "CNode::connect() Warning: Could not find gate"
                   << input_name << "in input Node"
                   << target.m_config.getName() << "."
                   << endl;
        return false;
    }

    // Link the output of the src gate with the dest gate of another node.
    if(!src_gate->link(dest_gate)) {
        qWarning() << "CNode::connect() Warning: Could not connect nodes."
                   << "(" << m_config.getName() << ") -> ("
                   << target.m_config.getName() << ")" << endl;
        return false;
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
        gate->link(this, SLOT(data()));
        m_input_gates.append(QSharedPointer<CGate>(gate));
    }

    // Iterate over the output templates.
    auto output_templates = config.getOutputTemplates();
    for(auto it = output_templates.begin(); it != output_templates.end(); ++it) {
        CGate *gate = new CGate(it->name, it->msg_type);
        m_output_gates.append(QSharedPointer<CGate>(gate));
    }
}

QSharedPointer<CGate> CNode::findInputGate(QString name)
{
    // Use C++11 for range loops.
    for(auto gate : m_input_gates) {
        if(gate->name() == name) {
            // Connect the node with the other node.
            return gate;
        }
    }

    // Nothing found, return a null pointer.
    return QSharedPointer<CGate>();
}

QSharedPointer<CGate> CNode::findOutputGate(QString name)
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



