#include "gate.h"
#include <QDebug>

//------------------------------------------------------------------------------
// Constructor and Destructor

CGate::CGate(QString name_id, QString msg_type, QObject *parent)
    : QObject(parent)
    , m_name_id(name_id)
    , m_msg_type(msg_type)
{

}

//------------------------------------------------------------------------------
// Public Functions

bool CGate::link(QObject *receiver, const char *slot)
{
    return QObject::connect(this, SIGNAL(processData()), receiver, slot);
}

bool CGate::link(QSharedPointer<CGate> gate)
{
    if(type() != gate->type()) {
        qWarning() << "CGate::link() Warning: Uncompatible gates linked."
                   << "(" << type() << ") -> (" << gate->type() << ")" << endl;
        return false;
    }

    return QObject::connect(this, SIGNAL(processData()),
        gate.data(), SLOT(inputData()));
}


//------------------------------------------------------------------------------
// Public Slots

void CGate::inputData()
{
    // Compare the message types to see if it's the expected type.
    // if(m_msg_type != msg_type) {
    //     // Messages do not match.
    //     qDebug() << "CGate::processMessage() Error:"
    //              << "Unexpected Message received.";
    //     return;
    // }

    emit processData();
}


//------------------------------------------------------------------------------
// Private Functions


//------------------------------------------------------------------------------
// Private Slots
