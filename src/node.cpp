#include "node.h"
#include <QDebug>
#include <QCoreApplication>

//------------------------------------------------------------------------------
// Constructor and Destructor

CNode::CNode(const CNodeConfig &config, QObject *parent/*= 0*/)
 : QObject(parent)
 , m_input_boxes()
 , m_output_boxes()
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
    // Get the QList of gates.
    auto input_boxes = config.getInputBoxTemplate();
    auto output_boxes = config.getOutputBoxTemplate();

    if(input_boxes.count() > 1 || output_boxes.count() > 1) {
        qDebug() << "CNode::setup() Error:"
                 << "Multiple gates not yet supported.";
        QCoreApplication::exit(1);
    }

    // Iterate the input boxes.
    for(auto it = input_boxes.begin(); it != input_boxes.end(); ++it) {
        // Create a new gate box with its configuration parameters.
        CGateBox *box = new CGateBox(it->sync);
        // Add gates to the GateBox.
        auto gates = it->gates;  // A QList of gates.
        for(auto jt = gates.begin(); jt != gates.end(); ++jt) {
            CGate *gate = new CGate(jt->name, jt->msg_type);
            box->addGate(QSharedPointer<CGate>(gate));
        }

        m_input_boxes.append(QSharedPointer<CGateBox>(box));
    }
}


//------------------------------------------------------------------------------
// Private Slots



