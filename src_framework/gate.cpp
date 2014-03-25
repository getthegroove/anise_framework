#include "gate.h"
#include "node.h"
#include <QDebug>

//------------------------------------------------------------------------------
// Constructor and Destructor

CGate::CGate(QString name, QString msg_type, QObject *parent)
    : QObject(parent)
    , m_name(name)
    , m_msg_type(msg_type)
{

}

//------------------------------------------------------------------------------
// Public Functions

// Connect the Gate with an internal function of the Node that owns the gate.
bool CGate::link(CNode *receiver, const char *slot)
{
    return QObject::connect(this, SIGNAL(forwardData(QSharedPointer<CData>)),
        receiver, slot, Qt::QueuedConnection);
}

// Connect to the gate of another Node.
bool CGate::link(QSharedPointer<CGate> gate)
{
    if(type() != gate->type()) {
        qWarning() << "CGate::link() Warning: Uncompatible gates linked."
                   << "(" << type() << ") -> (" << gate->type() << ")" << endl;
        return false;
    }

    return QObject::connect(this, SIGNAL(forwardData(QSharedPointer<CData>)),
        gate.data(), SLOT(inputData(QSharedPointer<CData>)),
        Qt::QueuedConnection);
}


//------------------------------------------------------------------------------
// Public Slots

void CGate::inputData(QSharedPointer<CData> data)
{
    // Compare the message types to see if it's the expected type.
    // if(m_msg_type != msg_type) {
    //     // Messages do not match.
    //     qDebug() << "CGate::processMessage() Error:"
    //              << "Unexpected Message received.";
    //     return;
    // }

    emit forwardData(data);
}


//------------------------------------------------------------------------------
// Private Functions


//------------------------------------------------------------------------------
// Private Slots
