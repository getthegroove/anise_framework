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
    qDebug() << "CNode.~CNode():: Info: Node '"
             << getConfig().getName()
             << "' destroyed." << endl;
}


//------------------------------------------------------------------------------
// Public Functions

const CNodeConfig& CNode::getConfig() const
{
    return m_config;
}

bool CNode::connect(QString output_name, const CNode &target, QString input_name)
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

int CNode::inputGatesSize()
{
    return m_input_gates.size();
}

int CNode::outputGatesSize()
{
    return m_output_gates.size();
}

int CNode::inputLinkCount(QString gate_name)
{
    auto gate = findInputGate(gate_name);
    if(gate.isNull()) {
        return 0;
    }
    else {
        return gate->inputLinks();
    }
}


//------------------------------------------------------------------------------
// Protected Functions

bool CNode::commit(QString gate_name, QSharedPointer<CData> data)
{
    // TODO: program me.
    auto output_gate = findOutputGate(gate_name);
    if(output_gate.isNull()) {
        qDebug() << "CNode.commit() Warning: Could not commit data from within"
                 << "Node" << m_config.getName() << ". The gate" << gate_name
                 << "was not found." << endl;
        return false;
    }

    output_gate->inputData(data);

    return true;
}


//------------------------------------------------------------------------------
// Private Functions

void CNode::setupGates(const CNodeConfig &config)
{
    // Iterate the input templates.
    auto input_templates = config.getInputTemplates();
    for(auto it = input_templates.begin(); it != input_templates.end(); ++it) {
        CGate *gate = new CGate(it->name, it->msg_type);
        // Point the gate to this object, as it owns the gate.
        gate->link(this, SLOT(data(QSharedPointer<CData>)));
        m_input_gates.append(QSharedPointer<CGate>(gate));
    }

    // Iterate over the output templates.
    auto output_templates = config.getOutputTemplates();
    for(auto it = output_templates.begin(); it != output_templates.end(); ++it) {
        CGate *gate = new CGate(it->name, it->msg_type);
        m_output_gates.append(QSharedPointer<CGate>(gate));
    }
}

QSharedPointer<CGate> CNode::findInputGate(QString name) const
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

QSharedPointer<CGate> CNode::findOutputGate(QString name) const
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

